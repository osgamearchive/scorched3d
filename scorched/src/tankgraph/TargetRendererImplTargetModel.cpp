////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Scorched3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////

#include <tankgraph/TargetRendererImplTargetModel.h>
#include <target/TargetLife.h>
#include <target/TargetState.h>
#include <target/TargetShield.h>
#include <landscape/Landscape.h>
#include <landscape/ShadowMap.h>
#include <3dsparse/ModelStore.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLViewPort.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLCamera.h>
#include <weapons/Accessory.h>
#include <weapons/Shield.h>
#include <GLW/GLWToolTip.h>
#include <dialogs/TutorialDialog.h>
#include <client/ScorchedClient.h>
#include <engine/ActionController.h>
#include <graph/ModelRendererStore.h>
#include <graph/OptionsDisplay.h>

TargetRendererImplTargetModel::TargetRendererImplTargetModel(Target *target,
	ModelID model, ModelID burntModel, 
	float scale, float color) :
	target_(target),
	canSeeTank_(false), burnt_(false),
	shieldHit_(0.0f), totalTime_(0.0f),
	posX_(0.0), posY_(0.0), posZ_(0.0),
	targetTips_(target),
	scale_(scale), color_(color)
{
	modelRenderer_ = new ModelRendererSimulator(
		ModelRendererStore::instance()->loadModel(model));
	burntModelRenderer_ = new ModelRendererSimulator(
		ModelRendererStore::instance()->loadModel(burntModel));
}

TargetRendererImplTargetModel::~TargetRendererImplTargetModel()
{
	delete modelRenderer_;
	delete burntModelRenderer_;
}

void TargetRendererImplTargetModel::simulate(float frameTime)
{
	frameTime *= ScorchedClient::instance()->getActionController().getFast().asFloat();
	totalTime_ += frameTime;
	if (shieldHit_ > 0.0f)
	{
		shieldHit_ -= frameTime / 25.0f;
		if (shieldHit_ < 0.0f) shieldHit_ = 0.0f;
	}

	if (burnt_) burntModelRenderer_->simulate(frameTime * 20.0f);
	else modelRenderer_->simulate(frameTime * 20.0f);
}

void TargetRendererImplTargetModel::draw(float distance, bool shadowdraw)
{
	// Don't draw the tank/target if we are drawing shadows and shadows are off
	// for this target
	if (shadowdraw &&
		!target_->getTargetState().getDisplayHardwareShadow())
	{
		return;
	}

	// Check we can see the tank
	canSeeTank_ = true;
	if (!GLCameraFrustum::instance()->
		sphereInFrustum(target_->getLife().getTargetPosition().asVector(), 
		target_->getLife().getSize().Max().asFloat() / 2.0f,
		GLCameraFrustum::FrustrumRed) ||
		!target_->getAlive())
	{
		canSeeTank_ = false;
		return;
	}

	// Get the size of the object
	float size = target_->getLife().getAabbSize().Max().asFloat();
	Accessory *shieldAcc = target_->getShield().getCurrentShield();
	if (shieldAcc)
	{
		Shield *shield = (Shield *) shieldAcc->getAction();
		size = MAX(shield->getBoundingSize().asFloat(), size);
	}	

	// Figure out the drawing distance
	float drawDistance = OptionsDisplay::instance()->getDrawDistance() * size * 2.0f;
	float drawDistanceFade =  OptionsDisplay::instance()->getDrawDistanceFade();
	float drawDistanceFadeStart = drawDistance - drawDistanceFade;
	if (distance > drawDistance) return;
	float fade = 1.0f;
	if (distance > drawDistanceFadeStart)
	{
		fade = 1.0f - ((distance - drawDistanceFadeStart) / drawDistanceFade);
	}

	createParticle(target_);
	storeTank2DPos();

	// Add the tank shadow
	if (target_->getTargetState().getDisplayShadow())
	{
		Landscape::instance()->getShadowMap().addCircle(
			target_->getLife().getTargetPosition()[0].asFloat(), 
			target_->getLife().getTargetPosition()[1].asFloat(), 
			target_->getLife().getSize().Max().asFloat() + 2.0f,
			fade);
	}

	// Draw the tank model
	static float rotMatrix[16];
	target_->getLife().getQuaternion().getOpenGLRotationMatrix(rotMatrix);

	glColor4f(color_, color_, color_, 1.0f);
	glPushMatrix();
		glTranslatef(
			target_->getLife().getTargetPosition()[0].asFloat(), 
			target_->getLife().getTargetPosition()[1].asFloat(), 
			target_->getLife().getTargetPosition()[2].asFloat());
		glMultMatrixf(rotMatrix);
		glScalef(scale_, scale_, scale_);
		if (burnt_) burntModelRenderer_->drawBottomAligned(distance, fade);
		else modelRenderer_->drawBottomAligned(distance, fade);
	glPopMatrix();
}

void TargetRendererImplTargetModel::draw2d()
{
	if (!canSeeTank_) return;

	// Add the tooltip that displays the tank info
	if (target_->getName()[0])
	{
		GLWToolTip::instance()->addToolTip(&targetTips_.targetTip,
			float(posX_) - 10.0f, float(posY_) - 10.0f, 20.0f, 20.0f);
	}
}

void TargetRendererImplTargetModel::drawSecond(float distance)
{
	if (!canSeeTank_) return;

	drawParachute(target_);
	drawShield(target_, shieldHit_, totalTime_);
}

void TargetRendererImplTargetModel::shieldHit()
{
	shieldHit_ = 0.25f;
}

void TargetRendererImplTargetModel::storeTank2DPos()
{
	if (!target_->getName()[0]) return;

	Vector &tankTurretPos = 
		target_->getLife().getCenterPosition().asVector();
	Vector camDir = 
		GLCamera::getCurrentCamera()->getLookAt() - 
		GLCamera::getCurrentCamera()->getCurrentPos();
	Vector tankDir = tankTurretPos - 
		GLCamera::getCurrentCamera()->getCurrentPos();

	if (camDir.dotP(tankDir) < 0.0f)
	{
		posX_ = - 1000.0;
	}
	else
	{
		static GLdouble modelMatrix[16];
		static GLdouble projMatrix[16];
		static GLint viewport[4];

		glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
		glGetIntegerv(GL_VIEWPORT, viewport);

		viewport[2] = GLViewPort::getWidth();
		viewport[3] = GLViewPort::getHeight();
		int result = gluProject(
			tankTurretPos[0], 
			tankTurretPos[1], 
			tankTurretPos[2],
			modelMatrix, projMatrix, viewport,
			&posX_, 
			&posY_,
			&posZ_);
	}
}

void TargetRendererImplTargetModel::fired()
{
}

void TargetRendererImplTargetModel::targetBurnt()
{
	burnt_ = true;
}

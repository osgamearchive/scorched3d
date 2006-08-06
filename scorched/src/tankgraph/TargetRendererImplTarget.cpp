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

#include <tankgraph/TargetRendererImplTarget.h>
#include <landscape/Landscape.h>
#include <landscape/ShadowMap.h>
#include <3dsparse/ModelStore.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLViewPort.h>
#include <GLEXT/GLState.h>
#include <GLW/GLWToolTip.h>
#include <dialogs/TutorialDialog.h>
#include <client/MainCamera.h>

TargetRendererImplTarget::TargetRendererImplTarget(Target *target,
	ModelID model, ModelID burntModel, 
	float scale) :
	target_(target),
	canSeeTank_(false), burnt_(false),
	shieldHit_(0.0f), totalTime_(0.0f),
	posX_(0.0), posY_(0.0), posZ_(0.0),
	targetTips_(target),
	scale_(scale)
{
	modelRenderer_ = new ModelRenderer(
		ModelStore::instance()->loadModel(model));
	burntModelRenderer_ = new ModelRenderer(
		ModelStore::instance()->loadModel(burntModel));
}

TargetRendererImplTarget::~TargetRendererImplTarget()
{
}

void TargetRendererImplTarget::simulate(float frameTime)
{
	totalTime_ += frameTime;
	if (shieldHit_ > 0.0f)
	{
		shieldHit_ -= frameTime / 25.0f;
		if (shieldHit_ < 0.0f) shieldHit_ = 0.0f;
	}

	if (burnt_) burntModelRenderer_->simulate(frameTime * 20.0f);
	else modelRenderer_->simulate(frameTime * 20.0f);
}

void TargetRendererImplTarget::draw(float distance)
{
	// Check we can see the tank
	canSeeTank_ = true;
	if (!GLCameraFrustum::instance()->
		sphereInFrustum(target_->getTargetPosition(), 
		target_->getLife().getSize().Max() / 2.0f,
		GLCameraFrustum::FrustrumRed) ||
		!target_->getAlive())
	{
		canSeeTank_ = false;
		return;
	}

	createParticle(target_);
	storeTank2DPos();

	// Add the tank shadow
	Landscape::instance()->getShadowMap().addCircle(
		target_->getTargetPosition()[0], 
		target_->getTargetPosition()[1], 
		target_->getLife().getSize().Max() + 2.0f);

	// Draw the tank model
	glPushMatrix();
		glTranslatef(
			target_->getTargetPosition()[0], 
			target_->getTargetPosition()[1], 
			target_->getTargetPosition()[2]);
		glRotatef(target_->getLife().getRotation(), 
			0.0f, 0.0f, 1.0f);
		glScalef(scale_, scale_, scale_);
		if (burnt_) burntModelRenderer_->drawBottomAligned();
		else modelRenderer_->drawBottomAligned();
	glPopMatrix();
}

void TargetRendererImplTarget::draw2d()
{
	if (!canSeeTank_) return;

	// Add the tooltip that displays the tank info
	if (target_->getName()[0])
	{
		GLWToolTip::instance()->addToolTip(&targetTips_.targetTip,
			float(posX_) - 10.0f, float(posY_) - 10.0f, 20.0f, 20.0f);
	}
}

void TargetRendererImplTarget::drawSecond(float distance)
{
	if (!canSeeTank_) return;

	drawParachute(target_);
	drawShield(target_, shieldHit_, totalTime_);
}

void TargetRendererImplTarget::shieldHit()
{
	shieldHit_ = 0.25f;
}

void TargetRendererImplTarget::storeTank2DPos()
{
	Vector &tankTurretPos = 
		target_->getCenterPosition();
	Vector camDir = 
		MainCamera::instance()->getCamera().getLookAt() - 
		MainCamera::instance()->getCamera().getCurrentPos();
	Vector tankDir = tankTurretPos - 
		MainCamera::instance()->getCamera().getCurrentPos();

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

void TargetRendererImplTarget::fired()
{
}

void TargetRendererImplTarget::targetBurnt()
{
	burnt_ = true;
}

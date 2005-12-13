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

#include <tankgraph/TargetModelRenderer.h>
#include <landscape/Landscape.h>
#include <landscape/ShadowMap.h>
#include <3dsparse/ModelStore.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLViewPort.h>
#include <GLEXT/GLState.h>
#include <GLW/GLWToolTip.h>
#include <client/MainCamera.h>

TargetModelRenderer::TargetModelRenderer(Target *target) :
	target_(target),
	canSeeTank_(false),
	shieldHit_(0.0f), totalTime_(0.0f),
	posX_(0.0), posY_(0.0), posZ_(0.0),
	targetTips_(target)
{
	modelRenderer_ = new ModelRenderer(
		ModelStore::instance()->loadModel(target_->getModel().getTargetModel()));
}

TargetModelRenderer::~TargetModelRenderer()
{
}

void TargetModelRenderer::simulate(float frameTime)
{
	totalTime_ += frameTime;
	if (shieldHit_ > 0.0f)
	{
		shieldHit_ -= frameTime / 25.0f;
		if (shieldHit_ < 0.0f) shieldHit_ = 0.0f;
	}

	modelRenderer_->simulate(frameTime);
}

void TargetModelRenderer::draw()
{
	// Check we can see the tank
	canSeeTank_ = true;
	if (!GLCameraFrustum::instance()->
		sphereInFrustum(target_->getTargetPosition(), 1))
	{
		canSeeTank_ = false;
		return;
	}

	storeTank2DPos();

	// Add the tank shadow
	Landscape::instance()->getShadowMap().addCircle(
		target_->getTargetPosition()[0], 
		target_->getTargetPosition()[1], 
		4.0f);

	// Draw the tank model
	glPushMatrix();
		glTranslatef(
			target_->getTargetPosition()[0], 
			target_->getTargetPosition()[1], 
			target_->getTargetPosition()[2]);
		glScalef(0.01f, 0.01f, 0.01f);
		modelRenderer_->drawBottomAligned();
	glPopMatrix();
}

void TargetModelRenderer::draw2d()
{
	if (!canSeeTank_) return;

	// Add the tooltip that displays the tank info
	GLWToolTip::instance()->addToolTip(&targetTips_.targetTip,
		float(posX_) - 10.0f, float(posY_) - 10.0f, 20.0f, 20.0f);
}

void TargetModelRenderer::drawSecond()
{
	if (!canSeeTank_) return;

	drawParachute(target_);
	drawShield(target_, shieldHit_, totalTime_);
}

void TargetModelRenderer::shieldHit()
{
	shieldHit_ = 0.25f;
}

void TargetModelRenderer::storeTank2DPos()
{
	Vector &tankTurretPos = 
		target_->getTargetPosition();
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

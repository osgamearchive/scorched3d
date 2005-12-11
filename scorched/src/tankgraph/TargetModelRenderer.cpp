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
#include <GLEXT/GLState.h>

TargetModelRenderer::TargetModelRenderer(Target *target) :
	target_(target),
	canSeeTank_(false)
{
	modelRenderer_ = new ModelRenderer(
		ModelStore::instance()->loadModel(target_->getModel().getTargetModel()));
}

TargetModelRenderer::~TargetModelRenderer()
{
}

void TargetModelRenderer::simulate(float frameTime)
{
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
		modelRenderer_->draw();
	glPopMatrix();
}

void TargetModelRenderer::draw2d()
{
}

void TargetModelRenderer::drawSecond()
{
	if (!canSeeTank_) return;

	drawParachute(target_);
	drawShield(target_, 0.0f, 0.0f);
}

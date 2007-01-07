////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
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

#include <tankgraph/RenderTargets.h>
#include <tankgraph/TargetRendererImplTank.h>
#include <tankgraph/RenderTracer.h>
#include <tankgraph/RenderGeoms.h>
#include <tank/TankContainer.h>
#include <target/TargetLife.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <GLEXT/GLCamera.h>
#include <algorithm>

RenderTargets *RenderTargets::instance_ = 0;

RenderTargets *RenderTargets::instance()
{
	if (!instance_)
	{
		instance_ = new RenderTargets;
	}
	return instance_;
}

RenderTargets::RenderTargets()
{
}

RenderTargets::~RenderTargets()
{
}

void RenderTargets::Renderer2D::draw(const unsigned state)
{
	RenderTargets::instance()->draw(RenderTargets::Type2D, state);
}

void RenderTargets::Renderer3D::draw(const unsigned state)
{
	RenderTracer::instance()->draw(state);
	RenderGeoms::instance()->draw(state);
	RenderTargets::instance()->draw(RenderTargets::Type3D, state);
}

void RenderTargets::Renderer2D::simulate(const unsigned state, float simTime)
{
	// Simulate the HUD
	TargetRendererImplTankHUD::simulate(simTime);
	TargetRendererImplTankAIM::simulate(simTime);

	// Simulate all of the tanks
	std::map<unsigned int, Target *> &targets = 
		ScorchedClient::instance()->getTargetContainer().getTargets();
	std::map<unsigned int, Target *>::iterator itor;
	for (itor = targets.begin();
		itor != targets.end();
		itor++)
	{
		Target *target = (*itor).second;
		TargetRenderer *model = target->getRenderer();
		if (model)
		{
			model->simulate(simTime);
		}
	}
}

static inline float approx_distance(float  dx, float dy)
{
   float approx = (dx * dx) + (dy * dy);
   return approx;
}

static inline bool lt_distance(const std::pair<float, Target *> &o1, 
	const std::pair<float, Target *> &o2) 
{ 
	return o1.first > o2.first;
}

void RenderTargets::draw(DrawType dt, const unsigned state)
{
	Vector &campos = GLCamera::getCurrentCamera()->getCurrentPos();

	// Don't put fully transparent areas into the depth buffer
	GLState glstate(GLState::BLEND_ON | GLState::TEXTURE_ON | GLState::ALPHATEST_ON);
	
	// Draw all of the tanks
	std::map<unsigned int, Target *> &targets = 
		ScorchedClient::instance()->getTargetContainer().getTargets();
	std::map<unsigned int, Target *>::iterator itor;
	for (itor = targets.begin();
		itor != targets.end();
		itor++)
	{
		Target *target = (*itor).second;
		float distance = approx_distance(
				target->getLife().getTargetPosition()[0] - campos[0],
				target->getLife().getTargetPosition()[1] - campos[1]);

		// Check we have the tank model for each tank
		TargetRenderer *model = target->getRenderer();
		if (!model) continue;

		switch (dt)
		{
		case Type3D:
			model->draw(distance);
			break;
		case Type2D:
			model->draw2d();
			break;
		}
	}
}

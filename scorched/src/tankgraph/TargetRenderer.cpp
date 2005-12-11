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

#include <tankgraph/TargetRenderer.h>
#include <tankgraph/TargetParticleRenderer.h>
#include <tankgraph/TargetModelRenderer.h>
#include <tankgraph/TankModelRenderer.h>
#include <tankgraph/TracerRenderer.h>
#include <tank/TankContainer.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <client/MainCamera.h>
#include <algorithm>

TargetRenderer *TargetRenderer::instance_ = 0;

TargetRenderer *TargetRenderer::instance()
{
	if (!instance_)
	{
		instance_ = new TargetRenderer;
	}
	return instance_;
}

TargetRenderer::TargetRenderer()
{
}

TargetRenderer::~TargetRenderer()
{
}

void TargetRenderer::Renderer2D::draw(const unsigned state)
{
	TargetRenderer::instance()->draw(TargetRenderer::Type2D, state);
}

void TargetRenderer::Renderer3D::draw(const unsigned state)
{
	TracerRenderer::instance()->draw(state);
	TargetRenderer::instance()->draw(TargetRenderer::Type3D, state);
}

void TargetRenderer::Renderer3D::simulate(const unsigned state, float simTime)
{
	// Simulate the HUD
	TankModelRendererHUD::simulate(simTime);
	TankModelRendererAIM::simulate(simTime);

	// Simulate all of the tanks
	std::map<unsigned int, Target *> &targets = 
		ScorchedClient::instance()->getTargetContainer().getTargets();
	std::map<unsigned int, Target *>::iterator itor;
	for (itor = targets.begin();
		itor != targets.end();
		itor++)
	{
		Target *target = (*itor).second;

		TargetModelIdRenderer *model =
			target->getModel().getModelIdRenderer();
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

void TargetRenderer::draw(DrawType dt, const unsigned state)
{
	Vector &campos = MainCamera::instance()->getCamera().getCurrentPos();

	// Sort all of the tanks
	std::vector<std::pair<float, Target *> > sortedTargets;	
	std::map<unsigned int, Target *> &targets = 
		ScorchedClient::instance()->getTargetContainer().getTargets();
	std::map<unsigned int, Target *>::iterator itor;
	for (itor = targets.begin();
		itor != targets.end();
		itor++)
	{
		Target *target = (*itor).second;

		bool add = false;
		if (target->getTargetType() == Target::eTank)
		{
			Tank *tank = (Tank *) target;
			if (tank->getState().getState() == TankState::sNormal)
			{
				// Only ever try to draw alive tanks
				add = true;
			}
		}
		else
		{
			add = true;
		}

		if (add)
		{
			float dist = approx_distance(
				target->getTargetPosition()[0] - campos[0],
				target->getTargetPosition()[1] - campos[1]);
			sortedTargets.push_back(std::pair<float, Target *>(dist, target));
		}
	}

	// Sort tanks
	std::sort(sortedTargets.begin(), sortedTargets.end(), lt_distance); 

	// Draw all of the tanks
	std::vector<std::pair<float, Target *> >::iterator sortedItor;
	for (sortedItor = sortedTargets.begin();
		sortedItor != sortedTargets.end();
		sortedItor++)
	{
		Target *target = (*sortedItor).second;

		// Check we have the tank model for each tank
		// If not load the model from the store
		TargetModelIdRenderer *model =
			target->getModel().getModelIdRenderer();
		if (!model)
		{
			if (target->getTargetType() == Target::eTank)
			{
				model = new TankModelRenderer((Tank*) target);
				target->getModel().setModelIdRenderer(model);
			}
			else
			{
				model = new TargetModelRenderer(target);
				target->getModel().setModelIdRenderer(model);
			}
		}
		
		// Check if we have made the particle
		// We may not have if there were not enough to create the 
		// tank in the first place
		if (!model->getMadeParticle())
		{
			// Pretent the tank is actually a particle, this is so
			// it gets rendered during the particle renderering phase
			// and using the correct z ordering
			Particle *particle = 
				ScorchedClient::instance()->getParticleEngine().
					getNextAliveParticle();
			if (particle)
			{
				particle->setParticle(
					1000.0f,  1.0f, 1.0f, //float life, float mass, float friction,
					Vector::nullVector, Vector::nullVector, //Vector &velocity, Vector &gravity,
					Vector::nullVector, Vector::nullVector, //Vector &color, Vector &colorCounter,
					Vector::nullVector, Vector::nullVector, //Vector &size, Vector &sizeCounter,
					1.0f, 0.0f, // float alpha, float alphaCounter,
					false, //bool additiveTexture,
					false); //bool windAffect);

				model->setMadeParticle(true);
				particle->life_ = 1000.0f;
				particle->renderer_ = TargetParticleRenderer::getInstance();
				particle->userData_ = new unsigned(target->getPlayerId());
			}
		}

		switch (dt)
		{
		case Type3D:
			model->draw();
			break;
		case Type2D:
			model->draw2d();
			break;
		}
	}
}

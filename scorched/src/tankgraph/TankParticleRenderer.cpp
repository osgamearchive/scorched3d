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

#include <tankgraph/TankParticleRenderer.h>
#include <tankgraph/TankModelRenderer.h>
#include <engine/Particle.h>
#include <tank/TankContainer.h>
#include <client/ScorchedClient.h>

TankParticleRenderer *TankParticleRenderer::getInstance()
{
	static TankParticleRenderer instance_;
	return &instance_;
}

void TankParticleRenderer::simulateParticle(Particle &particle, float time)
{
	Tank *tank =
		ScorchedClient::instance()->getTankContainer().getTankById(
			*((unsigned*) particle.userData_));
	if (tank)
	{
		// Tank and particle alive
		particle.life_ = 1000.0f; // Alive
		particle.position_ = tank->getPhysics().getTankPosition();
	}
	else
	{
		// Tank expired but particle has not
		particle.life_ = 0.0f; // Dead
	}
}

void TankParticleRenderer::renderParticle(Particle &particle)
{
	Tank *tank =
		ScorchedClient::instance()->getTankContainer().getTankById(
			*((unsigned*) particle.userData_));
	if (tank)
	{
		if (tank->getState().getState() == TankState::sNormal)
		{
			TankModelRenderer *model = (TankModelRenderer *) 
				tank->getModel().getModelIdRenderer();
			if (model)
			{
				model->drawSecond();
				glDepthMask(GL_FALSE);
			}
		}
	}
}

void TankParticleRenderer::recycleParticle(Particle &particle)
{
	Tank *tank =
		ScorchedClient::instance()->getTankContainer().getTankById(
			*((unsigned*) particle.userData_));
	if (tank)
	{
		TankModelRenderer *model = (TankModelRenderer *) 
			tank->getModel().getModelIdRenderer();
		if (model)
		{
			// Particle expired but tank has not
			model->setMadeParticle(false);
		}
	}
}

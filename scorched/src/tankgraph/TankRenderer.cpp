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


#include <tankgraph/TankRenderer.h>
#include <tankgraph/TankModelRenderer.h>
#include <tank/TankContainer.h>
#include <client/ClientState.h>

TankRenderer *TankRenderer::instance_ = 0;

TankRenderer *TankRenderer::instance()
{
	if (!instance_)
	{
		instance_ = new TankRenderer;
	}
	return instance_;
}

TankRenderer::TankRenderer()
{
}

TankRenderer::~TankRenderer()
{
}

void TankRenderer::Renderer2D::draw(const unsigned state)
{
	TankRenderer::instance()->draw(false, state);
}

void TankRenderer::Renderer3D::draw(const unsigned state)
{
	TankRenderer::instance()->tracerStore_.draw(state);
	TankRenderer::instance()->draw(true, state);
}

void TankRenderer::Renderer3D::simulate(const unsigned state, float simTime)
{
	// Simulate the HUD
	TankModelRendererHUD::simulate(simTime);

	// Draw all of the tanks
	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		TankModelRenderer *model = (TankModelRenderer *) 
			tank->getModel().getModelIdRenderer();
		if (model && tank->getState().getState() == TankState::sNormal)
		{
			model->simulate(simTime);
		}
	}
}

void TankRenderer::newGame()
{
	tracerStore_.newGame();

	// Make sure we don't draw any dead tanks
	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getState().getState() == TankState::sNormal)
		{
			tank->getState().setState(TankState::sDead);
		}
	}
}

void TankRenderer::draw(bool d, const unsigned state)
{
	Tank *currentTank =
		TankContainer::instance()->getCurrentTank();
	
	// Draw all of the tanks
	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		// Check we have the tank model for each tank
		// If not load the model from the store
		TankModelRenderer *model = (TankModelRenderer *) 
			tank->getModel().getModelIdRenderer();
		if (!model)
		{
			model = new TankModelRenderer(tank);
			tank->getModel().setModelIdRenderer(model);
		}

		// draw tanks
		bool current = false;
		if (state == ClientState::StateMain)
		{
			current = (tank == currentTank);
		}

		if (d) model->draw(current);
		else model->draw2d(current);
	}
}

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
#include <client/ScorchedClient.h>
#include <client/MainCamera.h>
#include <algorithm>

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
	TankRenderer::instance()->draw(TankRenderer::Type2D, state);
}

void TankRenderer::Renderer3D::draw(const unsigned state)
{
	TankRenderer::instance()->tracerStore_.draw(state);
	TankRenderer::instance()->draw(TankRenderer::Type3D, state);
}

void TankRenderer::Renderer3DSecond::draw(const unsigned state)
{
	TankRenderer::instance()->draw(TankRenderer::Type3DSecond, state);
}

void TankRenderer::Renderer3D::simulate(const unsigned state, float simTime)
{
	// Simulate the HUD
	TankModelRendererHUD::simulate(simTime);
	TankModelRendererAIM::simulate(simTime);

	// Simulate all of the tanks
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedClient::instance()->getTankContainer().getPlayingTanks();
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
}

static inline float approx_distance(float  dx, float dy)
{
   float approx = (dx * dx) + (dy * dy);
   return approx;
}

static inline bool lt_distance(const std::pair<float, Tank *> &o1, 
	const std::pair<float, Tank *> &o2) 
{ 
	return o1.first > o2.first;
}

void TankRenderer::draw(DrawType dt, const unsigned state)
{
	Tank *currentTank =
		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	Vector &campos = MainCamera::instance()->getCamera().getCurrentPos();

	// Sort all of the tanks
	std::vector<std::pair<float, Tank *> > sortedTanks;	
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedClient::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		// Only ever try to draw alive tanks
		if (tank->getState().getState() == TankState::sNormal)
		{
			float dist = approx_distance(
				tank->getPhysics().getTankPosition()[0] - campos[0],
				tank->getPhysics().getTankPosition()[1] - campos[1]);
			sortedTanks.push_back(std::pair<float, Tank *>(dist, tank));
		}
	}

	// Sort tanks
	std::sort(sortedTanks.begin(), sortedTanks.end(), lt_distance); 

	// Draw all of the tanks
	std::vector<std::pair<float, Tank *> >::iterator sortedItor;
	for (sortedItor = sortedTanks.begin();
		sortedItor != sortedTanks.end();
		sortedItor++)
	{
		Tank *tank = (*sortedItor).second;

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
		if (state == ClientState::StatePlaying)
		{
			current = (tank == currentTank);
		}
	
		switch (dt)
		{
		case Type3D:
			model->draw(current);
			break;
		case Type3DSecond:
			model->drawSecond(current);
			break;
		case Type2D:
			model->draw2d(current);
			break;
		}
	}
}

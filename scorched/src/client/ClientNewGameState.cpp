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

#include <client/ClientNewGameState.h>
#include <client/ScorchedClient.h>
#include <client/SpeedChange.h>
#include <client/MainBanner.h>
#include <client/ScorchedClient.h>
#include <client/ClientState.h>
#include <landscape/Landscape.h>
#include <tankgraph/TankRenderer.h>
#include <GLEXT/GLLenseFlare.h>
#include <dialogs/ProgressDialog.h>
#include <sprites/ExplosionTextures.h>
#include <engine/ActionController.h>
#include <engine/MainLoop.h>

ClientNewGameState *ClientNewGameState::instance_ = 0;

ClientNewGameState *ClientNewGameState::instance()
{
	if (!instance_)
	{
		instance_ = new ClientNewGameState;
	}
	return instance_;
}

ClientNewGameState::ClientNewGameState()
{
}

ClientNewGameState::~ClientNewGameState()
{
}

void ClientNewGameState::enterState(const unsigned state)
{
	// Calculate all the new landscape settings (graphics)
	Landscape::instance()->generate(ProgressDialog::instance());

	// Calculate other graphics that need to be built, only once
	static bool initOnce = false;
	if (!initOnce)
	{
		ExplosionTextures::instance()->createTextures(
			ProgressDialog::instance());
		GLLenseFlare::instance()->init(
			ProgressDialog::instance());
		initOnce = true;
	}
	TankRenderer::instance()->newGame();
	SpeedChange::instance()->resetSpeed();

	// Remove all actions (graphical objects) from the last round
	ScorchedClient::instance()->getActionController().clear();

	// Tell all tanks to update transient settings
	ScorchedClient::instance()->getTankContainer().clientNewGame();

	// As we have not returned to the main loop for ages the
	// timer will have a lot of time in it
	// Get rid of this time so we don't screw things up
	ScorchedClient::instance()->getMainLoop().getTimer().getTimeDifference();

	// Make sure the landscape has been optimized
	Landscape::instance()->recalculate(0, 0, 10000);

	// Stimulate into the next round state
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
}


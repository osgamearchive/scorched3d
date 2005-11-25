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

#include <client/ClientGameStateHandler.h>
#include <client/ScorchedClient.h>
#include <tank/TankContainer.h>
#include <engine/ActionController.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/Landscape.h>
#include <landscape/PatchGrid.h>
#include <coms/ComsGameStateMessage.h>
#include <common/OptionsTransient.h>
#include <common/Vector.h>

ClientGameStateHandler *ClientGameStateHandler::instance_ = 0;

ClientGameStateHandler *ClientGameStateHandler::instance()
{
	if (!instance_)
	{
	  instance_ = new ClientGameStateHandler();
	}

	return instance_;
}

ClientGameStateHandler::ClientGameStateHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsGameStateMessage",
		this);
}

ClientGameStateHandler::~ClientGameStateHandler()
{

}

bool ClientGameStateHandler::processMessage(unsigned int id,
		const char *messageType,
		NetBufferReader &reader)
{
	ComsGameStateMessage message;
	if (!message.readMessage(reader)) return false;

	// Set the physics for the next shot
	ScorchedClient::instance()->getActionController().getPhysics().generate();

	// Make sure no objects are around the tanks
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedClient::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getState().getState() == TankState::sNormal &&
			!tank->getState().getSpectator())
		{
			ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getObjects().
				removeObjects(
				ScorchedClient::instance()->getContext(),
					(unsigned int ) tank->getPhysics().getTankPosition()[0],
					(unsigned int ) tank->getPhysics().getTankPosition()[1],
					3,
					0);
		}
	}
	Landscape::instance()->getPatchGrid().recalculateTankVariance();

	return true;
}

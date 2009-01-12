////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2009
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

#include <client/ClientStartGameHandler.h>
#include <client/ClientState.h>
#include <client/ClientNewGameHandler.h>
#include <client/ScorchedClient.h>
#include <landscape/Landscape.h>
#include <landscapemap/LandscapeMaps.h>
#include <engine/ActionController.h>
#include <tank/TankContainer.h>
#include <tank/TankAccessories.h>
#include <tank/TankCamera.h>
#include <graph/MainCamera.h>
#include <graph/OptionsDisplay.h>
#include <common/OptionsTransient.h>
#include <common/OptionsScorched.h>
#include <common/Defines.h>
#include <sound/SoundUtils.h>
#include <coms/ComsStartGameMessage.h>
#include <weapons/Accessory.h>

ClientStartGameHandler *ClientStartGameHandler::instance_ = 0;

ClientStartGameHandler *ClientStartGameHandler::instance()
{
	if (!instance_)
	{
	  instance_ = new ClientStartGameHandler();
	}

	return instance_;
}

ClientStartGameHandler::ClientStartGameHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsStartGameMessage",
		this);
}

ClientStartGameHandler::~ClientStartGameHandler()
{

}

bool ClientStartGameHandler::processMessage(
	NetMessage &netMessage,
	const char *messageType,
	NetBufferReader &reader)
{
	ComsStartGameMessage message;
	if (!message.readMessage(reader)) return false;

	ScorchedClient::instance()->getTankContainer().setCurrentPlayerId(
		message.getCurrentPlayerId());
	Tank *current = ScorchedClient::instance()->getTankContainer().getCurrentTank();

	// Set the camera back to this players camera position
	if (OptionsDisplay::instance()->getStorePlayerCamera())
	{
		if (current)
		{
			MainCamera::instance()->getTarget().getCamera().setLookAt(current->getCamera().getCameraLookAt());
			Vector rotation = current->getCamera().getCameraRotation();
			MainCamera::instance()->getTarget().getCamera().movePosition(rotation[0], rotation[1], rotation[2]);
			MainCamera::instance()->getTarget().setCameraType((TargetCamera::CamType) current->getCamera().getCameraType());
		}
	}

	// Ensure that the landscape is set to the "proper" texture
	Landscape::instance()->restoreLandscapeTexture();

	// make sound to tell client a new game is commencing
	if (current->getDestinationId() == 
		ScorchedClient::instance()->getTankContainer().getCurrentDestinationId())
	{
		CACHE_SOUND(playSound, S3D::getDataFile("data/wav/misc/play.wav"));
		SoundUtils::playRelativeSound(VirtualSoundPriority::eText, playSound);
	}

	// Stimulate into the new game state
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
	ScorchedClient::instance()->getGameState().checkStimulate();
	if (message.getBuyWeapons())
	{
		ScorchedClient::instance()->getGameState().stimulate(
			ClientState::StimBuyWeapons);
	}
	else
	{
		ScorchedClient::instance()->getGameState().stimulate(
			ClientState::StimPlaying);

		// Set the current weapon, so any graphics are updated
		Tank *currentTank = 
			ScorchedClient::instance()->getTankContainer().getCurrentTank();
		if (currentTank)
		{
			Accessory *currentWeapon = currentTank->getAccessories().getWeapons().getCurrent();
			if (currentWeapon && 
				currentWeapon->getPositionSelect() != Accessory::ePositionSelectNone)
			{
				std::list<Accessory *> &entries =
					currentTank->getAccessories().getAllAccessoriesByGroup("weapon");
				if (!entries.empty()) currentWeapon = entries.front();
			}
			currentTank->getAccessories().getWeapons().setWeapon(currentWeapon);
		}
	}
	return true;
}
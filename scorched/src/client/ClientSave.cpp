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

#include <client/ClientSave.h>
#include <client/ScorchedClient.h>
#include <server/ScorchedServer.h>
#include <tankai/TankAIStore.h>
#include <coms/NetBuffer.h>
#include <coms/ComsAddPlayerMessage.h>
#include <coms/ComsMessageSender.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <stdio.h>

static NetBuffer saveBuffer;
static bool stateRestoredFromFile = false;

bool ClientSave::storeClient()
{
	NetBuffer &buffer = saveBuffer;
	buffer.reset();

	// Add Version
	buffer.addToBuffer(ScorchedProtocolVersion);
	
	// GameState (removing the spectator tank)
	ScorchedServer::instance()->getOptionsGame().setNoMaxPlayers(
		ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers()-1);
	ScorchedServer::instance()->getOptionsGame().writeToBuffer(buffer);
	ScorchedServer::instance()->getOptionsGame().setNoMaxPlayers(
		ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers()+1);
	
	// Transient State
	if (!ScorchedServer::instance()->getOptionsTransient().writeToBuffer(
		buffer)) return false;
	
	// Players
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().
			getPlayingTanks();
	buffer.addToBuffer((int) tanks.size() - 1);
 	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		// Add each tank
		Tank *tank = (*itor).second;

		// Remove the spectator tank
		if (!tank->getState().getSpectator())
		{
			// Add all other tanks
			buffer.addToBuffer(tank->getPlayerId());
			if (!tank->writeMessage(buffer)) return false;

			TankAI *tankAI = tank->getTankAI();
			if (tankAI) 
			{
				buffer.addToBuffer(tankAI->getName());
			}
			else
			{
				buffer.addToBuffer("Human");
			}
		}
	}

	return true;
}

bool ClientSave::saveClient(const char *fileName)
{
	FILE *file = fopen(fileName, "wb");
	if (!file) return false;

	int size = fwrite(saveBuffer.getBuffer(),	
		sizeof(char),
		saveBuffer.getBufferUsed(),
		file);
	fclose(file);
	return (size == saveBuffer.getBufferUsed());
}

bool ClientSave::restoreClient(bool loadGameState, bool loadPlayers)
{
	NetBufferReader reader(saveBuffer);

	// Add Version
	std::string version;
	if (!reader.getFromBuffer(version)) return false;
	if (0 != strcmp(version.c_str(), ScorchedProtocolVersion))
	{
		Logger::log(0, "ERROR: Saved file version does not match game version");
		return false;
	}
	
	// GameState
	if (loadGameState)
	{
		if (!ScorchedServer::instance()->getOptionsGame().readFromBuffer(
			reader)) return false;
	}
	else
	{
		OptionsGame optionsGame;
		optionsGame.readFromBuffer(reader);
	}
	
	// Transient State
	if (!ScorchedServer::instance()->getOptionsTransient().readFromBuffer(
		reader)) return false;
	
	if (!loadPlayers) return true;

	bool specTanks = (ScorchedServer::instance()->getTankContainer().getNoOfTanks() -
		ScorchedServer::instance()->getTankContainer().getNoOfNonSpectatorTanks() > 1);

	// Players
	int noTanks = 0;
	if (!reader.getFromBuffer(noTanks)) return false;
	for (int i=0; i<noTanks; i++)
	{
		unsigned int playerId = 0;
		if (!reader.getFromBuffer(playerId)) return false;
	
		Tank *tank = 
			ScorchedServer::instance()->getTankContainer().getTankById(playerId);
		if (!specTanks)
		{
			if (tank)
			{
				if (!tank->readMessage(reader)) return false;
				std::string tankAIStr;
				if (!reader.getFromBuffer(tankAIStr)) return false;
			}
		}
		else
		{
			Vector color;
			TankModelId model("");
			Tank *tank = new Tank(
				ScorchedServer::instance()->getContext(),
				playerId, // PlayerId
				0, // DestinationId
				"", // Name
				color,
				model);
			if (!tank->readMessage(reader)) return false;
		
			std::string tankAIStr;
			if (!reader.getFromBuffer(tankAIStr)) return false;
		
			if (tank->getState().getSpectator())
			{
				delete tank;
			}
			else
			{
				ComsAddPlayerMessage message(
					tank->getPlayerId(),
					tank->getName(),
					tank->getColor(),
					tank->getModel().getModelName(),
					ScorchedClient::instance()->getTankContainer().getCurrentDestinationId(),
					tank->getTeam(),
					tankAIStr.c_str());
			        ComsMessageSender::sendToServer(message);
			}
		}
	}

	return true;
}

bool ClientSave::stateRestored()
{
	return stateRestoredFromFile;
}

void ClientSave::setStateNotRestored()
{
	stateRestoredFromFile = false;
}

bool ClientSave::loadClient(const char *fileName)
{
	FILE *file = fopen(fileName, "rb");
	if (!file) 
	{
		Logger::log(0, "ERROR: File \"%s\" cannot be found.",
			fileName);
		return false;
	}

	saveBuffer.reset();
	char buffer[2];
	while (0 != fread(buffer, 1, 1, file))
	{
		saveBuffer.addDataToBuffer(buffer, 1);
	}

	fclose(file);
	stateRestoredFromFile = true;
	return true;
}

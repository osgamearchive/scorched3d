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
#include <server/ScorchedServer.h>
#include <tankai/TankAIStore.h>
#include <coms/NetBuffer.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <stdio.h>

static NetBuffer saveBuffer;
static bool stateRestoredFromFile = false;

bool storeClient()
{
	NetBuffer &buffer = saveBuffer;
	buffer.reset();

	// Add Version
	buffer.addToBuffer(ScorchedProtocolVersion);
	
	// GameState
	if (!ScorchedServer::instance()->getOptionsGame().writeToBuffer(
		buffer)) return false;
	
	// Transient State
	if (!ScorchedServer::instance()->getOptionsTransient().writeToBuffer(
		buffer)) return false;
	
	// Players
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().
			getPlayingTanks();
	buffer.addToBuffer((int) tanks.size());
 	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		// Add each tank
		Tank *tank = (*itor).second;
		buffer.addToBuffer(tank->getPlayerId());
		if (!tank->writeMessage(buffer)) return false;

		TankAI *tankAI = tank->getTankAI();
		if (tankAI) 
		{
			buffer.addToBuffer(tankAI->getName());
		}
		else
		{
			buffer.addToBuffer("");
		}
	}

	return true;
}

bool saveClient(const char *fileName)
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

bool restoreClient(bool stateOnly)
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
	if (!ScorchedServer::instance()->getOptionsGame().readFromBuffer(
		reader)) return false;
	
	// Transient State
	if (!ScorchedServer::instance()->getOptionsTransient().readFromBuffer(
		reader)) return false;
	
	if (stateOnly) return true;
	
	// Players
	int noTanks = 0;
	if (!reader.getFromBuffer(noTanks)) return false;
	for (int i=0; i<noTanks; i++)
	{
		unsigned int playerId = 0;
		if (!reader.getFromBuffer(playerId)) return false;
	
		Tank *tank = 
			ScorchedServer::instance()->getTankContainer().getTankById(
				playerId);
		if (tank)
		{
			if (!tank->readMessage(reader)) return false;
			std::string tankAIStr;
			if (!reader.getFromBuffer(tankAIStr)) return false;
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
		
			if (tankAIStr.c_str()[0])
			{
				TankAI *tankai = TankAIStore::instance()->
					getAIByName(tankAIStr.c_str());
				if (!tankai)
				{
					Logger::log(0, "Unabled to find saved tank ai \"%s\"",
						tankAIStr.c_str());
					return false;
				}
				tank->setTankAI(tankai);
			}
		
			if (tank->getState().getSpectator())
			{
				delete tank;
			}
			else
			{
				ScorchedServer::instance()->getTankContainer().addTank(tank);
			}
		}
	}

	return true;
}

bool stateRestored()
{
	return stateRestoredFromFile;
}

bool loadClient(const char *fileName)
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


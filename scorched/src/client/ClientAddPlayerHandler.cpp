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


#include <client/ClientAddPlayerHandler.h>
#include <client/ClientConnectionAcceptHandler.h>
#include <client/ClientState.h>
#include <tank/TankContainer.h>
#include <tankai/TankAIHuman.h>
#include <common/Logger.h>
#include <coms/ComsAddPlayerMessage.h>

ClientAddPlayerHandler *ClientAddPlayerHandler::instance_ = 0;

ClientAddPlayerHandler *ClientAddPlayerHandler::instance()
{
	if (!instance_)
	{
	  instance_ = new ClientAddPlayerHandler();
	}

	return instance_;
}

ClientAddPlayerHandler::ClientAddPlayerHandler()
{
	ComsMessageHandler::instance()->addHandler(
		"ComsAddPlayerMessage",
		this);
}

ClientAddPlayerHandler::~ClientAddPlayerHandler()
{

}

bool ClientAddPlayerHandler::processMessage(NetPlayerID &id,
		const char *messageType,
		NetBufferReader &reader)
{
	ComsAddPlayerMessage message;
	if (!message.readMessage(reader)) return false;

	// Create the new tank and add it to the tank container
	// Collections
	TankModelId modelId(message.getModelName());
	Tank *tank = new Tank(
		message.getPlayerId(),
		message.getPlayerName(),
		message.getPlayerColor(),
		modelId);
	if (message.getPlayerId() == 
		TankContainer::instance()->getCurrentPlayerId())
	{
		TankAI *ai = new TankAIHuman(tank);
		tank->setTankAI(ai);
	}
	TankContainer::instance()->addTank(tank);

	// Tell this computer that a new tank has connected
	Logger::log(tank,
			"New player connected \"%s\" %s",
			message.getPlayerName(),
			(tank->getPlayerId() == TankContainer::instance()->getCurrentPlayerId()?
			"<- Local":""));

	return true;
}
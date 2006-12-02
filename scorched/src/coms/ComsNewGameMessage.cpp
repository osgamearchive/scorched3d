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

#include <coms/ComsNewGameMessage.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <weapons/AccessoryStore.h>
#include <client/ScorchedClient.h>
#include <server/ScorchedServer.h>

ComsNewGameMessage::ComsNewGameMessage() :
	ComsMessage("ComsNewGameMessage"),
	gameStateEnclosed_(false),
	playerState_(false)
{

}

ComsNewGameMessage::~ComsNewGameMessage()
{

}

void ComsNewGameMessage::addGameState()
{
	gameStateEnclosed_ = true;
}

bool ComsNewGameMessage::writeMessage(NetBuffer &buffer, unsigned int destinationId)
{
	buffer.addToBuffer(gameStateEnclosed_);
	if (gameStateEnclosed_)
	{
		if (!ScorchedServer::instance()->
			getOptionsGame().writeToBuffer(buffer, false, false)) return false;
		if (!ScorchedServer::instance()->
			getOptionsTransient().writeToBuffer(buffer)) return false;
	}
	if (!levelMessage_.writeMessage(buffer, destinationId)) return false;

	// Both of these are read later as they must be processed after the level
	// has been built
	if (!ScorchedServer::instance()->getAccessoryStore().
		writeEconomyToBuffer(buffer)) return false;
	if (!playerState_.writeMessage(buffer, destinationId)) return false;

	return true;
}

bool ComsNewGameMessage::readMessage(NetBufferReader &reader)
{
#ifndef S3D_SERVER
	if (!reader.getFromBuffer(gameStateEnclosed_)) return false;
	if (gameStateEnclosed_)
	{
		if (!ScorchedClient::instance()->
			getOptionsGame().readFromBuffer(reader, false, false)) return false;
		if (!ScorchedClient::instance()->
			getOptionsTransient().readFromBuffer(reader)) return false;
	}
	if (!levelMessage_.readMessage(reader)) return false;
#endif

	return true;
}

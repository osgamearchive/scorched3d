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

#include <client/ScorchedClient.h>
#include <server/ScorchedServer.h>
#include <coms/ComsStartGameMessage.h>
#include <common/OptionsTransient.h>

ComsStartGameMessage::ComsStartGameMessage(unsigned int currentPlayerId,
										   bool buyWeapons) :
	ComsMessage("ComsStartGameMessage"),
	currentPlayerId_(currentPlayerId),
	buyWeapons_(buyWeapons)
{
}

ComsStartGameMessage::~ComsStartGameMessage()
{
}

bool ComsStartGameMessage::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(currentPlayerId_);
	buffer.addToBuffer(buyWeapons_);
	if (!stateMessage_.writeMessage(buffer)) return false;
	if (!ScorchedServer::instance()->getOptionsTransient().writeToBuffer(buffer)) return false;
	return true;
}

bool ComsStartGameMessage::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(currentPlayerId_)) return false;
	if (!reader.getFromBuffer(buyWeapons_)) return false;
	if (!stateMessage_.readMessage(reader)) return false;
	if (!ScorchedClient::instance()->getOptionsTransient().readFromBuffer(reader)) return false;
	return true;
}

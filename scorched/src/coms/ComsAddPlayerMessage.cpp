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

#include <coms/ComsAddPlayerMessage.h>

ComsAddPlayerMessage::ComsAddPlayerMessage(
		unsigned int playerId,
		const char *playerName,
		Vector playerColor,
		const char *modelName,
		unsigned int destinationId,
		const char *playerType) :
	ComsMessage("ComsAddPlayerMessage"),
	playerId_(playerId),
	playerName_(playerName),
	playerType_(playerType),
	playerColor_(playerColor),
	modelName_(modelName),
	destinationId_(destinationId)
{

}

ComsAddPlayerMessage::~ComsAddPlayerMessage()
{

}

bool ComsAddPlayerMessage::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(playerName_.c_str());
	buffer.addToBuffer(playerType_.c_str());
	buffer.addToBuffer(modelName_.c_str());
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(destinationId_);
	buffer.addToBuffer(playerColor_[0]);
	buffer.addToBuffer(playerColor_[1]);
	buffer.addToBuffer(playerColor_[2]);
	return true;
}

bool ComsAddPlayerMessage::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerName_)) return false;
	if (!reader.getFromBuffer(playerType_)) return false;
	if (!reader.getFromBuffer(modelName_)) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(destinationId_)) return false;
	if (!reader.getFromBuffer(playerColor_[0])) return false;
	if (!reader.getFromBuffer(playerColor_[1])) return false;
	if (!reader.getFromBuffer(playerColor_[2])) return false;
	return true;
}

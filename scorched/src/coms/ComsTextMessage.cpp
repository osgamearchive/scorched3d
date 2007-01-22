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


#include <coms/ComsTextMessage.h>

ComsTextMessage::ComsTextMessage(const char *text,
								 unsigned int playerId,
								 bool showAsMessage,
								 bool teamOnlyMessage,
								 unsigned int infoLen,
								 unsigned int toPlayerId) :
	ComsMessage("ComsTextMessage"),
	playerId_(playerId), infoLen_(infoLen),
	text_(text), toPlayerId_(toPlayerId),
	showAsMessage_(showAsMessage), 
	teamOnlyMessage_(teamOnlyMessage)
{
}

ComsTextMessage::~ComsTextMessage()
{
}

bool ComsTextMessage::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(text_);
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(showAsMessage_);
	buffer.addToBuffer(teamOnlyMessage_);
	buffer.addToBuffer(infoLen_);
	buffer.addToBuffer(toPlayerId_);
	return true;
}

bool ComsTextMessage::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(text_)) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(showAsMessage_)) return false;
	if (!reader.getFromBuffer(teamOnlyMessage_)) return false;
	reader.getFromBuffer(infoLen_); // Dont check so it is more backward compatable
	reader.getFromBuffer(toPlayerId_); // Dont check so it is more backward compatable
	return true;
}

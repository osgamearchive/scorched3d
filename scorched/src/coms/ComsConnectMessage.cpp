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

#include <coms/ComsConnectMessage.h>

ComsConnectMessage::ComsConnectMessage(const char *version,
		const char *protocolVersion,
		const char *password,
		const char *unqiueId)
	: ComsMessage("ComsConnectMessage"),
	  version_(version),
	  protocolVersion_(protocolVersion),
	  password_(password),
	  uniqueId_(unqiueId)
{

}

ComsConnectMessage::~ComsConnectMessage()
{

}

bool ComsConnectMessage::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(version_);
	buffer.addToBuffer(protocolVersion_);
	buffer.addToBuffer(password_);
	buffer.addToBuffer(uniqueId_);
	buffer.addToBuffer((unsigned int) players_.size());
	std::list<PlayerEntry>::iterator itor;
	for (itor = players_.begin();
		itor != players_.end();
		itor++)
	{
		PlayerEntry &entry = (*itor);
		buffer.addToBuffer(entry.name);
		buffer.addToBuffer(entry.model);
	}

	return true;
}

bool ComsConnectMessage::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(version_)) return false;
	if (!reader.getFromBuffer(protocolVersion_)) return false;
	if (!reader.getFromBuffer(password_)) return false;
	if (!reader.getFromBuffer(uniqueId_)) return false;
	unsigned int noPlayers = 0;
	if (!reader.getFromBuffer(noPlayers)) return false;
	for (unsigned int i=0; i<noPlayers; i++)
	{
		PlayerEntry entry;
		if (!reader.getFromBuffer(entry.name)) return false;
		if (!reader.getFromBuffer(entry.model)) return false;
		players_.push_back(entry);
	}

	return true;
}

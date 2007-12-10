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
#include <common/Defines.h>

ComsConnectMessage::ComsConnectMessage()
	: ComsMessage("ComsConnectMessage")
{

}

ComsConnectMessage::~ComsConnectMessage()
{

}

bool ComsConnectMessage::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer((unsigned int) values_.size());
	std::map<std::string, std::string>::iterator itor;
	for (itor = values_.begin();
		itor != values_.end();
		itor++)
	{
		buffer.addToBuffer((*itor).first.c_str());
		buffer.addToBuffer((*itor).second.c_str());
	}

	return true;
}

bool ComsConnectMessage::readMessage(NetBufferReader &reader)
{
	unsigned int noV = 0;
	values_.clear();
	if (!reader.getFromBuffer(noV)) return false;
	for (unsigned int i=0; i<noV; i++)
	{
		std::string name, value;

		if (!reader.getFromBuffer(name)) return false;
		if (!reader.getFromBuffer(value)) return false;

		// Validate the user strings
		if (strlen(value.c_str()) > 50)
		{
			((char *)value.c_str())[50] = '\0';
		}

		values_[name] = value;
	}

	return true;
}

const char *ComsConnectMessage::getValue(const char *name)
{
	std::map<std::string, std::string>::iterator itor =
		values_.find(name);
	if (itor == values_.end()) return "";

	return (*itor).second.c_str();
}

void ComsConnectMessage::setValue(const char *name, const char *value)
{
	values_[name] = value;
}


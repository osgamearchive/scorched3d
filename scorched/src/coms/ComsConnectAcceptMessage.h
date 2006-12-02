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

#ifndef _comsConnectAcceptMessage_h
#define _comsConnectAcceptMessage_h

#include <coms/ComsMessage.h>

class ComsConnectAcceptMessage : public ComsMessage
{
public:
	ComsConnectAcceptMessage(unsigned int destinationId = 0,
		const char *serverName = "",
		const char *publishAddress = "",
		const char *uniqueId = "");
	virtual ~ComsConnectAcceptMessage();

	unsigned int getDestinationId() { return destinationId_; }
	const char *getServerName() { return serverName_.c_str(); }
	const char *getPublishAddress() { return publishAddress_.c_str(); }
	const char *getUniqueId() { return uniqueId_.c_str(); }
	NetBuffer &getServerPng() { return serverPng_; }

	// Inherited from ComsMessage
    virtual bool writeMessage(NetBuffer &buffer, unsigned int destinationId);
    virtual bool readMessage(NetBufferReader &reader);

protected:
	unsigned int destinationId_;
	std::string serverName_;
	std::string publishAddress_;
	std::string uniqueId_;
	NetBuffer serverPng_;

private:
	ComsConnectAcceptMessage(const ComsConnectAcceptMessage &);
	const ComsConnectAcceptMessage & operator=(const ComsConnectAcceptMessage &);

};

#endif // _comsConnectAcceptMessage_h


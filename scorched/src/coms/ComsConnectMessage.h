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

#ifndef _comsConnectMessage_h
#define _comsConnectMessage_h

// The very first message sent from the client to the server
// requesting a connection.
// NOTE: Do not change the serialization format of this
// message as it will cause problems for earlier versioned
// servers. Ooops i have had to change it, oh well

#include <coms/ComsMessage.h>
#include <list>

class ComsConnectMessage : public ComsMessage
{
public:
	struct PlayerEntry
	{
		PlayerEntry() : spectator(false) {}
		std::string name;
		std::string model;
		bool spectator;
	};

	ComsConnectMessage(const char *version = "",
		const char *protocolVersion = "",
		const char *password = "",
		const char *unqiueId = "");
	virtual ~ComsConnectMessage();

	const char *getVersion() { return version_.c_str(); }
	const char *getProtocolVersion() { return protocolVersion_.c_str(); }
	const char *getPassword() { return password_.c_str(); }
	const char *getUniqueId() { return uniqueId_.c_str(); }
	std::list<PlayerEntry> &getPlayers() { return players_; }

	void addPlayers(std::list<PlayerEntry> &players) { players_.swap(players); }

	// Inherited from ComsMessage
    virtual bool writeMessage(NetBuffer &buffer);
    virtual bool readMessage(NetBufferReader &reader);

protected:
	std::string version_;
	std::string protocolVersion_;
	std::string password_;
	std::string uniqueId_;
	std::list<PlayerEntry> players_;

private:
	ComsConnectMessage(const ComsConnectMessage &);
	const ComsConnectMessage & operator=(const ComsConnectMessage &);

};

#endif // _comsConnectMessage_h


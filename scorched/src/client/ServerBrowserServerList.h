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

#if !defined(__INCLUDE_ServerBrowserServerListh_INCLUDE__)
#define __INCLUDE_ServerBrowserServerListh_INCLUDE__

#include <coms/NetServer.h>
#include <map>
#include <string>
#include <vector>

class ServerBrowserEntry
{
public:
	ServerBrowserEntry();
	virtual ~ServerBrowserEntry();

	void addAttribute(const char *name, const char *value);
	const char *getAttribute(const char *attrName);

	int retries_;
	int sentTime_;

protected:
	std::map<std::string, std::string> attributes_;
};

class ServerBrowserServerList : public NetMessageHandlerI
{
public:
	ServerBrowserServerList();
	virtual ~ServerBrowserServerList();

	// Fetch the new list from the server
	bool fetchServerList();
	unsigned int &getRefreshId() { return refreshId_; }

	// Accessors onto the list
	int getNoEntries();
	const char *getEntryValue(int pos, const char *name);
	ServerBrowserEntry &getEntry(int pos);

	// Inherited from NetMessageHandlerI
	virtual void processMessage(NetMessage &message);

protected:
	bool complete_;
	unsigned int refreshId_;
	SDL_mutex *vectorMutex_;
	NetServer netServer_;
	NetBuffer sendNetBuffer_;
	std::vector<ServerBrowserEntry> servers_;
};

#endif

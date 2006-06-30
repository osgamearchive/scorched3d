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

#include <client/ServerBrowserServerList.h>

ServerBrowserEntry::ServerBrowserEntry()
{
}

ServerBrowserEntry::~ServerBrowserEntry()
{
}

void ServerBrowserEntry::addAttribute(const char *name, const char *value)
{
	attributes_[name] = value;
}

const char *ServerBrowserEntry::getAttribute(const char *attrName)
{
	static const char *empty = "";

	std::map<std::string, std::string>::iterator itor = 
		attributes_.find(attrName);
	if (itor == attributes_.end()) return empty;
	
	return (*itor).second.c_str();
}

ServerBrowserServerList::ServerBrowserServerList()
{
	vectorMutex_ = SDL_CreateMutex();
}

ServerBrowserServerList::~ServerBrowserServerList()
{
	SDL_DestroyMutex(vectorMutex_);
}

const char *ServerBrowserServerList::getEntryValue(int pos, const char *name)
{
	SDL_LockMutex(vectorMutex_);
	DIALOG_ASSERT(pos >=0 && pos < (int) servers_.size());
	ServerBrowserEntry &entry = servers_[pos];
	static char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s", entry.getAttribute(name));
	SDL_UnlockMutex(vectorMutex_);

	return buffer;
}

void ServerBrowserServerList::addEntryValue(int pos, const char *name, const char *value)
{
	SDL_LockMutex(vectorMutex_);
	DIALOG_ASSERT(pos >=0 && pos < (int) servers_.size());
	ServerBrowserEntry &entry = servers_[pos];
	entry.addAttribute(name, value);
	SDL_UnlockMutex(vectorMutex_);
}

int ServerBrowserServerList::getNoEntries()
{
	SDL_LockMutex(vectorMutex_);
	int size = (int) servers_.size();
	SDL_UnlockMutex(vectorMutex_);

	return size;
}

void ServerBrowserServerList::addEntry(ServerBrowserEntry &newEntry)
{
	SDL_LockMutex(vectorMutex_);
	servers_.push_back(newEntry);
	SDL_UnlockMutex(vectorMutex_);
}

void ServerBrowserServerList::clear()
{
	SDL_LockMutex(vectorMutex_);
	servers_.clear();
	SDL_UnlockMutex(vectorMutex_);
}


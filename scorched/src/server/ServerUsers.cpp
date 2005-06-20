////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <server/ServerUsers.h>
#include <server/ScorchedServer.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <XML/XMLFile.h>
#include <wx/filefn.h>

ServerUsers::ServerUsers() : lastReadTime_(0)
{
}

ServerUsers::~ServerUsers()
{
}

ServerUsers::UserEntry *ServerUsers::getUserByName(const char *name)
{
	load();

	std::list<UserEntry>::iterator itor;
	for (itor = entries_.begin();
		itor != entries_.end();
		itor++)
	{
		UserEntry &entry = *itor;
		if (0 == strcmp(entry.name.c_str(), name))
		{
			return &entry;
		}
	}
	return 0;
}

ServerUsers::UserEntry *ServerUsers::getUserById(const char *uniqueId)
{
	load();

	std::list<UserEntry>::iterator itor;
	for (itor = entries_.begin();
		itor != entries_.end();
		itor++)
	{
		UserEntry &entry = *itor;
		if (0 == strcmp(entry.uniqueid.c_str(), uniqueId))
		{
			return &entry;
		}
	}
	return 0;
}

bool ServerUsers::load()
{
	const char *filename = 
		getSettingsFile("preferedplayers-%i.xml", 
			ScorchedServer::instance()->getOptionsGame().getPortNo());
	if (!::wxFileExists(filename)) return true;

	time_t fileTime = ::wxFileModificationTime(filename);
	if (fileTime == lastReadTime_) return true;

	XMLFile file;
	if (!file.readFile(filename))
	{
		Logger::log("Failed to parse user file \"%s\"\n%s", 
			filename, file.getParserError());
		return false;
	}

	Logger::log("Refreshing user list %s", filename);
	lastReadTime_ = fileTime;
	entries_.clear();
	if (!file.getRootNode()) return true; // Empty File

	std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = file.getRootNode()->getChildren();
	for (childrenItor = children.begin();
		 childrenItor != children.end();
		childrenItor++)
	{
		XMLNode *currentNode = (*childrenItor);

		UserEntry entry;
		if (!currentNode->getNamedChild("name", entry.name)) return false;
		if (!currentNode->getNamedChild("uniqueid", entry.uniqueid)) return false;
		if (!currentNode->failChildren()) return false;
		entries_.push_back(entry);
	}
	return true;
}

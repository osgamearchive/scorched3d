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

#include <server/ServerBanned.h>
#include <server/ScorchedServer.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>
#include <XML/XMLFile.h>

ServerBanned *ServerBanned::instance_ = 0;

ServerBanned *ServerBanned::instance()
{
	if (!instance_)
	{
		instance_ = new ServerBanned();
	}

	return instance_;
}

ServerBanned::ServerBanned()
{
}

ServerBanned::~ServerBanned()
{
}

bool ServerBanned::load()
{
	bannedIps_.clear();
	XMLFile file;
	const char *filename = 
		getSettingsFile("banned-%i.xml", 
			ScorchedServer::instance()->getOptionsGame().getPortNo());
	if (!file.readFile(filename))
	{
		dialogMessage("ServerBanned", 
					  "Failed to parse \"%s\"\n%s", 
					  filename,
					  file.getParserError());
		return false;
	}
	if (!file.getRootNode()) return true; // Empty File

    std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = file.getRootNode()->getChildren();
    for (childrenItor = children.begin();
		 childrenItor != children.end();
		 childrenItor++)
    {
        XMLNode *currentNode = (*childrenItor);
		unsigned int ip = atol(currentNode->getContent());
		bannedIps_.insert(ip);
	}
	return true;
}

bool ServerBanned::isBanned(unsigned int ip)
{
	std::set<unsigned int>::iterator findItor =
		bannedIps_.find(ip);
	return (findItor != bannedIps_.end());
}

void ServerBanned::addBanned(unsigned int ip)
{
	bannedIps_.insert(ip);
	save();
}

bool ServerBanned::save()
{
	const char *filename = 
		getSettingsFile("banned-%i.xml", 
			ScorchedServer::instance()->getOptionsGame().getPortNo());

	XMLNode node("bannednodes");
	std::set<unsigned int>::iterator itor;
	for (itor = bannedIps_.begin();
		itor != bannedIps_.end();
		itor++)
	{
		unsigned int ip = *itor;
		char address[4];
		memcpy(address, &ip, sizeof(address));

		// Add comment
		node.addChild(new XMLNode("", 
			formatString("%i.%i.%i.%i",
			address[0], address[1], address[2], address[3]), 
			XMLNode::XMLCommentType));

		// Add ip address
		XMLNode *optionNode = 
			new XMLNode("ipaddress", formatString("%i", ip));
		node.addChild(optionNode);
	}
	return node.writeToFile(filename);
}

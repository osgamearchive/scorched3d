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
#include <coms/NetInterface.h>
#include <XML/XMLFile.h>
#include <limits.h>

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
		XMLNode *maskNode = 0, *nameNode = 0, *timeNode = 0, *typeNode = 0;

		// Read the mask
		unsigned int m = UINT_MAX;
		if (currentNode->getNamedParameter("mask", maskNode, false))
		{
			unsigned int mask[4];
			if (sscanf(maskNode->getContent(), "%u.%u.%u.%u", 
				&mask[3], &mask[2], &mask[1], &mask[0]) != 4)
			{
				dialogMessage("ServerBanned",
					"Failed to parse mask %s",
					maskNode->getContent());
				return false;
			}
			m = mask[3] << 24 | mask[2] << 16 | mask[1] << 8 | mask[0];
		}

		// Read name
		std::string name;
		if (currentNode->getNamedParameter("name", nameNode, false))
		{
			name = nameNode->getContent();
		}

		// Time
		time_t bantime = 0;
		if (currentNode->getNamedParameter("time", timeNode, false))
		{
			sscanf(timeNode->getContent(), "%u", &bantime);
		}

		// Type
		BannedType type = Banned;
		if (currentNode->getNamedParameter("type", typeNode, false))
		{
			if (0 == strcmp("banned", typeNode->getContent())) type = Banned;
			else if (0 == strcmp("muted", typeNode->getContent())) type = Muted;
			else
			{
				dialogMessage("ServerBanned", 
					"Failed to parse banned type %s",
					typeNode->getContent());
				return false;
			}
		}

		// Read the ip address
		unsigned int address[4];
		if (sscanf(currentNode->getContent(), "%u.%u.%u.%u", 
			&address[3], &address[2], &address[1], &address[0]) != 4)
		{
			dialogMessage("ServerBanned", 
				"Failed to parse ip address %s", 
				currentNode->getContent());
			return false;
		}

		unsigned int ip = 0;
		ip = address[3] << 24 | address[2] << 16 | address[1] << 8 | address[0];

		// Add the new entry
		addBannedEntry(ip, m, name.c_str(), bantime, type);
	}
	return true;
}

ServerBanned::BannedType ServerBanned::getBanned(unsigned int ip)
{
	std::list<BannedRange>::iterator itor;
	for (itor = bannedIps_.begin();
		itor != bannedIps_.end();
		itor++)
	{
		BannedRange &range = *itor;
		unsigned int newip = range.mask & ip;
		std::map<unsigned int, BannedEntry>::iterator findItor =
			range.ips.find(newip);
		if (findItor != range.ips.end())
		{
			BannedEntry &entry = (*findItor).second;
			return entry.type;
		}
	}
	return NotBanned;
}

void ServerBanned::addBanned(unsigned int ip, const char *name, BannedType type)
{
	unsigned int t = time(0);
	addBannedEntry(ip, UINT_MAX, name, t, type);
	save();
}

void ServerBanned::addBannedEntry(unsigned int ip, unsigned int mask,
	const char *name, unsigned int bantime, BannedType type)
{
	unsigned int newip = mask & ip;
	BannedEntry newEntry;
	newEntry.name = name;
	newEntry.bantime = bantime;
	newEntry.type = type;

	bool found = false;
	std::list<BannedRange>::iterator itor;
	for (itor = bannedIps_.begin();
		itor != bannedIps_.end();
		itor++)
	{
		BannedRange &range = *itor;
		if (range.mask == mask)
		{
			found = true;
			range.ips[newip] = newEntry;
		}
	}
	if (!found)
	{
		BannedRange range;
		range.ips[newip] = newEntry;
		range.mask = mask;
		bannedIps_.push_back(range);
	}
}

const char *ServerBanned::getBannedTypeStr(BannedType type)
{
	const char *str = "banned";
	if (type == Muted)
	{
		str = "muted";
	}
	else if (type == Banned)
	{
		str = "banned";
	}
	else if (type == NotBanned)
	{
		str = "notbanned";
	}
	return str;
}

bool ServerBanned::save()
{
	const char *filename = 
		getSettingsFile("banned-%i.xml", 
			ScorchedServer::instance()->getOptionsGame().getPortNo());

	XMLNode node("bannednodes");
	std::list<BannedRange>::iterator itor;
	for (itor = bannedIps_.begin();
		itor != bannedIps_.end();
		itor++)
	{
		BannedRange &range = *itor;
		unsigned int m = range.mask;
		std::map<unsigned int, BannedEntry>::iterator ipitor;
		for (ipitor = range.ips.begin();
			ipitor != range.ips.end();
			ipitor++)
		{
			// Add ip address
			unsigned int ip = (*ipitor).first;
			BannedEntry &entry = (*ipitor).second;
			XMLNode *optionNode = 
				new XMLNode("ipaddress", NetInterface::getIpName(ip));

			// Add the mask
			if (m != UINT_MAX)
			{
				optionNode->addParameter(new XMLNode("mask", NetInterface::getIpName(m),
						XMLNode::XMLParameterType));
			}
			optionNode->addParameter(new XMLNode("name", 
					entry.name.c_str(),
					XMLNode::XMLParameterType));
			optionNode->addParameter(new XMLNode("time", 
					formatString("%u", entry.bantime),
					XMLNode::XMLParameterType));
			optionNode->addParameter(new XMLNode("type", 
					getBannedTypeStr(entry.type),
					XMLNode::XMLParameterType));

			// Add to file
			node.addChild(optionNode);
		}
	}
	return node.writeToFile(filename);
}


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
		XMLNode *maskNode = 0;

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
		addBannedEntry(ip, m);
	}
	return true;
}

bool ServerBanned::isBanned(unsigned int ip)
{
	std::list<BannedRange>::iterator itor;
	for (itor = bannedIps_.begin();
		itor != bannedIps_.end();
		itor++)
	{
		BannedRange &range = *itor;
		unsigned int newip = range.mask & ip;
		std::set<unsigned int>::iterator findItor =
			range.ips.find(newip);
		if (findItor != range.ips.end()) return true;
	}
	return false;
}

void ServerBanned::addBanned(unsigned int ip)
{
	addBannedEntry(ip, UINT_MAX);
	save();
}

void ServerBanned::addBannedEntry(unsigned int ip, unsigned int mask)
{
	unsigned int newip = mask & ip;

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
			range.ips.insert(newip);
		}
	}
	if (!found)
	{
		BannedRange range;
		range.ips.insert(newip);
		range.mask = mask;
		bannedIps_.push_back(range);
	}
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
		std::set<unsigned int>::iterator ipitor;
		for (ipitor = range.ips.begin();
			ipitor != range.ips.end();
			ipitor++)
		{
			// Add ip address
			unsigned int ip = (*ipitor);
			XMLNode *optionNode = 
				new XMLNode("ipaddress", NetInterface::getIpName(ip));

			// Add the mask
			if (m != UINT_MAX)
			{
				optionNode->addParameter(new XMLNode("mask", NetInterface::getIpName(m),
						XMLNode::XMLParameterType));
			}

			// Add to file
			node.addChild(optionNode);
		}
	}
	return node.writeToFile(filename);
}


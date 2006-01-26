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

#include <tank/TankType.h>
#include <XML/XMLParser.h>
#include <common/Defines.h>
#include <engine/ScorchedContext.h>
#include <weapons/AccessoryStore.h>

TankType::TankType()
{
}

TankType::~TankType()
{
}

bool TankType::initFromXML(ScorchedContext &context, XMLNode *node)
{
	if (!node->getNamedChild("name", name_)) return false;
	if (!node->getNamedChild("life", life_)) return false;
	if (!node->getNamedChild("power", power_)) return false;

	XMLNode *accessoryNode = 0;
	while (node->getNamedChild("accessory", accessoryNode, false))
	{
		std::string name;
		int count;
		if (!accessoryNode->getNamedChild("name", name)) return false;
		if (!accessoryNode->getNamedChild("count", count)) return false;
		if (!accessoryNode->failChildren()) return false;

		Accessory *accessory = context.accessoryStore->
			findByPrimaryAccessoryName(name.c_str());
		if (!accessory)
		{
			return accessoryNode->returnError("Failed to find named accessory");
		}

		accessories_[accessory] = count;
	}

	return node->failChildren();
}

const char *TankType::getDescription()
{
	unsigned int count = 0;
	std::string buffer;
	std::map<Accessory *, int>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++, count++)
	{
		if (count == 0) buffer.append("\n");
		Accessory *accessory = (*itor).first;
		buffer.append(accessory->getName());
		if (count + 1 < accessories_.size()) buffer.append("\n");
	}

	return formatString(
		"Life : %.0f\n"
		"Power : %.0f%s",
		getLife(),
		getPower(),
		buffer.c_str());		
}

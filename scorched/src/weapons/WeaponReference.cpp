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

#include <weapons/WeaponReference.h>
#include <weapons/AccessoryStore.h>

REGISTER_ACCESSORY_SOURCE(WeaponReference);

WeaponReference::WeaponReference()
{

}

WeaponReference::~WeaponReference()
{

}

bool WeaponReference::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	// Get the next weapon
	std::string subNode;
	if (!accessoryNode->getNamedChild("weapon", subNode)) return false;

	// Find the accessory name
	std::map<std::string, XMLNode *> &nodes = store_->getParsingNodes();
	std::map<std::string, XMLNode *>::iterator finditor =
		nodes.find(subNode.c_str());
	if (finditor == nodes.end()) 
	{
		dialogMessage("WeaponReference",
			"Failed to find weapon \"%s\"",
			subNode.c_str());
		return false;	
	}
	XMLNode *weaponNode = (*finditor).second;
	weaponNode->resurrectRemovedChildren();
	replaceValues(weaponNode, "name", getName());
	replaceValues(weaponNode, "armslevel", 
		formatString("%i", getArmsLevel()));
	
	// Create the new weapon
	Accessory *accessory = store_->createAccessory(weaponNode);
	if (!accessory || accessory->getType() != Accessory::AccessoryWeapon)
	{
		dialogMessage("Accessory",
			"Failed to find create weapon/wrong type \"%s\"",
			subNode.c_str());
		return false;
	}
	refWeapon_ = (Weapon*) accessory;

	return true;
}

void WeaponReference::replaceValues(XMLNode *node, 
	const char *name, 
	const char *value)
{
	std::list<XMLNode *> &children = node->getChildren();
	std::list<XMLNode *>::iterator itor;
	for (itor = children.begin();
		itor != children.end();
		itor++)
	{
		XMLNode *current = (*itor);
		if (0 == strcmp(current->getName(), name))
		{
			current->setContent(value);
		}
		replaceValues(current, name, value);
	}
}

bool WeaponReference::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	if (!store_->writeWeapon(buffer, refWeapon_)) return false;
	return true;
}

bool WeaponReference::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	refWeapon_ = store_->readWeapon(reader); if (!refWeapon_) return false;
	return true;
}

void WeaponReference::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	refWeapon_->fireWeapon(context, playerId, position, velocity, data);
}


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

#include <weapons/WeaponLeapFrog.h>
#include <weapons/AccessoryStore.h>

REGISTER_ACCESSORY_SOURCE(WeaponLeapFrog);

WeaponLeapFrog::WeaponLeapFrog():  
	collisionAction_(0), bounce_(0.6f)
{

}

WeaponLeapFrog::~WeaponLeapFrog()
{

}

bool WeaponLeapFrog::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	// Get the next weapon
	XMLNode *subNode = accessoryNode->getNamedChild("collisionaction", false, true);
	if (!subNode)
	{
		dialogMessage("Accessory",
			"Failed to find collisionaction node in accessory \"%s\"",
			name_.c_str());
		return false;
	}

	// Check next weapon is correct type
	Accessory *accessory = AccessoryStore::instance()->createAccessory(subNode);
	if (!accessory || accessory->getType() != Accessory::AccessoryWeapon)
	{
		dialogMessage("Accessory",
			"Sub weapon of wrong type \"%s\"",
			name_.c_str());
		return false;
	}
	collisionAction_ = (Weapon*) accessory;

	// Get the bounce
	XMLNode *bounceNode = accessoryNode->getNamedChild("bounce", false, true);
	if (!bounceNode)
	{
		dialogMessage("Accessory",
			"Failed to find bounce node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	bounce_ = (float) atof(bounceNode->getContent());

	return true;
}

bool WeaponLeapFrog::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	if (!Weapon::write(buffer, collisionAction_)) return false;
	buffer.addToBuffer(bounce_);
	return true;
}

bool WeaponLeapFrog::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	collisionAction_ = Weapon::read(reader); if (!collisionAction_) return false;
	if (!reader.getFromBuffer(bounce_)) return false;
	return true;
}

void WeaponLeapFrog::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity)
{
	Vector newVelocity = velocity * bounce_;
	if (newVelocity[2] < 0.0f) newVelocity[2] *= -1.0f;
	collisionAction_->fireWeapon(context, playerId, position, newVelocity);
}


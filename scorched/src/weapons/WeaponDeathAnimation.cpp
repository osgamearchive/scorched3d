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

#include <weapons/WeaponDeathAnimation.h>
#include <weapons/AccessoryStore.h>
#include <actions/DeathAnimation.h>
#include <common/Defines.h>

REGISTER_ACCESSORY_SOURCE(WeaponDeathAnimation);

WeaponDeathAnimation::WeaponDeathAnimation()
{

}

WeaponDeathAnimation::~WeaponDeathAnimation()
{

}

bool WeaponDeathAnimation::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	XMLNode *rendererNode = accessoryNode->removeNamedChild("renderer");
	if (!rendererNode)
	{
		dialogMessage("Accessory",
			"Failed to find renderer node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	rendererName_ = rendererNode->getContent();

	return true;
}

bool WeaponDeathAnimation::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	buffer.addToBuffer(rendererName_);
	return true;
}

bool WeaponDeathAnimation::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(rendererName_)) return false;
	return true;
}

void WeaponDeathAnimation::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity)
{
	context.actionController.addAction(
		new DeathAnimation(playerId, 
			position, velocity,
			rendererName_.c_str()));
}

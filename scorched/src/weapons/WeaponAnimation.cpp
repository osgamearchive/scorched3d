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

#include <weapons/WeaponAnimation.h>
#include <actions/Animation.h>
#include <engine/ActionController.h>
#include <common/Defines.h>

REGISTER_ACCESSORY_SOURCE(WeaponAnimation);

WeaponAnimation::WeaponAnimation()
{

}

WeaponAnimation::~WeaponAnimation()
{

}

bool WeaponAnimation::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	XMLNode *rendererNode = accessoryNode->getNamedChild("animation", false, true);
	if (!rendererNode)
	{
		dialogMessage("Accessory",
			"Failed to find animation node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	rendererName_ = rendererNode->getContent();

	MetaClass *newclass = MetaClassRegistration::getNewClass(
		rendererName_.c_str());
	if (!newclass)
	{
		dialogMessage("Accessory",
			"Failed to find animation named \"%s\" in accessory \"%s\"",
			rendererName_.c_str(),
			name_.c_str());
		return false;
	}
	delete newclass;

	return true;
}

bool WeaponAnimation::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	buffer.addToBuffer(rendererName_);
	return true;
}

bool WeaponAnimation::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(rendererName_)) return false;
	return true;
}

void WeaponAnimation::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity)
{
	context.actionController->addAction(
		new Animation(playerId, 
			position, velocity,
			rendererName_.c_str()));
}

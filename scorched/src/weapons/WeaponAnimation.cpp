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
#include <weapons/Accessory.h>
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

bool WeaponAnimation::parseXML(AccessoryCreateContext &context, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, accessoryNode)) return false;

	if (!accessoryNode->getNamedChild("data", data_)) return false;
	if (!accessoryNode->getNamedChild("animation", rendererName_)) return false;

	if (0 != strcmp(rendererName_.c_str(), "ExplosionLaserBeamRenderer") &&
		0 != strcmp(rendererName_.c_str(), "ExplosionRingRenderer"))
	{
		dialogMessage("Accessory", formatString(
			"Failed to find animation named \"%s\" in accessory \"%s\"",
			rendererName_.c_str(),
			parent_->getName()));
		return false;
	}

	return true;
}

void WeaponAnimation::fireWeapon(ScorchedContext &context,
	WeaponFireContext &weaponContext, Vector &position, Vector &velocity)
{
	context.actionController->addAction(
		new Animation(weaponContext.getPlayerId(), 
			position, velocity,
			rendererName_.c_str(), 
			data_.c_str()));
}

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

#include <weapons/WeaponInvokeWeapon.h>
#include <weapons/AccessoryStore.h>
#include <common/Defines.h>

std::set<Weapon *> WeaponInvokeWeapon::weaponStack_;

REGISTER_ACCESSORY_SOURCE(WeaponInvokeWeapon);

WeaponInvokeWeapon::WeaponInvokeWeapon() :
	invokeWeapon_(0)
{

}

WeaponInvokeWeapon::~WeaponInvokeWeapon()
{

}

bool WeaponInvokeWeapon::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	std::string invoke;
	if (!accessoryNode->getNamedChild("invoke", invoke)) return false;
	Accessory *accessory = store->findByPrimaryAccessoryName(invoke.c_str());
	if (!accessory)
	{
		return accessoryNode->returnError(
			formatString("Failed to find accessory named %s",
				invoke.c_str()));
	}

	AccessoryPart *accessoryPart = accessory->getAction();
	if (!accessoryPart || accessoryPart->getType() != AccessoryPart::AccessoryWeapon)
	{
		return accessoryNode->returnError("Failed to find sub weapon, not a weapon");
	}
	invokeWeapon_ = (Weapon*) accessoryPart;

	return accessoryNode->failChildren();
}

void WeaponInvokeWeapon::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	if (weaponStack_.find(invokeWeapon_) != weaponStack_.end())
	{
		dialogExit("Scorched3D",
			"WeaponInvokeWeapon recursive called for weapon %s",
			invokeWeapon_->getParent()->getName());
	}
    
	weaponStack_.insert(invokeWeapon_);
	invokeWeapon_->fireWeapon(context, playerId, position, velocity, data);
	weaponStack_.erase(invokeWeapon_);
}

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

#include <weapons/WeaponMirv.h>
#include <weapons/AccessoryStore.h>
#include <common/Defines.h>

REGISTER_ACCESSORY_SOURCE(WeaponMirv);

WeaponMirv::WeaponMirv() :
	noWarheads_(0), hspreadDist_(0.0f), vspreadDist_(0.0f)
{

}

WeaponMirv::~WeaponMirv()
{

}

bool WeaponMirv::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	// Get the accessory spread
	if (!accessoryNode->getNamedChild("hspreaddist", hspreadDist_)) 
		return false;
	if (!accessoryNode->getNamedChild("vspreaddist", vspreadDist_)) 
		return false;

	// Get the next weapon
	XMLNode *subNode = 0;
	if (!accessoryNode->getNamedChild("aimedweapon", subNode)) return false;

	// Check next weapon is correct type
	AccessoryPart *accessory = store->createAccessoryPart(context, parent_, subNode);
	if (!accessory || accessory->getType() != AccessoryPart::AccessoryWeapon)
	{
		return subNode->returnError("Failed to find sub weapon, not a weapon");
	}
	aimedWeapon_ = (Weapon*) accessory;

	// Get the accessory warheads
	if (!accessoryNode->getNamedChild("nowarheads", noWarheads_)) return false;

	return true;
}

void WeaponMirv::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	// Add a shot that will fall where the original was aimed
	aimedWeapon_->fireWeapon(context, playerId, position, velocity, data);

	// Add all of the sub warheads that have a random spread
	for (int i=0; i<noWarheads_ - 1; i++)
	{
		Vector newDiff = velocity;
		newDiff[2] = 0.0f;
		if (hspreadDist_ != 0.0f)
		{
			Vector diff = newDiff;
			diff[2] -= 1.0f;
			Vector perp = newDiff * diff;

			newDiff += (perp * ((RAND * hspreadDist_) - (hspreadDist_ * 0.5f)));
		}
		newDiff[2] += (float(i - (noWarheads_ / 2)) / 
			float(noWarheads_ / 2)) * vspreadDist_;

		aimedWeapon_->fireWeapon(context, playerId, position, newDiff, data);
	}
}


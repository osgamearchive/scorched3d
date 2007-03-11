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

#include <weapons/WeaponLaser.h>
#include <weapons/AccessoryStore.h>
#include <actions/Laser.h>
#include <engine/ActionController.h>

REGISTER_ACCESSORY_SOURCE(WeaponLaser);

WeaponLaser::WeaponLaser()
{
}

WeaponLaser::~WeaponLaser()
{
}

bool WeaponLaser::parseXML(AccessoryCreateContext &context, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, accessoryNode)) return false;

	if (!accessoryNode->getNamedChild("minimumhurt", minimumHurtExp_)) return false;
	if (!accessoryNode->getNamedChild("maximumhurt", maximumHurtExp_)) return false;
	if (!accessoryNode->getNamedChild("hurtradius", hurtRadiusExp_)) return false;
	if (!accessoryNode->getNamedChild("minimumdistance", minimumDistanceExp_)) return false;
	if (!accessoryNode->getNamedChild("maximumdistance", maximumDistanceExp_)) return false;
	if (!accessoryNode->getNamedChild("totaltime", totalTimeExp_)) return false;
	if (!accessoryNode->getNamedChild("color", color_)) return false;

	return true;
}

void WeaponLaser::fireWeapon(ScorchedContext &context,
	WeaponFireContext &weaponContext, Vector &position, Vector &velocity)
{
	// convert NumberParser expressions to values
	minimumHurt_ = minimumHurtExp_.getValue(context);
	maximumHurt_ = maximumHurtExp_.getValue(context);
	minimumDistance_ = minimumDistanceExp_.getValue(context);
	maximumDistance_ = maximumDistanceExp_.getValue(context);
	hurtRadius_ = hurtRadiusExp_.getValue(context);
	totalTime_ = totalTimeExp_.getValue(context);

	context.actionController->addAction(
		new Laser(this, position, velocity, weaponContext));
}

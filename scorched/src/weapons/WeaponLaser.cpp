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

bool WeaponLaser::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	if (!accessoryNode->getNamedChild("minimumhurt", minimumHurt_)) return false;
	if (!accessoryNode->getNamedChild("maximumhurt", maximumHurt_)) return false;
	if (!accessoryNode->getNamedChild("hurtradius", hurtRadius_)) return false;
	if (!accessoryNode->getNamedChild("minimumdistance", minimumDistance_)) return false;
	if (!accessoryNode->getNamedChild("maximumdistance", maximumDistance_)) return false;
	if (!accessoryNode->getNamedChild("totaltime", totalTime_)) return false;
	if (!accessoryNode->getNamedChild("color", color_)) return false;

	return true;
}

void WeaponLaser::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	context.actionController->addAction(
		new Laser(playerId, this, position, velocity, data));
}

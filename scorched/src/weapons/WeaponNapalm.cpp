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

#include <weapons/WeaponNapalm.h>
#include <tank/TankContainer.h>
#include <actions/ShotProjectileNapalm.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WeaponNapalm::WeaponNapalm() : hot_(false)
{

}

WeaponNapalm::~WeaponNapalm()
{

}

bool WeaponNapalm::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	// Get the accessory hot
	XMLNode *hotNode = accessoryNode->getNamedChild("hot");
	if (!hotNode)
	{
		dialogMessage("Accessory",
			"Failed to find hot node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	hot_ = (strcmp(hotNode->getContent(), "true") == 0);

	return true;
}

Action *WeaponNapalm::fireWeapon(unsigned int playerId, Vector &position, Vector &velocity)
{
	Tank *tank = TankContainer::instance()->getTankById(playerId);
	if (tank)
	{
		Action *action = new ShotProjectileNapalm(
			position, 
			velocity,
			this, playerId);

		return action;
	}

	return 0;
}

const char *WeaponNapalm::getFiredSound()
{
	return PKGDIR "data/wav/shoot/small.wav";
}
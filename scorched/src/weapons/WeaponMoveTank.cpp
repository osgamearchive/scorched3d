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

#include <weapons/WeaponMoveTank.h>
#include <weapons/AccessoryStore.h>
#include <actions/TankMovement.h>
#include <engine/ActionController.h>
#include <landscape/LandscapeMaps.h>
#include <landscapedef/LandscapeDefn.h>

REGISTER_ACCESSORY_SOURCE(WeaponMoveTank);

WeaponMoveTank::WeaponMoveTank() :
	stepTime_(0.05f), useFuel_(true), maximumRange_(100)
{

}

WeaponMoveTank::~WeaponMoveTank()
{

}

bool WeaponMoveTank::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;
	accessoryNode->getNamedChild("steptime", stepTime_, false);
	accessoryNode->getNamedChild("usefuel", useFuel_, false);
	accessoryNode->getNamedChild("maximumrange", maximumRange_, false);
	return true;
}

void WeaponMoveTank::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	int posX = (int) position[0];
	int posY = (int) position[1];
	if (posX > 5 && posX < context.landscapeMaps->getDefinitions().getDefn()->landscapewidth - 5 &&
		posY > 5 && posY < context.landscapeMaps->getDefinitions().getDefn()->landscapeheight - 5)
	{
		context.actionController->addAction(
			new TankMovement(playerId, this, posX, posY));		
	}
}
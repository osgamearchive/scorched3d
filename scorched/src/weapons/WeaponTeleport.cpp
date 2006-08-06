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

#include <weapons/WeaponTeleport.h>
#include <engine/ActionController.h>
#include <landscape/LandscapeMaps.h>
#include <landscapedef/LandscapeTex.h>
#include <actions/Teleport.h>
#include <common/Defines.h>

REGISTER_ACCESSORY_SOURCE(WeaponTeleport);

WeaponTeleport::WeaponTeleport() : groundOnly_(true)
{

}

WeaponTeleport::~WeaponTeleport()
{

}

bool WeaponTeleport::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;
	if (!accessoryNode->getNamedChild("delay", delay_)) return false;
	if (!accessoryNode->getNamedChild("sound", sound_)) return false;
	accessoryNode->getNamedChild("groundonly", groundOnly_, false);
	
	if (!checkDataFile(getSound())) return false;

	return true;
}

void WeaponTeleport::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	// Mininum height
	float allowedHeight = 0.0f;
	LandscapeTex &tex = *context.landscapeMaps->getDefinitions().getTex();
		if (tex.border->getType() == LandscapeTexType::eWater)
	{
		LandscapeTexBorderWater *water = 
			(LandscapeTexBorderWater *) tex.border;

		allowedHeight = water->height;
	}

	int mapWidth = context.landscapeMaps->getGroundMaps().getMapWidth();
	int mapHeight = context.landscapeMaps->getGroundMaps().getMapHeight();

	if (position[0] > 5.0f && 
		position[1] > 5.0f &&
		position[0] < float(mapWidth - 5) &&
		position[1] < float(mapHeight - 5))
	{
		float landscapeHeight = context.landscapeMaps->getGroundMaps().getInterpHeight(
			position[0], position[1]);
		if (landscapeHeight >= allowedHeight - 1.0f)
		{
			context.actionController->addAction(new Teleport(position, playerId, this));
		}
	}
}

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

#include <weapons/WeaponScatter.h>
#include <weapons/AccessoryStore.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/LandscapeTex.h>
#include <common/Defines.h>
#include <math.h>

REGISTER_ACCESSORY_SOURCE(WeaponScatter);

WeaponScatter::WeaponScatter()
{

}

WeaponScatter::~WeaponScatter()
{

}

bool WeaponScatter::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

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

	if (!accessoryNode->getNamedChild("height", height_)) return false;
	if (!accessoryNode->getNamedChild("landonly", landonly_)) return false;
	if (!accessoryNode->getNamedChild("direction", direction_)) return false;
	if (!accessoryNode->getNamedChild("directionoffset", directionOffset_)) return false;

	return true;
}

void WeaponScatter::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &p, Vector &v,
	unsigned int data)
{
	// Mininum height, if we are grounding
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

	Vector pos, vel;
	vel[0] += direction_[0] - directionOffset_[0] + 
		directionOffset_[0] * 2.0f * RAND;
	vel[1] += direction_[1] - directionOffset_[1] + 
		directionOffset_[1] * 2.0f * RAND;
	vel[2] += direction_[2] - directionOffset_[2] + 
		directionOffset_[2] * 2.0f * RAND;

	bool ok = false;
	while (!ok)
	{
		ok = true;
		pos[0] = RAND * float(mapWidth - 10) + 5.0f;
		pos[1] = RAND * float(mapHeight - 10) + 5.0f;
		pos[2] = height_;
		if (height_ == 0.0f)
		{
			pos[2] = context.landscapeMaps->getGroundMaps().getInterpHeight(
				pos[0], pos[1]);
		}

		if (landonly_)
		{
			if (pos[2] < allowedHeight)
			{
				ok = false;
				allowedHeight -= 0.01f;
			}
		}
	}

	aimedWeapon_->fireWeapon(context, playerId, pos, vel, data);
}


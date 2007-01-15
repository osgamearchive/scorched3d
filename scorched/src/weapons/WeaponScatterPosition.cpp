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

#include <weapons/WeaponScatterPosition.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapedef/LandscapeTex.h>
#include <common/Defines.h>
#include <math.h>

REGISTER_ACCESSORY_SOURCE(WeaponScatterPosition);

WeaponScatterPosition::WeaponScatterPosition()
{

}

WeaponScatterPosition::~WeaponScatterPosition()
{

}

bool WeaponScatterPosition::parseXML(AccessoryCreateContext &context, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, accessoryNode)) return false;

	// Get the next weapon
	XMLNode *subNode = 0;
	if (!accessoryNode->getNamedChild("aimedweapon", subNode)) return false;

	// Check next weapon is correct type
	AccessoryPart *accessory = context.getAccessoryStore()->
		createAccessoryPart(context, parent_, subNode);
	if (!accessory || accessory->getType() != AccessoryPart::AccessoryWeapon)
	{
		return subNode->returnError("Failed to find sub weapon, not a weapon");
	}
	aimedWeapon_ = (Weapon*) accessory;

	if (!accessoryNode->getNamedChild("landonly", landonly_)) return false;
	if (!accessoryNode->getNamedChild("landheight", landheight_)) return false;
	if (!accessoryNode->getNamedChild("scatterpercentage", scatterpercentage_)) return false;

	// Check if position and velocity should be updated for future events
	accessoryNode->getNamedChild("updateposition", updatePosition_, false);

	return true;
}

void WeaponScatterPosition::fireWeapon(ScorchedContext &context,
	WeaponFireContext &weaponContext, Vector &p, Vector &velocity)
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

	float width = 
		float(context.landscapeMaps->getGroundMaps().getMapWidth()) *
		scatterpercentage_ / 100.0f;
	float height = 
		float(context.landscapeMaps->getGroundMaps().getMapHeight()) *
		scatterpercentage_ / 100.0f;

	Vector pos;
	RandomGenerator &random = context.actionController->getRandom();
	bool ok = false;
	while (!ok)
	{
		ok = true;

		pos[0] = p[0] + (random.getRandFloat() * width) - (width / 2.0f);
		pos[1] = p[1] + (random.getRandFloat() * height) - (height / 2.0f);
		pos[2] = p[2];
		if (landheight_)
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

	aimedWeapon_->fireWeapon(context, weaponContext, pos, velocity);

	if (updatePosition_)
		p = pos;
}


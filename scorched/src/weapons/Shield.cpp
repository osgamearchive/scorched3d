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

#include <weapons/Shield.h>
#include <common/VectorLib.h>
#include <common/Defines.h>
#include <math.h>

REGISTER_ACCESSORY_SOURCE(Shield);

Shield::Shield() : 
	laserProof_(false), movementProof_(false), glow_(true)
{
}

Shield::~Shield()
{
}

bool Shield::parseXML(OptionsGame &context,
	AccessoryStore *store, XMLNode *accessoryNode)
{
	// Get the remove power 
	if (!accessoryNode->getNamedChild("removepower", removePower_)) return false;

	// Get the penetration
	if (!accessoryNode->getNamedChild("penetration", penetration_)) return false;

	// Get the penetration
	if (!accessoryNode->getNamedChild("power", power_)) return false;

	// Get the penetration
	if (!accessoryNode->getNamedChild("radius", radius_)) return false;
	if (radius_ <= 0.0f) return accessoryNode->returnError("Shield radius must be > 0");

	// Get the collision sound
	if (!accessoryNode->getNamedChild("collisionsound", collisionSound_)) return false;
	if (!checkDataFile(formatString("data/wav/%s", getCollisionSound()))) return false;

	// Get the accessory color
	XMLNode *colorNode = 0;
	if (!accessoryNode->getNamedChild("color", colorNode)) return false;
	if (!colorNode->getNamedChild("r", color_[0])) return false;
	if (!colorNode->getNamedChild("g", color_[1])) return false;
	if (!colorNode->getNamedChild("b", color_[2])) return false;

	// Get the half size
	if (!accessoryNode->getNamedChild("halfshield", halfShield_)) return false;

	accessoryNode->getNamedChild("laserproof", laserProof_, false);
	accessoryNode->getNamedChild("movementproof", movementProof_, false);
	accessoryNode->getNamedChild("glow", glow_, false);

	return true;
}

const char *Shield::getCollisionSound()
{
	if (!collisionSound_.c_str()[0]) return 0;
	return collisionSound_.c_str();
}

Shield::ShieldType Shield::getShieldType()
{
	return ShieldTypeNormal;
}

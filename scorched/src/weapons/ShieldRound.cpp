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

#include <weapons/ShieldRound.h>
#include <common/VectorLib.h>
#include <common/Defines.h>
#include <math.h>

REGISTER_ACCESSORY_SOURCE(ShieldRound);

ShieldRound::ShieldRound() : 
	glow_(true)
{
}

ShieldRound::~ShieldRound()
{
}

Shield::ShieldType ShieldRound::getShieldType()
{
	return ShieldTypeRoundNormal;
}

bool ShieldRound::inShield(Vector &offset)
{
	return offset.Magnitude() <= radius_;
}

bool ShieldRound::parseXML(OptionsGame &context,
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Shield::parseXML(context, store, accessoryNode)) return false;

	// Get the penetration
	if (!accessoryNode->getNamedChild("radius", radius_)) return false;
	if (radius_ <= 0.0f) return accessoryNode->returnError("ShieldRound radius must be > 0");

	// Get the half size
	if (!accessoryNode->getNamedChild("halfshield", halfShield_)) return false;

	accessoryNode->getNamedChild("glow", glow_, false);

	return true;
}

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

Shield::Shield()
{

}

Shield::~Shield()
{

}

bool Shield::parseXML(XMLNode *accessoryNode)
{
	if (!Accessory::parseXML(accessoryNode)) return false;

	// Get the accessory radius
	XMLNode *radiusNode = 0;
	if (!accessoryNode->getNamedChild("radius", radiusNode)) return false; 
	radius_ = ((strcmp(radiusNode->getContent(), "large")==0)?ShieldSizeLarge:ShieldSizeSmall);

	// Get the remove power 
	if (!accessoryNode->getNamedChild("removepower", removePower_)) return false;

	// Get the collision sound
	if (!accessoryNode->getNamedChild("collisionsound", collisionSound_)) return false;

	// Get the accessory color
	XMLNode *colorNode = 0;
	if (!accessoryNode->getNamedChild("color", colorNode)) return false;
	if (!colorNode->getNamedChild("r", color_[0])) return false;
	if (!colorNode->getNamedChild("g", color_[1])) return false;
	if (!colorNode->getNamedChild("b", color_[2])) return false;

	// Get the half size
	if (!accessoryNode->getNamedChild("halfshield", halfShield_)) return false;

	return true;
}

bool Shield::writeAccessory(NetBuffer &buffer)
{
	if (!Accessory::writeAccessory(buffer)) return false;
	buffer.addToBuffer((int) radius_);
	buffer.addToBuffer(removePower_);
	buffer.addToBuffer(collisionSound_);
	buffer.addToBuffer(color_);
	buffer.addToBuffer(halfShield_);
	return true;
}

bool Shield::readAccessory(NetBufferReader &reader)
{
	if (!Accessory::readAccessory(reader)) return false;
	int r; if (!reader.getFromBuffer(r)) return false; radius_ = (ShieldSize) r;
	if (!reader.getFromBuffer(removePower_)) return false;
	if (!reader.getFromBuffer(collisionSound_)) return false;
	if (!reader.getFromBuffer(color_)) return false;
	if (!reader.getFromBuffer(halfShield_)) return false;
	return true;
}

const char *Shield::getCollisionSound()
{
	if (!collisionSound_.c_str()[0]) return 0;
	return collisionSound_.c_str();
}

float Shield::getHitRemovePower()
{
	return removePower_;
	if (radius_ == ShieldSizeSmall) return 20;
	return 15;
}

Shield::ShieldType Shield::getShieldType()
{
	return ShieldTypeNormal;
}

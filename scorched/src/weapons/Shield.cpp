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
	XMLNode *radiusNode = accessoryNode->getNamedChild("radius", false, true);
	if (!radiusNode)
	{
		dialogMessage("Accessory",
			"Failed to find radius node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	radius_ = ((strcmp(radiusNode->getContent(), "large")==0)?ShieldSizeLarge:ShieldSizeSmall);

	// Get the remove power 
	XMLNode *removePowerNode = accessoryNode->getNamedChild("removepower", false, true);
	if (!removePowerNode)
	{
		dialogMessage("Accessory",
			"Failed to find removepower node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	removePower_ = (float) atof(removePowerNode->getContent());

	// Get the collision sound
	XMLNode *collisionSoundNode = accessoryNode->getNamedChild("collisionsound", false, true);
	if (!collisionSoundNode)
	{
		dialogMessage("Accessory",
			"Failed to find collisionsound node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	collisionSound_ = collisionSoundNode->getContent();

	// Get the accessory color
	XMLNode *colorNode = accessoryNode->getNamedChild("color", false, true);
	if (!colorNode)
	{
		dialogMessage("Accessory",
			"Failed to find color node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	XMLNode *rcolorNode = colorNode->getNamedChild("r", false, true);
	XMLNode *gcolorNode = colorNode->getNamedChild("g", false, true);
	XMLNode *bcolorNode = colorNode->getNamedChild("b", false, true);
	if (!rcolorNode || !gcolorNode || !bcolorNode)
	{
		dialogMessage("Accessory",
			"Failed to find color component node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	color_[0] = (float) atof(rcolorNode->getContent());
	color_[1] = (float) atof(gcolorNode->getContent());
	color_[2] = (float) atof(bcolorNode->getContent());

	return true;
}

bool Shield::writeAccessory(NetBuffer &buffer)
{
	if (!Accessory::writeAccessory(buffer)) return false;
	buffer.addToBuffer((int) radius_);
	buffer.addToBuffer(removePower_);
	buffer.addToBuffer(collisionSound_);
	buffer.addToBuffer(color_[0]);
	buffer.addToBuffer(color_[1]);
	buffer.addToBuffer(color_[2]);
	return true;
}

bool Shield::readAccessory(NetBufferReader &reader)
{
	if (!Accessory::readAccessory(reader)) return false;
	int r; if (!reader.getFromBuffer(r)) return false; radius_ = (ShieldSize) r;
	if (!reader.getFromBuffer(removePower_)) return false;
	if (!reader.getFromBuffer(collisionSound_)) return false;
	if (!reader.getFromBuffer(color_[0])) return false;
	if (!reader.getFromBuffer(color_[1])) return false;
	if (!reader.getFromBuffer(color_[2])) return false;
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

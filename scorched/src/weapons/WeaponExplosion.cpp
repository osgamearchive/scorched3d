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

#include <weapons/WeaponExplosion.h>

REGISTER_ACCESSORY_SOURCE(WeaponExplosion);

WeaponExplosion::WeaponExplosion() : size_(0), 
	multiColored_(false), hurts_(false),
	deformType_(Explosion::DeformNone)
{

}

WeaponExplosion::~WeaponExplosion()
{

}

bool WeaponExplosion::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

    // Get the accessory size
    XMLNode *sizeNode = accessoryNode->getNamedChild("size");
    if (!sizeNode)
    {
        dialogMessage("Accessory",
            "Failed to find size node in accessory \"%s\"",
            name_.c_str());
        return false;
    }
    size_ = atoi(sizeNode->getContent());

    // Get the accessory colored
    XMLNode *colorNode = accessoryNode->getNamedChild("multicolor");
    if (colorNode) multiColored_ = true;

    // Get the hutiness
    XMLNode *hurtsNode = accessoryNode->getNamedChild("hurts");
    if (!hurtsNode)
    {
        dialogMessage("Accessory",
            "Failed to find hurts node in accessory \"%s\"",
            name_.c_str());
        return false;
    }
	if (0==strcmp(hurtsNode->getContent(), "yes")) hurts_ = true;
	else if (0==strcmp(hurtsNode->getContent(), "no")) hurts_ = false;
	else 
	{
        dialogMessage("Accessory",
            "Unknown hurts type \"%s\" in accessory \"%s\"",
            hurtsNode->getContent(), name_.c_str());
        return false;			
	}

	// Get the deform
	XMLNode *deformNode = accessoryNode->getNamedChild("deform");
    if (!deformNode)
    {
        dialogMessage("Accessory",
            "Failed to find deform node in accessory \"%s\"",
            name_.c_str());
        return false;
    }
    if (0 == strcmp(deformNode->getContent(), "down")) deformType_ = Explosion::DeformDown;
	else if (0 == strcmp(deformNode->getContent(), "up")) deformType_ = Explosion::DeformUp;
	else if (0 == strcmp(deformNode->getContent(), "none")) deformType_ = Explosion::DeformNone;
	else
	{
        dialogMessage("Accessory",
            "Unknown deform type \"%s\" in accessory \"%s\"",
            deformNode->getContent(), name_.c_str());
        return false;		
	}

	return true;
}

bool WeaponExplosion::writeAccessory(NetBuffer &buffer)
{
    if (!Weapon::writeAccessory(buffer)) return false;
    buffer.addToBuffer(size_);
	buffer.addToBuffer(hurts_);
	buffer.addToBuffer((int) deformType_);
	buffer.addToBuffer(multiColored_);
    return true;
}

bool WeaponExplosion::readAccessory(NetBufferReader &reader)
{
    if (!Weapon::readAccessory(reader)) return false;
    if (!reader.getFromBuffer(size_)) return false;
	if (!reader.getFromBuffer(hurts_)) return false;
	int def = 0;
	if (!reader.getFromBuffer(def)) return false;
	deformType_ = (Explosion::DeformType) def;
	if (!reader.getFromBuffer(multiColored_)) return false;
    return true;
}

Vector &WeaponExplosion::getExplosionColor()
{
    if (!multiColored_) return Weapon::getExplosionColor();

    static Vector red(1.0f, 0.0f, 0.0f);
    static Vector green(0.0f, 1.0f, 0.0f);
    static Vector blue(0.0f, 0.0f, 1.0f);
    static Vector yellow(1.0f, 1.0f, 0.0f);

    int color = int(RAND * 4.0f);
    switch (color)
    {
    case 0:
        return red;
    case 1:
        return green;
    case 2:
        return blue;
    case 3:
        return yellow;
    }
    return Weapon::getExplosionColor();
}

void WeaponExplosion::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity)
{
	Action *action = new Explosion(
		position, (float) size_,
		this, playerId, hurts_, deformType_);
	context.actionController.addAction(action);	
}

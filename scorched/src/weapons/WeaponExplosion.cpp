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
#include <engine/ActionController.h>

REGISTER_ACCESSORY_SOURCE(WeaponExplosion);

WeaponExplosion::WeaponExplosion() : size_(0.0f), 
	multiColored_(false), hurtAmount_(0.0f),
	deformType_(Explosion::DeformNone),
	createDebris_(true), createMushroom_(false),
	createSplash_(true), windAffected_(true),
	luminance_(true), animate_(false),
	minLife_(0.5f), maxLife_(1.0f)
{

}

WeaponExplosion::~WeaponExplosion()
{

}

bool WeaponExplosion::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

    // Get the accessory size
    if (!accessoryNode->getNamedChild("size", size_)) return false;

    // Get the accessory colored
    XMLNode *colorNode = 0;
	accessoryNode->getNamedChild("multicolor", colorNode, false);
    if (colorNode) multiColored_ = true;

    // Get the hutiness
	if (!accessoryNode->getNamedChild("hurtamount", hurtAmount_)) return false;

	// Get the no debris node
	XMLNode *noCreateDebrisNode = 0;
	accessoryNode->getNamedChild("nocreatedebris", noCreateDebrisNode, false);
	if (noCreateDebrisNode) createDebris_ = false;

	// Get the no splash node
	XMLNode *noCreateSplashNode = 0;
	accessoryNode->getNamedChild("nocreatesplash", noCreateSplashNode, false);
	if (noCreateSplashNode) createSplash_ = false;

	// Get the createMushroom node
	XMLNode *createMushroomNode = 0;
	accessoryNode->getNamedChild("createmushroom", createMushroomNode, false);
	if (createMushroomNode) createMushroom_ = true;

	// Get the no windaffecting
	XMLNode *noWindAffectedNode = 0;
	accessoryNode->getNamedChild("nowindaffected", noWindAffectedNode, false);
	if (noWindAffectedNode) windAffected_ = false;

	// Get the no luminance node
	XMLNode *noLuminanceNode = 0;
	accessoryNode->getNamedChild("noluminance", noLuminanceNode, false);
	if (noLuminanceNode) luminance_ = false;

	// Get the animate node
	XMLNode *animateNode = 0;
	accessoryNode->getNamedChild("animate", animateNode, false);
	if (animateNode) animate_ = true;

	// Get the optional explosion life nodes
	accessoryNode->getNamedChild("minlife", minLife_, false);
	accessoryNode->getNamedChild("maxlife", maxLife_, false);

	// Get the deform
	XMLNode *deformNode = 0;
	if (!accessoryNode->getNamedChild("deform", deformNode)) return false;
    if (0 == strcmp(deformNode->getContent(), "down")) deformType_ = Explosion::DeformDown;
	else if (0 == strcmp(deformNode->getContent(), "up")) deformType_ = Explosion::DeformUp;
	else if (0 == strcmp(deformNode->getContent(), "none")) deformType_ = Explosion::DeformNone;
	else
	{
        dialogMessage("Accessory",
            "Unknown deform type \"%s\" in accessory \"%s\" should be up, down or none",
            deformNode->getContent(), name_.c_str());
        return false;		
	}

	return true;
}

bool WeaponExplosion::writeAccessory(NetBuffer &buffer)
{
    if (!Weapon::writeAccessory(buffer)) return false;
    buffer.addToBuffer(size_);
	buffer.addToBuffer(minLife_);
	buffer.addToBuffer(maxLife_);
	buffer.addToBuffer(hurtAmount_);
	buffer.addToBuffer((int) deformType_);
	buffer.addToBuffer(multiColored_);
	buffer.addToBuffer(createDebris_);
	buffer.addToBuffer(createMushroom_);
	buffer.addToBuffer(createSplash_);
	buffer.addToBuffer(windAffected_);
	buffer.addToBuffer(luminance_);
	buffer.addToBuffer(animate_);
    return true;
}

bool WeaponExplosion::readAccessory(NetBufferReader &reader)
{
    if (!Weapon::readAccessory(reader)) return false;
    if (!reader.getFromBuffer(size_)) return false;
	if (!reader.getFromBuffer(minLife_)) return false;
	if (!reader.getFromBuffer(maxLife_)) return false;
	if (!reader.getFromBuffer(hurtAmount_)) return false;
	int def = 0;
	if (!reader.getFromBuffer(def)) return false;
	deformType_ = (Explosion::DeformType) def;
	if (!reader.getFromBuffer(multiColored_)) return false;
	if (!reader.getFromBuffer(createDebris_)) return false;
	if (!reader.getFromBuffer(createMushroom_)) return false;
	if (!reader.getFromBuffer(createSplash_)) return false;
	if (!reader.getFromBuffer(windAffected_)) return false;
	if (!reader.getFromBuffer(luminance_)) return false;
	if (!reader.getFromBuffer(animate_)) return false;
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
		position, this, playerId);
	context.actionController->addAction(action);	
}


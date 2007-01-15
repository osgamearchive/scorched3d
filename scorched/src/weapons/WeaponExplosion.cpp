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
#include <weapons/Accessory.h>
#include <engine/ActionController.h>
#include <common/Defines.h>

REGISTER_ACCESSORY_SOURCE(WeaponExplosion);

WeaponExplosion::WeaponExplosion() : size_(0.0f), 
	multiColored_(false), hurtAmount_(0.0f),
	deform_(Explosion::DeformNone),
	createDebris_(true), createMushroomAmount_(0.0f),
	createSplash_(true), windAffected_(true),
	luminance_(true), animate_(false),
	onlyHurtShield_(false),
	minLife_(0.5f), maxLife_(1.0f), shake_(0.0f),
	explosionTexture_("exp00")
{

}

WeaponExplosion::~WeaponExplosion()
{

}

bool WeaponExplosion::parseXML(AccessoryCreateContext &context, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, accessoryNode)) return false;

    // Get the accessory size
    if (!accessoryNode->getNamedChild("size", size_)) return false;

    // Get the accessory colored
    XMLNode *colorNode = 0;
	accessoryNode->getNamedChild("multicolor", colorNode, false);
    if (colorNode) multiColored_ = true;

    // Get the hutiness
	if (!accessoryNode->getNamedChild("hurtamount", hurtAmount_)) return false;

	// Get the weapon model explosion shake
	accessoryNode->getNamedChild("explosionshake", shake_, false);

	// Get the hurt shield node
	accessoryNode->getNamedChild("onlyhurtshield", onlyHurtShield_, false);

	// Get the no debris node
	XMLNode *noCreateDebrisNode = 0;
	accessoryNode->getNamedChild("nocreatedebris", noCreateDebrisNode, false);
	if (noCreateDebrisNode) createDebris_ = false;

	// Get the no splash node
	XMLNode *noCreateSplashNode = 0;
	accessoryNode->getNamedChild("nocreatesplash", noCreateSplashNode, false);
	if (noCreateSplashNode) createSplash_ = false;

	// Get the createmushroomamount node
	accessoryNode->getNamedChild("createmushroomamount", createMushroomAmount_, false);

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

	// Get tje deform texture
	if (accessoryNode->getNamedChild("deformtexture", deformTexture_, false))
	{
		if (!checkDataFile(getDeformTexture())) return false;
	}

	// Get the explosion texture
	accessoryNode->getNamedChild("explosiontexture", explosionTexture_, false);

	// Get the explosion sound
	if (!accessoryNode->getNamedChild("explosionsound", explosionSound_, false))
	{
		if (!checkDataFile(formatString("data/wav/%s", getExplosionSound()))) return false;
	}

	// Get the deform
	XMLNode *deformNode = 0;
	if (!accessoryNode->getNamedChild("deform", deformNode)) return false;
    if (0 == strcmp(deformNode->getContent(), "down")) deform_ = Explosion::DeformDown;
	else if (0 == strcmp(deformNode->getContent(), "up")) deform_ = Explosion::DeformUp;
	else if (0 == strcmp(deformNode->getContent(), "none")) deform_ = Explosion::DeformNone;
	else
	{
        dialogMessage("Accessory", formatString(
            "Unknown deform type \"%s\" in accessory \"%s\" should be up, down or none",
            deformNode->getContent(), parent_->getName()));
        return false;		
	}

	return true;
}

const char *WeaponExplosion::getExplosionTexture()
{
	return explosionTexture_.c_str();
}

const char *WeaponExplosion::getExplosionSound()
{
	if (!explosionSound_.c_str()[0]) return 0;
	return explosionSound_.c_str();
}

Vector &WeaponExplosion::getExplosionColor()
{
	static Vector white(1.0f, 1.0f, 1.0f);
	if (!multiColored_) return white;

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
    return white;
}

void WeaponExplosion::fireWeapon(ScorchedContext &context,
	WeaponFireContext &weaponContext, Vector &position, Vector &velocity)
{
	Action *action = new Explosion(
		position, this, weaponContext);
	context.actionController->addAction(action);	
}


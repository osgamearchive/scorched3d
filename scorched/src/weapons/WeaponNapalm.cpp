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

#include <weapons/WeaponNapalm.h>
#include <actions/Napalm.h>
#include <common/Defines.h>
#include <common/SoundStore.h>
#include <engine/ActionController.h>
#include <landscape/LandscapeMaps.h>

REGISTER_ACCESSORY_SOURCE(WeaponNapalm);

WeaponNapalm::WeaponNapalm() : 
	noSmoke_(false), noObjectDamage_(false)
{

}

WeaponNapalm::~WeaponNapalm()
{

}

bool WeaponNapalm::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	// Mandatory Attributes
	if (!accessoryNode->getNamedChild("napalmtime", napalmTime_)) return false;
	if (!accessoryNode->getNamedChild("napalmheight", napalmHeight_)) return false;
	if (!accessoryNode->getNamedChild("steptime", stepTime_)) return false;
	if (!accessoryNode->getNamedChild("hurtsteptime", hurtStepTime_)) return false;
	if (!accessoryNode->getNamedChild("hurtpersecond", hurtPerSecond_)) return false;
	if (!accessoryNode->getNamedChild("numberstreams", numberStreams_)) return false;
	if (!accessoryNode->getNamedChild("effectradius", effectRadius_)) return false;
	if (!accessoryNode->getNamedChild("napalmsound", napalmSound_)) return false;
	if (!accessoryNode->getNamedChild("napalmtexture", napalmTexture_)) return false;
	if (!accessoryNode->getNamedChild("allowunderwater", allowUnderWater_)) return false;
	if (!checkDataFile("data/wav/%s", getNapalmSound())) return false;

	// Optional Attributes
	XMLNode *noSmokeNode = 0, *noObjectDamageNode = 0;
	accessoryNode->getNamedChild("nosmoke", noSmokeNode, false);
	accessoryNode->getNamedChild("noobjectdamage", noObjectDamageNode, false);
	if (noSmokeNode) noSmoke_ = true;
	if (noObjectDamageNode) noObjectDamage_ = true;

	return true;
}

void WeaponNapalm::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	for (int i=0; i<numberStreams_; i++)
	{
		int x = int(position[0] + RAND * 4.0f - 2.0f);
		int y = int(position[1] + RAND * 4.0f - 2.0f);
		addNapalm(context, playerId, x, y, data);
	}

	if (!context.serverMode) 
	{
		if (getNapalmSound()[0] &&
			0 != strcmp(getNapalmSound(), "none"))
		{
			SoundBuffer *expSound = 
				SoundStore::instance()->fetchOrCreateBuffer((char *)
					getDataFile("data/wav/%s", getNapalmSound()));
			expSound->play();
		}
	}
}

void WeaponNapalm::addNapalm(ScorchedContext &context, 
	unsigned int playerId, int x, int y, unsigned int data)
{
	// Ensure that the napalm has not hit the walls
	// or anything outside the landscape
	if (x > 1 && y > 1 &&
		x < context.landscapeMaps->getHMap().getWidth() - 1 &&
		y < context.landscapeMaps->getHMap().getWidth() - 1)
	{
		context.actionController->addAction(
			new Napalm(x, y, this, playerId, data));
	}
}

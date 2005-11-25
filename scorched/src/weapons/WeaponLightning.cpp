////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <weapons/WeaponLightning.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>
#include <actions/Lightning.h>
#include <common/Defines.h>

REGISTER_ACCESSORY_SOURCE(WeaponLightning);

WeaponLightning::WeaponLightning()
{
}

WeaponLightning::~WeaponLightning()
{
}

bool WeaponLightning::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;
	if (!accessoryNode->getNamedChild("conelength", coneLength_)) return false;
	if (!accessoryNode->getNamedChild("seglength", segLength_)) return false;
	if (!accessoryNode->getNamedChild("segvar", segVar_)) return false;
	if (!accessoryNode->getNamedChild("size", size_)) return false;
	if (!accessoryNode->getNamedChild("sizevar", sizeVar_)) return false;
	if (!accessoryNode->getNamedChild("minsize", minSize_)) return false;
	if (!accessoryNode->getNamedChild("splitprob", splitProb_)) return false;
	if (!accessoryNode->getNamedChild("splitvar", splitVar_)) return false;
	if (!accessoryNode->getNamedChild("deathprob", deathProb_)) return false;
	if (!accessoryNode->getNamedChild("derivangle", derivAngle_)) return false;
	if (!accessoryNode->getNamedChild("anglevar", angleVar_)) return false;
	if (!accessoryNode->getNamedChild("totaltime", totalTime_)) return false;
	if (!accessoryNode->getNamedChild("seghurt", segHurt_)) return false;
	if (!accessoryNode->getNamedChild("seghurtradius", segHurtRadius_)) return false;
	if (!accessoryNode->getNamedChild("sound", sound_)) return false;
	if (!checkDataFile("%s", getSound())) return false;
	return true;
}

void WeaponLightning::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	Action *action = new Lightning(
		this, playerId, position, velocity, data); 
	context.actionController->addAction(action);	
}

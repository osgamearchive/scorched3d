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

#include <weapons/AccessoryStore.h>
#include <weapons/WeaponProjectile.h>
#include <actions/ShotProjectile.h>
#include <engine/ActionController.h>

REGISTER_ACCESSORY_SOURCE(WeaponProjectile);

WeaponProjectile::WeaponProjectile() : 
	under_(false), collisionAction_(0), apexCollision_(false),
	showShotPath_(false), showEndPoint_(false), 
	createSmoke_(true),	createFlame_(true), 
	spinSpeed_(1.0f),
	flameLife_(1.0f), smokeLife_(4.0f),
	flameStartColor1_(0.9f, 0.0f, 0.0f), flameStartColor2_(1.0f, 0.2f, 0.2f),
	flameEndColor1_(0.95f, 0.9f, 0.2f), flameEndColor2_(1.0f, 1.0f, 0.3f)
{

}

WeaponProjectile::~WeaponProjectile()
{

}

bool WeaponProjectile::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	// Get the accessory under
	XMLNode *underNode = 0;
	accessoryNode->getNamedChild("under", underNode, false);
	if (underNode) under_ = true;

	// Get the spin
	accessoryNode->getNamedChild("spinspeed", spinSpeed_, false);

	// Get smoke life
	accessoryNode->getNamedChild("smokelife", smokeLife_, false);
	accessoryNode->getNamedChild("flamelife", flameLife_, false);

	// Get the smoke trails
	XMLNode *smokeNode = 0;
	accessoryNode->getNamedChild("showshotpath", smokeNode, false);
	if (smokeNode) showShotPath_ = true;

	// Get the end point
	XMLNode *endPointNode = 0;
	accessoryNode->getNamedChild("showendpoint", endPointNode, false);
	if (endPointNode) showEndPoint_ = true;
	
	// Get the apex point
	XMLNode *apexNode = 0;
	accessoryNode->getNamedChild("apexcollision", apexNode, false);
	if (apexNode) apexCollision_ = true;

	// Get the no smoke node
	XMLNode *noCreateSmokeNode = 0;
	accessoryNode->getNamedChild("nocreatesmoke", noCreateSmokeNode, false);
	if (noCreateSmokeNode) createSmoke_ = false;

	// Get the no smoke node
	XMLNode *noCreateFlameNode = 0;
	accessoryNode->getNamedChild("nocreateflame", noCreateFlameNode, false);
	if (noCreateFlameNode) createFlame_ = false;

	// Get the next weapon
	XMLNode *subNode = 0;
	if (!accessoryNode->getNamedChild("collisionaction", subNode)) return false;

	// Check next weapon is correct type
	AccessoryPart *accessory = store->createAccessoryPart(context, parent_, subNode);
	if (!accessory || accessory->getType() != AccessoryPart::AccessoryWeapon)
	{
		dialogMessage("Accessory",
			"Sub weapon of wrong type \"%s\"",
			accessory->getAccessoryTypeName());
		return false;
	}
	collisionAction_ = (Weapon*) accessory;

	return true;
}

void WeaponProjectile::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	Action *action = new ShotProjectile(
		position, 
		velocity,
		this, 
		playerId, 
		0, // FlareType
		data); 
	context.actionController->addAction(action);	
}

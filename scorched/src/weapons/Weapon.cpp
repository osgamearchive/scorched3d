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

#include <weapons/Weapon.h>
#include <weapons/AccessoryStore.h>
#include <tankgraph/TankModelRenderer.h>
#include <tankgraph/MissileMesh.h>
#include <common/OptionsDisplay.h>

Weapon::Weapon() : 
	scale_(1.0f), muzzleFlash_(true)
{

}

Weapon::~Weapon()
{

}

bool Weapon::parseXML(OptionsGame &context, AccessoryStore *store, XMLNode *accessoryNode)
{
	// Get the muzzleflash
	XMLNode *muzzleFlashNode = 0;
	accessoryNode->getNamedChild("nomuzzleflash", muzzleFlashNode, false);
	if (muzzleFlashNode) muzzleFlash_ = false;

	// Get the weapon model scale
	accessoryNode->getNamedChild("projectilescale", scale_, false);

	// Get the weapon model
	XMLNode *modelNode = 0;
	if (accessoryNode->getNamedChild("projectilemodel", modelNode, false))
	{
		if (!modelId_.initFromNode("data/accessories", modelNode)) return false;
	}

	return true;
}

ModelID &Weapon::getModelID()
{
	return modelId_;
}

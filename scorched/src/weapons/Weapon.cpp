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

std::map<std::string, MissileMesh *> Weapon::loadedMeshes_;

Weapon::Weapon() : deathAnimationWeight_(0),
	scale_(1.0f), muzzleFlash_(true)
{

}

Weapon::~Weapon()
{

}

bool Weapon::parseXML(XMLNode *accessoryNode)
{
	if (!Accessory::parseXML(accessoryNode)) return false;

	// Get the deathWeight
	accessoryNode->getNamedChild("deathanimationweight", deathAnimationWeight_, false);

	// Get the explosion texture
	if (accessoryNode->getNamedChild("firedsound", firedSound_, false))
	{
		if (!checkDataFile("data/wav/%s", getFiredSound())) return false;
	}

	// Get the weapon model scale
	accessoryNode->getNamedChild("projectilescale", scale_, false);

	// Get the muzzleflash
	XMLNode *muzzleFlashNode = 0;
	accessoryNode->getNamedChild("nomuzzleflash", muzzleFlashNode, false);
	if (muzzleFlashNode) muzzleFlash_ = false;

	// Get the weapon model
	XMLNode *modelNode = 0;
	if (accessoryNode->getNamedChild("projectilemodel", modelNode, false))
	{
		if (!modelId_.initFromNode("data/accessories", modelNode)) return false;
	}

	return true;
}

bool Weapon::writeAccessory(NetBuffer &buffer)
{
	if (!Accessory::writeAccessory(buffer)) return false;
	buffer.addToBuffer(deathAnimationWeight_);
	buffer.addToBuffer(firedSound_);
	buffer.addToBuffer(scale_);
	buffer.addToBuffer(muzzleFlash_);
	if (!modelId_.writeModelID(buffer)) return false;
	return true;
}

bool Weapon::readAccessory(NetBufferReader &reader)
{
	if (!Accessory::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(deathAnimationWeight_)) return false;
	if (!reader.getFromBuffer(firedSound_)) return false;
	if (!reader.getFromBuffer(scale_)) return false;
	if (!reader.getFromBuffer(muzzleFlash_)) return false;
	if (!modelId_.readModelID(reader)) return false;
	return true;
}

ModelID &Weapon::getModelID()
{
	return modelId_;
}

const char *Weapon::getFiredSound()
{
	if (!firedSound_.c_str()[0]) return 0;
	return firedSound_.c_str();
}

MissileMesh *Weapon::getWeaponMesh(Tank *currentPlayer)
{
	// Set the default model to use if neither the tank
	// or weapon have one
	static ModelID defaultModelId;
	if (!defaultModelId.modelValid())
	{
		defaultModelId.initFromString(
			"MilkShape",
			"data/accessories/v2missile/v2missile.txt",
			"");
	}

	// Set the model to use as the default model id
	ModelID *usedModelId = &defaultModelId;

	// Get the model to use from the weapon (if there is one)
	if (getModelID().modelValid())
	{
		usedModelId = &getModelID();
	}
	else
	{
		// The weapon does not have a model defined for it
		// check the player to see if they have a default model
		if (currentPlayer)
		{
			TankModelRenderer *model = (TankModelRenderer *) 
					currentPlayer->getModel().getModelIdRenderer();
			if (model && 
				model->getModel()->getProjectileModelID().modelValid())
			{
				usedModelId = &model->getModel()->getProjectileModelID();
			}
		}
	}

	// Load or find the correct missile mesh
	MissileMesh *mesh = 0;
	const char *name = usedModelId->getStringHash();
	std::map<std::string, MissileMesh *>::iterator itor =
		loadedMeshes_.find(name);
	if (itor == loadedMeshes_.end())
	{
		usedModelId->clearCachedFile();
		ModelsFile *newFile = usedModelId->getModelsFile();
		if (!newFile) return 0;

		mesh = new MissileMesh(*newFile, 
			!OptionsDisplay::instance()->getNoSkins(), 1.0f);
		loadedMeshes_[name] = mesh;
	}
	else
	{
		// Find
		mesh = (*itor).second;
	}
	return mesh;
}

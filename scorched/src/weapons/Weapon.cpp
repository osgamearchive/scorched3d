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
#include <client/ScorchedClient.h>
#include <common/OptionsDisplay.h>

std::map<std::string, MissileMesh *> Weapon::loadedMeshes_;

Weapon::Weapon() : deathAnimationWeight_(0), explosionTexture_("exp00"), scale_(1.0f), shake_(0.0f)
{

}

Weapon::~Weapon()
{

}

bool Weapon::parseXML(XMLNode *accessoryNode)
{
	if (!Accessory::parseXML(accessoryNode)) return false;

	// Get the deathWeight
	XMLNode *deathWeight = accessoryNode->getNamedChild("deathanimationweight", false, true);
	if (deathWeight)
	{
		deathAnimationWeight_ = atoi(deathWeight->getContent());
	}

	// Get the explosion texture
	XMLNode *explosionTextureNode = accessoryNode->getNamedChild("explosiontexture", false, true);
	if (explosionTextureNode)
	{
		explosionTexture_ = explosionTextureNode->getContent();
	}

	// Get the explosion texture
	XMLNode *firedSoundNode = accessoryNode->getNamedChild("firedsound", false, true);
	if (firedSoundNode)
	{
		firedSound_ = firedSoundNode->getContent();
	}

	// Get the explosion texture
	XMLNode *explosionSoundNode = accessoryNode->getNamedChild("explosionsound", false, true);
	if (explosionSoundNode)
	{
		explosionSound_ = explosionSoundNode->getContent();
	}

	// Get the weapon model scale
	XMLNode *modelScaleNode = accessoryNode->getNamedChild("projectilescale", false, true);
	if (modelScaleNode)
	{
		scale_= (float) atof(modelScaleNode->getContent());
	}

	// Get the weapon model explosion shake
	XMLNode *modelShakeNode = accessoryNode->getNamedChild("explosionshake", false, true);
	if (modelShakeNode)
	{
		shake_ = (float) atof(modelShakeNode->getContent());
	}

	// Get the weapon model
	XMLNode *modelNode = accessoryNode->getNamedChild("projectilemodel", false, true);
	if (modelNode)
	{
		if (!modelId_.initFromNode("data/accessories", modelNode)) return false;
	}

	return true;
}

bool Weapon::writeAccessory(NetBuffer &buffer)
{
	if (!Accessory::writeAccessory(buffer)) return false;
	buffer.addToBuffer(deathAnimationWeight_);
	buffer.addToBuffer(explosionTexture_);
	buffer.addToBuffer(firedSound_);
	buffer.addToBuffer(explosionSound_);
	buffer.addToBuffer(scale_);
	buffer.addToBuffer(shake_);
	if (!modelId_.writeModelID(buffer)) return false;
	return true;
}

bool Weapon::readAccessory(NetBufferReader &reader)
{
	if (!Accessory::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(deathAnimationWeight_)) return false;
	if (!reader.getFromBuffer(explosionTexture_)) return false;
	if (!reader.getFromBuffer(firedSound_)) return false;
	if (!reader.getFromBuffer(explosionSound_)) return false;
	if (!reader.getFromBuffer(scale_)) return false;
	if (!reader.getFromBuffer(shake_)) return false;
	if (!modelId_.readModelID(reader)) return false;
	return true;
}

ModelID &Weapon::getModelID()
{
	return modelId_;
}

bool Weapon::write(NetBuffer &buffer, Weapon *weapon)
{
	buffer.addToBuffer(weapon->getAccessoryId());
	return true;
}

Weapon *Weapon::read(NetBufferReader &reader)
{
	unsigned int weaponId;
	if (!reader.getFromBuffer(weaponId)) return 0;
	Accessory *accessory = 
		ScorchedClient::instance()->getAccessoryStore().
		findByAccessoryId(weaponId);
	if (!accessory || (accessory->getType() != Accessory::AccessoryWeapon)) 
	{
		return 0;
	}
	return ((Weapon *) accessory);
}

Vector &Weapon::getExplosionColor()
{
	static Vector white(1.0f, 1.0f, 1.0f);
	return white;
}

const char *Weapon::getExplosionTexture()
{
	return explosionTexture_.c_str();
}

const char *Weapon::getFiredSound()
{
	if (!firedSound_.c_str()[0]) return 0;
	return firedSound_.c_str();
}

const char *Weapon::getExplosionSound()
{
	if (!explosionSound_.c_str()[0]) return 0;
	return explosionSound_.c_str();
}

float Weapon::getScale()
{
	return scale_;
}

float Weapon::getShake()
{
	return shake_;
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

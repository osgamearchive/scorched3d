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
	XMLNode *deathWeight = accessoryNode->getNamedChild("deathanimationweight");
	if (deathWeight)
	{
		deathAnimationWeight_ = atoi(deathWeight->getContent());
	}

	// Get the explosion texture
	XMLNode *explosionTextureNode = accessoryNode->getNamedChild("explosiontexture");
	if (explosionTextureNode)
	{
		explosionTexture_ = explosionTextureNode->getContent();
	}

	// Get the explosion texture
	XMLNode *firedSoundNode = accessoryNode->getNamedChild("firedsound");
	if (firedSoundNode)
	{
		firedSound_ = firedSoundNode->getContent();
	}

	// Get the explosion texture
	XMLNode *explosionSoundNode = accessoryNode->getNamedChild("explosionsound");
	if (explosionSoundNode)
	{
		explosionSound_ = explosionSoundNode->getContent();
	}

	// Get the weapon model scale
	XMLNode *modelScaleNode = accessoryNode->getNamedChild("projectilescale");
	if (modelScaleNode)
	{
		scale_= (float) atof(modelScaleNode->getContent());
	}

	// Get the weapon model explosion shake
	XMLNode *modelShakeNode = accessoryNode->getNamedChild("explosionshake");
	if (modelShakeNode)
	{
		shake_ = (float) atof(modelShakeNode->getContent());
	}

	// Get the weapon model
	XMLNode *modelNode = accessoryNode->getNamedChild("projectilemodel");
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
		AccessoryStore::instance()->findByAccessoryId(weaponId);
	if (!accessory || (accessory->getType() != Accessory::AccessoryWeapon)) 
	{
		return 0;
	}
	return ((Weapon *) accessory);
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

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

#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLTexture.h>
#include <weapons/Accessory.h>
#include <common/Defines.h>
#include <stdlib.h>

unsigned int Accessory::nextAccessoryId_ = 0;

Accessory::Accessory() :
	name_("NONAME"), description_("NODESC"), toolTip_("", ""),
	price_(0), bundle_(0), armsLevel_(0), accessoryId_(++nextAccessoryId_),
	primary_(false), purchasable_(true), texture_(0), store_(0)
{

}

Accessory::~Accessory()
{

}

bool Accessory::parseXML(XMLNode *accessoryNode)
{
	// Get the accessory name
	if (!accessoryNode->getNamedChild("name", name_)) return false;

	// Get the accessory armslevel
	if (!accessoryNode->getNamedChild("armslevel", armsLevel_)) return false;

	// Get the accessory description
	accessoryNode->getNamedChild("description", description_, false);
	toolTip_.setText(getName(), getDescription());

	// Get the accessory icon
	accessoryNode->getNamedChild("icon", iconName_, false);

	// Get the accessory sound 
	accessoryNode->getNamedChild("activationsound", activationSound_, false);

	// Get the accessory bundle
	bundle_ = -1;
	accessoryNode->getNamedChild("bundlesize", bundle_, false);

	// Get the accessory cost
	price_ = -1;
	accessoryNode->getNamedChild("cost", price_, false);

	// Get the purchasable
	XMLNode *nonpurchasable = 0;
	accessoryNode->getNamedChild("nonpurchasable", nonpurchasable, false);
	if (nonpurchasable) purchasable_ = false;

	sellPrice_ = 0;
	if (price_ > 0 && bundle_ > 0) sellPrice_ = int((price_ / bundle_) * 0.8f);
	originalPrice_ = price_;
	originalSellPrice_ = sellPrice_;

	return true;
}

bool Accessory::writeAccessory(NetBuffer &buffer)
{
	buffer.addToBuffer(name_);
	buffer.addToBuffer(description_);
	buffer.addToBuffer(iconName_);
	buffer.addToBuffer(activationSound_);
	buffer.addToBuffer(price_);
	buffer.addToBuffer(originalPrice_);
	buffer.addToBuffer(bundle_);
	buffer.addToBuffer(armsLevel_);
	buffer.addToBuffer(sellPrice_);
	buffer.addToBuffer(originalSellPrice_);
	buffer.addToBuffer(accessoryId_);
	buffer.addToBuffer(primary_);
	buffer.addToBuffer(purchasable_);
	return true;
}

bool Accessory::readAccessory(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(name_)) return false;
	if (!reader.getFromBuffer(description_)) return false;
	if (!reader.getFromBuffer(iconName_)) return false;
	if (!reader.getFromBuffer(activationSound_)) return false;
	if (!reader.getFromBuffer(price_)) return false;
	if (!reader.getFromBuffer(originalPrice_)) return false;
	if (!reader.getFromBuffer(bundle_)) return false;
	if (!reader.getFromBuffer(armsLevel_)) return false;
	if (!reader.getFromBuffer(sellPrice_)) return false;
	if (!reader.getFromBuffer(originalSellPrice_)) return false;
	if (!reader.getFromBuffer(accessoryId_)) return false;
	if (!reader.getFromBuffer(primary_)) return false;
	if (!reader.getFromBuffer(purchasable_)) return false;
	toolTip_.setText(getName(), getDescription());
	return true;
}

const char *Accessory::getActivationSound()
{
	if (!activationSound_.c_str()[0]) return 0;
	return activationSound_.c_str();
}

const char *Accessory::getName()
{ 
	return name_.c_str(); 
}

const char *Accessory::getDescription()
{
	return description_.c_str();
}

const int Accessory::getPrice() 
{ 
	return price_; 
}

const int Accessory::getSellPrice() 
{ 
	return sellPrice_; 
}

const int Accessory::getOriginalSellPrice()
{
	return originalSellPrice_;
}

const int Accessory::getOriginalPrice()
{
	return originalPrice_;
}

const int Accessory::getBundle() 
{ 
	return bundle_; 
}

const int Accessory::getArmsLevel() 
{ 
	return armsLevel_; 
}

bool Accessory::singular()
{
	return false;
}

GLWTip &Accessory::getToolTip()
{ 
	return toolTip_; 
}

std::map<std::string, Accessory *> *AccessoryMetaRegistration::accessoryMap = 0;

void AccessoryMetaRegistration::addMap(const char *name, Accessory *accessory)
{
	if (!accessoryMap) accessoryMap = new std::map<std::string, Accessory *>;

	std::map<std::string, Accessory *>::iterator itor = 
		accessoryMap->find(name);
	DIALOG_ASSERT(itor == accessoryMap->end());

	(*accessoryMap)[name] = accessory;
}

Accessory *AccessoryMetaRegistration::getNewAccessory(const char *name, AccessoryStore *store)
{
	std::map<std::string, Accessory *>::iterator itor = 
		accessoryMap->find(name);
	if (itor == accessoryMap->end()) return 0;

	Accessory *newAccessory = (*itor).second->getAccessoryCopy();
	newAccessory->setAccessoryStore(store);
	return newAccessory;
}

GLTexture *Accessory::getTexture()
{
	if (texture_) return texture_;

	GLTexture *texture = 0;
	if (getIconName()[0])
	{
		GLBitmap bmap(getDataFile("data/textures/wicons/%s", getIconName()), true);
		texture = new GLTexture();
		texture->create(bmap, GL_RGBA, false);
	}
	texture_ = texture;
	return texture;
}

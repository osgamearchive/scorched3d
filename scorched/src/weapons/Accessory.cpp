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

#include <weapons/Accessory.h>
#include <common/Defines.h>
#include <stdlib.h>

unsigned int Accessory::nextAccessoryId_ = 0;

Accessory::Accessory() :
	name_("NONAME"), description_("NODESC"), toolTip_("", ""),
	price_(0), bundle_(0), armsLevel_(0), accessoryId_(++nextAccessoryId_),
	primary_(false)
{

}

Accessory::~Accessory()
{

}

bool Accessory::parseXML(XMLNode *accessoryNode)
{
	// Get the accessory name
	XMLNode *nameNode = accessoryNode->getNamedChild("name");
	if (!nameNode)
	{
		dialogMessage("Accessory",
			"Failed to find name node");
		return false;
	}
	name_ = nameNode->getContent();

	// Get the accessory description
	XMLNode *descriptionNode = accessoryNode->getNamedChild("description");
	if (!descriptionNode)
	{
		dialogMessage("Accessory",
			"Failed to find description node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	description_ = descriptionNode->getContent();
	toolTip_.setText(getName(), getDescription());

	// Get the accessory icon
	XMLNode *iconNode = accessoryNode->getNamedChild("icon");
	if (iconNode)
	{
		iconName_ = iconNode->getContent();
	}

	// Get the accessory icon
	XMLNode *activationSoundNode = accessoryNode->getNamedChild("activationsound");
	if (activationSoundNode)
	{
		activationSound_ = activationSoundNode->getContent();
	}

	// Get the accessory bundle
	XMLNode *bundleNode = accessoryNode->getNamedChild("bundlesize");
	if (!bundleNode)
	{
		dialogMessage("Accessory",
			"Failed to find bundlesize node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	bundle_ = atoi(bundleNode->getContent());

	// Get the accessory cost
	XMLNode *costNode = accessoryNode->getNamedChild("cost");
	if (!costNode)
	{
		dialogMessage("Accessory",
			"Failed to find cost node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	price_ = atoi(costNode->getContent());

	// Get the accessory armslevel
	XMLNode *armsLevelNode = accessoryNode->getNamedChild("armslevel");
	if (!armsLevelNode)
	{
		dialogMessage("Accessory",
			"Failed to find armslevel node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	armsLevel_ = atoi(armsLevelNode->getContent());

	sellPrice_ = 0;
	if (price_ > 0 && bundle_ > 0) sellPrice_ = int((price_ / bundle_) * 0.8f);

	return true;
}

bool Accessory::writeAccessory(NetBuffer &buffer)
{
	buffer.addToBuffer(name_);
	buffer.addToBuffer(description_);
	buffer.addToBuffer(iconName_);
	buffer.addToBuffer(activationSound_);
	buffer.addToBuffer(price_);
	buffer.addToBuffer(bundle_);
	buffer.addToBuffer(armsLevel_);
	buffer.addToBuffer(sellPrice_);
	buffer.addToBuffer(accessoryId_);
	buffer.addToBuffer(primary_);
	return true;
}

bool Accessory::readAccessory(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(name_)) return false;
	if (!reader.getFromBuffer(description_)) return false;
	if (!reader.getFromBuffer(iconName_)) return false;
	if (!reader.getFromBuffer(activationSound_)) return false;
	if (!reader.getFromBuffer(price_)) return false;
	if (!reader.getFromBuffer(bundle_)) return false;
	if (!reader.getFromBuffer(armsLevel_)) return false;
	if (!reader.getFromBuffer(sellPrice_)) return false;
	if (!reader.getFromBuffer(accessoryId_)) return false;
	if (!reader.getFromBuffer(primary_)) return false;
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

Accessory *AccessoryMetaRegistration::getNewAccessory(const char *name)
{
	std::map<std::string, Accessory *>::iterator itor = 
		accessoryMap->find(name);
	if (itor == accessoryMap->end()) return 0;
	return (*itor).second->getAccessoryCopy();
}


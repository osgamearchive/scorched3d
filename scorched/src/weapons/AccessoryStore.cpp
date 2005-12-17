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

#include <XML/XMLFile.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <common/OptionsParam.h>
#include <weapons/AccessoryStore.h>
#include <weapons/Weapon.h>
#include <math.h>
#include <stdio.h>

AccessoryStore::AccessoryStore() : muzzleFlash_(0)
{

}

AccessoryStore::~AccessoryStore()
{

}

bool AccessoryStore::parseFile(
	OptionsGame &context, ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Loading weapons");

	const char *fileName = getDataFile("data/accessories.xml");
	clearAccessories();

	XMLFile file;
	if (!file.readFile(fileName))
	{
		dialogMessage("AccessoryStore", 
					  "Failed to parse \"%s\"\n%s", 
					  fileName,
					  file.getParserError());
		return false;
	}

	// Check file exists
	if (!file.getRootNode())
	{
		dialogMessage("AccessoryStore",
					"Failed to find accessory file \"%s\"",
					  fileName);
		return false;		
	}

	// Itterate all of the accessories in the file
	int noChildren = file.getRootNode()->getChildren().size();
	int childCount = 0;
	XMLNode *currentNode = 0;
	while (file.getRootNode()->getNamedChild("accessory", currentNode, false))
	{
		if (counter) counter->setNewPercentage(
			float(++childCount) / float(noChildren) * 100.0f);

		// Parse the accessory
		Accessory *accessory = new Accessory();
		if (!accessory->parseXML(context, this, currentNode))
		{
			return currentNode->returnError(
				formatString("Failed to create accessory \"%s\"",
				accessory->getName()));
		}

		// Check uniqueness
		if (findByPrimaryAccessoryName(accessory->getName()))
		{
			return currentNode->returnError(
				formatString("Accessory \"%s\" already exists",
				accessory->getName()));
		}

		// Add the accessory
		accessories_.push_back(accessory);

		// Add weapons to death animations, weighted by arms level
		if (accessory->getAction()->getType() == AccessoryPart::AccessoryWeapon)
		{
			Weapon *weapon = (Weapon *) accessory->getAction();
			if (0 == strcmp(accessory->getName(), "WeaponMuzzle"))
			{
				muzzleFlash_ = weapon;
			}
			else if (0 == strcmp(accessory->getName(), "WeaponDeathAnimation"))
			{
				deathAnimation_ = weapon;
			}
		}	

		// Add to the map so references can find it
		parsingNodes_[accessory->getName()] = currentNode;
	}

	if (!muzzleFlash_)
	{
		return file.getRootNode()->returnError(
			"Failed to find WeaponMuzzle weapon used for muzzle flash.");
	}
	if (!deathAnimation_)
	{
		return file.getRootNode()->returnError(
			"Failed to find WeaponDeathAnimation weapon used for tank explosions.");
	}

	// Clear mapping as it now contains invalid pointers
	parsingNodes_.clear();
	return file.getRootNode()->failChildren();
}

AccessoryPart *AccessoryStore::createAccessoryPart(
	OptionsGame &context, 
	Accessory *parent, XMLNode *currentNode)
{
	XMLNode *typeNode = 0;
	if (!currentNode->getNamedParameter("type", typeNode)) return false;

	AccessoryPart *accessoryPart = 
		AccessoryMetaRegistration::getNewAccessory(typeNode->getContent(), this);
	if (!accessoryPart)
	{
		dialogMessage("AccessoryStore",
						"Failed to find accessory part type \"%s\"",
						typeNode->getContent());
		return 0;
	}
	// Set the parent accessory
	accessoryPart->setParent(parent);
	
	// Tell this accessory instance to initialize its settings from
	// the current accessory xml definition node
	if (!accessoryPart->parseXML(context, this, currentNode)) return 0;

	// There should not be any children left
	// Any that are, are children that have not been
	// handled by the parse routine
	if (!currentNode->failChildren()) return 0;
	DIALOG_ASSERT(accessoryPart->getParent());

	// Add the accessory
	accessoryParts_.push_back(accessoryPart);
	return accessoryPart;
}

void AccessoryStore::sortList(std::list<Accessory *> &accList)
{
	std::vector<Accessory *> accVector;
	std::list<Accessory *>::iterator itor;
	for (itor = accList.begin();
		itor != accList.end();
		itor++)
	{
		accVector.push_back(*itor);
	}

	// Crudely sort by name
	// stl sort method list is broken in visual c 6
	// bubble sort
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0; i<int(accVector.size())-1; i++)
		{
			if (strcmp(accVector[i]->getName(), accVector[i+1]->getName())<0)
			{
				// swap
				Accessory *tmp = accVector[i];
				accVector[i] = accVector[i+1];
				accVector[i+1] = tmp;
				changed = true;
				break;
			}
		}
	} 
	
	accList.clear();
	for (int i=0; i<int(accVector.size()); i++)
	{
		accList.push_back(accVector[i]);
	}
}

std::list<Accessory *> AccessoryStore::getAllWeapons(bool sort)
{
	std::list<Accessory *> result;
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++)
	{
		if ((*itor)->getAction()->getType() == AccessoryPart::AccessoryWeapon)
		{
			result.push_back(*itor);
		}
	}

	if (sort) sortList(result);
	return result;
}

std::list<Accessory *> AccessoryStore::getAllOthers(bool sort)
{
	std::list<Accessory *> result;
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++)
	{
		if ((*itor)->getAction()->getType() != AccessoryPart::AccessoryWeapon)
		{
			result.push_back(*itor);
		}
	}

	if (sort) sortList(result);
	return result;
}

std::list<Accessory *> AccessoryStore::getAllAccessories(bool sort)
{
	std::list<Accessory *> result;
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++)
	{
		result.push_back(*itor);
	}

	if (sort) sortList(result);
	return result;
}

Weapon *AccessoryStore::getMuzzelFlash()
{
	return muzzleFlash_;
}

Weapon *AccessoryStore::getDeathAnimation()
{
	return deathAnimation_;
}

Accessory *AccessoryStore::findByAccessoryType(AccessoryPart::AccessoryType type)
{
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		if (accessory->getAction()->getType() == type) return accessory;
	}
	return 0;
}

Accessory *AccessoryStore::findByPrimaryAccessoryName(const char *name)
{
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		if (strcmp(accessory->getName(), name) == 0)
		{
			return accessory;
		}
	}
	return 0;
}

Accessory *AccessoryStore::findByAccessoryId(unsigned int id)
{
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		if (accessory->getAccessoryId() == id)
		{
			return accessory;
		}
	}
	return 0;
}

AccessoryPart *AccessoryStore::findByAccessoryPartId(unsigned int id)
{
	std::list<AccessoryPart *>::iterator itor;
	for (itor = accessoryParts_.begin();
		itor != accessoryParts_.end();
		itor++)
	{
		AccessoryPart *accessoryPart = (*itor);
		if (accessoryPart->getAccessoryPartId() == id)
		{
			return accessoryPart;
		}
	}
	return 0;
}

void AccessoryStore::clearAccessories()
{
	AccessoryPart::resetAccessoryPartIds();
	Accessory::resetAccessoryIds();
	muzzleFlash_ = 0;
	deathAnimation_ = 0;
	while (!accessories_.empty())
	{
		Accessory *accessory = accessories_.front();
		accessories_.pop_front();
		delete accessory;
	}
	while (!accessoryParts_.empty())
	{
		AccessoryPart *accessoryPart = accessoryParts_.front();
		accessoryParts_.pop_front();
		delete accessoryPart;
	}
}

bool AccessoryStore::writeWeapon(NetBuffer &buffer, Weapon *weapon)
{
	buffer.addToBuffer(weapon->getAccessoryPartId());
	return true;
}

Weapon *AccessoryStore::readWeapon(NetBufferReader &reader)
{
	unsigned int weaponId;
	if (!reader.getFromBuffer(weaponId)) return 0;
	AccessoryPart *accessoryPart = findByAccessoryPartId(weaponId);
	if (accessoryPart->getAccessoryPartId() == weaponId &&
		accessoryPart->getType() == AccessoryPart::AccessoryWeapon)
	{
		return ((Weapon *) accessoryPart);
	}
	return 0;
}

bool AccessoryStore::writeEconomyToBuffer(NetBuffer &buffer)
{
	std::list<Accessory *> accessories = getAllAccessories();
	buffer.addToBuffer((int) accessories.size());

	std::list<Accessory *>::iterator itor;
	for (itor = accessories.begin();
		itor != accessories.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		buffer.addToBuffer(accessory->getAccessoryId());
		buffer.addToBuffer(accessory->getPrice());
		buffer.addToBuffer(accessory->getSellPrice());
	}
	return true;
}

bool AccessoryStore::readEconomyFromBuffer(NetBufferReader &reader)
{
	int noAccessories = 0;
	if (!reader.getFromBuffer(noAccessories)) return false;
	for (int a=0; a<noAccessories; a++)
	{
		unsigned int accessoryId = 0;
		int price = 0, sellPrice = 0;
		if (!reader.getFromBuffer(accessoryId)) return false;
		if (!reader.getFromBuffer(price)) return false;
		if (!reader.getFromBuffer(sellPrice)) return false;

		Accessory *accessory = findByAccessoryId(accessoryId);
		if (!accessory) return false;
		accessory->setPrice(price);
		accessory->setSellPrice(sellPrice);
	}
	return true;
}

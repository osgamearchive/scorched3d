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
#include <tank/TankAccessories.h>
#include <tank/TankType.h>
#include <tank/Tank.h>
#include <tankgraph/TankModelStore.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>

TankAccessories::TankAccessories(ScorchedContext &context) :
	context_(context),
	tankWeapon_(context),
	tankAuto_(context),
	tankBatteries_(context)
{
}

TankAccessories::~TankAccessories()
{
}

void TankAccessories::setTank(Tank *tank)
{
	tank_ = tank;
	tankWeapon_.setTank(tank);
	tankAuto_.setTank(tank);
	tankBatteries_.setTank(tank);
}

void TankAccessories::newMatch()
{
	accessories_.clear();

	tankWeapon_.newMatch();
	tankAuto_.newMatch();
	tankBatteries_.newMatch();

	// Add all the accessories the tank should start with
	// this is the accessories from the global accessories file
	// and also if give all accessories is set
	{
		std::list<Accessory *> accessories = 
			context_.accessoryStore->getAllAccessories();
		std::list<Accessory *>::iterator itor;
		for (itor = accessories.begin();
			itor != accessories.end();
			itor++)
		{
			Accessory *accessory = (*itor);
			if (accessory->getMaximumNumber() > 0)
			{
				int startingNumber = accessory->getStartingNumber();
				if (context_.optionsGame->getGiveAllWeapons())
				{
					startingNumber = -1;
				}

				if (startingNumber != 0)
				{
					add_(accessory, startingNumber, true);
				}
			}
		}
	}

	// Add all of the accessories that come from the tank's type
	{
		TankType *type = context_.tankModelStore->getTypeByName(
			tank_->getModelContainer().getTankTypeName());
		std::map<Accessory *, int> accessories = type->getAccessories();
		std::map<Accessory *, int>::iterator itor;
		for (itor = accessories.begin();
			itor != accessories.end();
			itor++)
		{
			Accessory *accessory = (*itor).first;
			int count = (*itor).second;

			add_(accessory, count, true);
		}
	}

	changed();
}

void TankAccessories::getAllAccessories(std::list<Accessory *> &result, bool sort)
{
	std::set<AccessoryPart::AccessoryType> types;
	std::map<Accessory *, int>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++)
	{
		Accessory *accessory = (*itor).first;
		AccessoryPart::AccessoryType type = accessory->getType();
		if (types.find(type) == types.end())
		{
			types.insert(type);
			getAllAccessoriesByType(type, result, false);
		}
	}

	if (sort) AccessoryStore::sortList(result);
}

void TankAccessories::getAllAccessoriesByType(
	AccessoryPart::AccessoryType type, 
	std::list<Accessory *> &result, bool sort)
{
	// Add all weapons ensuring they are in the
	// same order as in the store
	std::list<Accessory *> allAccessories = 
		context_.accessoryStore->getAllAccessories();
	std::list<Accessory *>::iterator itor;
	for (itor = allAccessories.begin();
		itor != allAccessories.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		if (accessory->getType() == type &&
			accessories_.find(accessory) != accessories_.end())
		{
			result.push_back(accessory);
		}
	}

	if (sort) AccessoryStore::sortList(result);
}

void TankAccessories::getAllAccessoriesByGroup(const char *groupName,
		std::list<Accessory *> &result, bool sort)
{
	// Add all weapons ensuring they are in the
	// same order as in the store
	std::list<Accessory *> allAccessories = 
		context_.accessoryStore->getAllAccessories();
	std::list<Accessory *>::iterator itor;
	for (itor = allAccessories.begin();
		itor != allAccessories.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		if (accessories_.find(accessory) != accessories_.end() &&
			0 == strcmp(accessory->getGroupName(), groupName))
		{
			result.push_back(accessory);
		}
	}

	if (sort) AccessoryStore::sortList(result);
}

int TankAccessories::getAccessoryCount(Accessory *accessory)
{
	std::map<Accessory *, int>::iterator foundAccessory =
		accessories_.find(accessory);
	int currentNumber = 0;
	if (foundAccessory != accessories_.end())
	{
		currentNumber = foundAccessory->second;
	}
	return currentNumber;
}

bool TankAccessories::accessoryAllowed(Accessory *accessory, int count)
{
	// Check if this tank type allows this accessory
	TankType *type = context_.tankModelStore->getTypeByName(
		tank_->getModelContainer().getTankTypeName());
	if (type->getAccessoryDisabled(accessory)) return false;

	// Check if this accessory is allowed at all
	if (accessory->getMaximumNumber() == 0) return false;
	int currentCount = getAccessoryCount(accessory);
	if (currentCount + count >
		accessory->getMaximumNumber())
	{
		return false;
	}

	// Check if this accessory exceeds the current arms level
	if (10 - accessory->getArmsLevel() > 
		context_.optionsTransient->getArmsLevel())
	{
		return false;
	}

	// Check if an infinite weapon is being bought twice
	if (currentCount == -1)
	{
		return false;
	}

	// Check if this is an ai only weapon
	if (accessory->getAIOnly())
	{
		if (tank_->getDestinationId() != 0)
		{
			return false;
		}
	}

	return true;
}

void TankAccessories::add(Accessory *accessory, int count, bool check)
{
	add_(accessory, count, check);
	changed();
}

void TankAccessories::add_(Accessory *accessory, int count, bool check)
{
	if (check)
	{
		// Check if this tank is allowed this accessory
		if (!accessoryAllowed(accessory, count)) return;
	}
	else
	{
		// Check if an infinite weapon is being bought twice
		if (getAccessoryCount(accessory) == -1)
		{
			return;
		}
	}

	// Add this accessory
	std::map<Accessory *, int>::iterator itor = accessories_.find(accessory);
	if (itor == accessories_.end() || count < 0)
	{
		accessories_[accessory] = count;
	}
	else
	{
		accessories_[accessory] += count;
	}
}

void TankAccessories::rm(Accessory *accessory, int count)
{
	std::map<Accessory *, int>::iterator itor = 
		accessories_.find(accessory);
	if (itor != accessories_.end())
	{
		if (accessories_[accessory] > 0)
		{
			accessories_[accessory] -= count;
			if (accessories_[accessory] <= 0)
			{
				accessories_.erase(accessory);
			}
		}
	}

	changed();
}

void TankAccessories::changed()
{
	// Tell the appropriate container that the count has changed
	tankAuto_.changed();
	tankWeapon_.changed();
	tankBatteries_.changed();
}

bool TankAccessories::writeMessage(NetBuffer &buffer, bool writeAccessories)
{
	if (writeAccessories)
	{
		// Add all the accessories that are sent to the client
		std::map<Accessory *, int>::iterator itor;
		buffer.addToBuffer((int) accessories_.size());
		for (itor = accessories_.begin();
			itor != accessories_.end();
			itor++)
		{
			buffer.addToBuffer((*itor).first->getAccessoryId());
			buffer.addToBuffer((*itor).second);
		}
	}
	else
	{
		// Don't let this client know about any accessories
		buffer.addToBuffer((int) 0);
	}

	if (!tankWeapon_.writeMessage(buffer, writeAccessories)) return false;
	if (!tankAuto_.writeMessage(buffer, writeAccessories)) return false;
	if (!tankBatteries_.writeMessage(buffer, writeAccessories)) return false;
	return true;
}

bool TankAccessories::readMessage(NetBufferReader &reader)
{
	std::set<Accessory *> coveredAccessories;

	// Check all the servers accessories exist
	int totalAccessories = 0;
	if (!reader.getFromBuffer(totalAccessories)) return false;
	for (int w=0; w<totalAccessories; w++)
	{
		unsigned int accessoryId = 0;
		int accessoryCount = 0;
		if (!reader.getFromBuffer(accessoryId)) return false;
		if (!reader.getFromBuffer(accessoryCount)) return false;

		Accessory *accessory = 
			context_.accessoryStore->findByAccessoryId(accessoryId);
		if (!accessoryId) return false;
		coveredAccessories.insert(accessory);

		int actualCount = getAccessoryCount(accessory);
		if (actualCount != accessoryCount)
		{
			accessories_[accessory] = accessoryCount;
		}
	}

	// Remove any accessories we have but the server does not
	std::map<Accessory *, int> accessoryCopy = accessories_;
	std::map<Accessory *, int>::iterator itor;
	for (itor = accessoryCopy.begin();
		itor != accessoryCopy.end();
		itor++)
	{
		Accessory *accessory = (*itor).first;
		if (coveredAccessories.find(accessory) == 
			coveredAccessories.end())
		{
			accessories_.erase(accessory);
		}
	}

	if (!tankWeapon_.readMessage(reader)) return false;
	if (!tankAuto_.readMessage(reader)) return false;
	if (!tankBatteries_.readMessage(reader)) return false;

	changed();
	return true;
}


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
#include <tank/TankModelStore.h>
#include <tank/TankModelContainer.h>
#include <tank/TankAccessories.h>
#include <tank/TankType.h>
#include <tank/Tank.h>
#include <target/TargetParachute.h>
#include <target/TargetShield.h>
#include <target/TargetLife.h>
#include <tankai/TankKeyboardControlUtil.h>
#include <common/OptionsScorched.h>
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
	clearAccessories();

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

void TankAccessories::clearAccessories()
{
	accessories_.clear();

	std::map<std::string, AccessoryList*>::iterator groupsItor;
	for (groupsItor = accessoryGroups_.begin();
		groupsItor != accessoryGroups_.end();
		groupsItor++)
	{
		delete (*groupsItor).second;
	}
	accessoryGroups_.clear();

	std::map<AccessoryPart::AccessoryType, AccessoryList*>::iterator typesItor;
	for (typesItor = accessoryTypes_.begin();
		typesItor != accessoryTypes_.end();
		typesItor++)
	{
		delete (*typesItor).second;
	}
	accessoryTypes_.clear();

}

void TankAccessories::getAllAccessories(std::list<Accessory *> &result)
{
	std::map<Accessory *, int>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++)
	{
		Accessory *accessory = (*itor).first;
		result.push_back(accessory);
	}
}

std::list<Accessory *> &TankAccessories::getAllAccessoriesByType(
	AccessoryPart::AccessoryType type)
{
	std::map<AccessoryPart::AccessoryType, AccessoryList*>::iterator itor =
		accessoryTypes_.find(type);
	if (itor != accessoryTypes_.end())
	{
		return *((*itor).second);
	}

	static std::list<Accessory *> emptyList;
	return emptyList;
}

std::list<Accessory *> &TankAccessories::getAllAccessoriesByGroup(
	const char *groupName)
{
	std::map<std::string, AccessoryList*>::iterator itor =
		accessoryGroups_.find(groupName);
	if (itor != accessoryGroups_.end())
	{
		return *((*itor).second);
	}

	static std::list<Accessory *> emptyList;
	return emptyList;
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
		if (!tank_->isTemp())
		{
			return false;
		}
	}
	
	// Check if this is a bot only weapon
	if (accessory->getBotOnly())
	{
		if(tank_->getDestinationId() != 0)
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

	// Add to groups
	{
		std::map<std::string, AccessoryList*>::iterator groupItor =
			accessoryGroups_.find(accessory->getGroupName());
		if (groupItor != accessoryGroups_.end())
		{
			(*groupItor).second->remove(accessory);
			(*groupItor).second->push_back(accessory);
		}
		else
		{
			AccessoryList *newList = new AccessoryList();
			accessoryGroups_[accessory->getGroupName()] = newList;
			newList->push_back(accessory);
		}
	}

	// Add to types
	{
		std::map<AccessoryPart::AccessoryType, AccessoryList*>::iterator typeItor =
			accessoryTypes_.find(accessory->getType());
		if (typeItor != accessoryTypes_.end())
		{
			(*typeItor).second->remove(accessory);
			(*typeItor).second->push_back(accessory);
		}
		else
		{
			AccessoryList *newList = new AccessoryList();
			accessoryTypes_[accessory->getType()] = newList;
			newList->push_back(accessory);
		}
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

				{ // Remove from groups
					std::map<std::string, AccessoryList*>::iterator groupItor =
						accessoryGroups_.find(accessory->getGroupName());
					if (groupItor != accessoryGroups_.end())
					{
						(*groupItor).second->remove(accessory);
					}
				}
				{ // Remove from types
					std::map<AccessoryPart::AccessoryType, AccessoryList*>::iterator typeItor =
						accessoryTypes_.find(accessory->getType());
					if (typeItor != accessoryTypes_.end())
					{
						(*typeItor).second->remove(accessory);
					}
				}
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
		// Write accessories in partid order
		std::map<unsigned int, int> ordered;
		std::map<unsigned int, int>::iterator itor2;
		std::map<Accessory *, int>::iterator itor;
		for (itor = accessories_.begin();
			itor != accessories_.end();
			itor++)
		{
			ordered[itor->first->getAccessoryId()] = itor->second;
		}

		// Add all the accessories that are sent to the client
		buffer.addToBuffer((int) ordered.size());
		for (itor2 = ordered.begin();
			itor2 != ordered.end();
			itor2++)
		{
			buffer.addToBuffer(itor2->first);
			buffer.addToBuffer(itor2->second);
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
	clearAccessories();

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
		if (!accessory)
		{
			return false;
		}

		add_(accessory, accessoryCount, false);
	}

	if (!tankWeapon_.readMessage(reader)) return false;
	if (!tankAuto_.readMessage(reader)) return false;
	if (!tankBatteries_.readMessage(reader)) return false;

	changed();
	return true;
}

void TankAccessories::activate(Accessory *accessory)
{
	DIALOG_ASSERT(!context_.serverMode);

#ifndef S3D_SERVER
	switch (accessory->getType())
	{
	case AccessoryPart::AccessoryParachute:
		TankKeyboardControlUtil::parachutesUpDown(
			tank_,
			(tank_->getParachute().getCurrentParachute()==accessory)?
			0:accessory->getAccessoryId());
		break;
	case AccessoryPart::AccessoryShield:
		TankKeyboardControlUtil::shieldsUpDown(
			tank_,
			(tank_->getShield().getCurrentShield()==accessory)?
			0:accessory->getAccessoryId());
		break;
	case AccessoryPart::AccessoryWeapon:
		getWeapons().setWeapon(accessory);
		break;
	case AccessoryPart::AccessoryBattery:
		if (tank_->getLife().getLife() < 
			tank_->getLife().getMaxLife())
		{
			TankKeyboardControlUtil::useBattery(
				tank_, accessory->getAccessoryId());
		}
		break;
	case AccessoryPart::AccessoryAutoDefense:
		default:
		break;
	}
#endif
}

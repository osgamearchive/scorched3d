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
#include <server/ScorchedServer.h>
#include <tankai/TankAIComputerBuyer.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <tank/Tank.h>
#include <math.h>

TankAIComputerBuyer::Entry::Entry(const Entry &other)
{
	(*this) = other;
}

TankAIComputerBuyer::Entry &TankAIComputerBuyer::Entry::operator=(const Entry &other)
{
	level = other.level;
	std::list<std::string>::const_iterator aitor;
	for (aitor = other.buyAccessories.begin();
			aitor != other.buyAccessories.end();
			aitor++)
	{
		buyAccessories.push_back(*aitor);
	}

	return *this;
}

TankAIComputerBuyer::TankAIComputerBuyer() : currentTank_(0)
{
}

TankAIComputerBuyer::~TankAIComputerBuyer()
{
}

bool TankAIComputerBuyer::parseConfig(AccessoryStore &store, XMLNode *node)
{
	// Weapons
	XMLNode *weaponNode = 0;
	while (node->getNamedChild("weapon", weaponNode, false))
	{
		std::string wtype, wname;
		int wlevel;
		if (!weaponNode->getNamedChild("name", wname)) return false;
		if (!weaponNode->getNamedChild("level", wlevel)) return false;
		if (!weaponNode->getNamedChild("type", wtype)) return false;

		if (!addAccessory(store, wname.c_str(),  wlevel)) return false;

		if (0 != strcmp(wtype.c_str(), "dig") &&
			0 != strcmp(wtype.c_str(), "explosionsmall") &&
			0 != strcmp(wtype.c_str(), "explosionlarge") &&
			0 != strcmp(wtype.c_str(), "defense"))
		{
			dialogExit("TankAIComputerBuyer",
				"Unknown ai weapon type \"%s\"\n"
				"Should be one of dig, defense, explosionsmall, explosionlarge", 
				wtype.c_str());
		}
		buyTypes_.insert(
			std::pair<std::string, std::string>(wtype, wname));
	}

	return node->failChildren();
}

void TankAIComputerBuyer::clearAccessories()
{
	buyEntries_.clear();
}

bool TankAIComputerBuyer::addAccessory(AccessoryStore &store, 
	const char *accessoryName,
	int buyLevel)
{
	Accessory *accessory = 
		store.findByPrimaryAccessoryName(accessoryName);
	if (!accessory)
	{
		dialogMessage("TankAIComputerBuyer", 
			"Failed to find accessory \"%s\"", accessoryName);
		return false;
	}

	std::list<Entry>::iterator itor;
	for (itor = buyEntries_.begin();
		itor != buyEntries_.end();
		itor++)
	{
		Entry &current = (*itor);
		if (current.level > buyLevel)
		{
			break;
		}

		if (current.level == buyLevel)
		{
			current.buyAccessories.push_back(accessoryName);
			return true;
		}
	}

	Entry newEntry;
	newEntry.level = buyLevel;
	newEntry.buyAccessories.push_back(accessoryName);

	if (itor == buyEntries_.end())
	{
		buyEntries_.push_back(newEntry);
	}
	else if (itor == buyEntries_.begin())
	{
		buyEntries_.push_front(newEntry);
	}
	else
	{
		itor--;
		buyEntries_.insert(itor, newEntry);
	}

	return true;
}

void TankAIComputerBuyer::buyAccessories(int no)
{
	for (int i=0; i<no; i++)
	{
		buyAccessory();
	}
}

void TankAIComputerBuyer::buyAccessory()
{
	std::list<Accessory *> buyList;

	// For each set of accessories
	std::list<Entry>::iterator itor;
	for (itor = buyEntries_.begin();
		itor != buyEntries_.end();
		itor++)
	{
		std::list<std::string>::iterator aitor;
		for (aitor = (*itor).buyAccessories.begin();
			aitor != (*itor).buyAccessories.end();
			aitor++)
		{
			std::string &name = (*aitor);
			Accessory *current = ScorchedServer::instance()->
				getAccessoryStore().findByPrimaryAccessoryName(name.c_str());
			if (!current)
			{
				dialogExit("TankAIComputerBuyer",
					"Failed to find accessory \"%s\"",
					name.c_str());
			}

			// Check if the tank has each accessory
			if (currentTank_->getAccessories().getAccessoryCount(current) == 0)
			{
				// It does not have this one yet
				buyList.push_back(current);
			}
		}

		// Do we need any accessories at this level
		if (!buyList.empty()) break;
	}

	// We don't need any weapons
	if (buyList.empty()) return;

	// Buy a random accessory on the list of possible
	// accessories
	int no = int(RAND * float(buyList.size()));
	int count = 0;
	std::list<Accessory *>::iterator aitor;
	for (aitor = buyList.begin();
		aitor != buyList.end();
		aitor++, count++)
	{
		Accessory *current = *aitor;
		if (count == no)
		{
			// Buy the accessory
			if (currentTank_->getScore().getMoney() >= current->getPrice())
			{
				currentTank_->getAccessories().add(current);
				currentTank_->getScore().setMoney(currentTank_->getScore().getMoney() - 
					current->getPrice());
			}
			break;
		}
	}
}

void TankAIComputerBuyer::dumpAccessories()
{
	Logger::log( "-------------");
	std::list<Entry>::iterator itor;
	for (itor = buyEntries_.begin();
		itor != buyEntries_.end();
		itor++)
	{
		std::list<std::string>::iterator itor2;
		for (itor2 = (*itor).buyAccessories.begin();
			itor2 != (*itor).buyAccessories.end();
			itor2++)
		{
			Logger::log( "%s", (*itor2).c_str());
		}
	}
	Logger::log( "-------------");
}

std::vector<Accessory *> TankAIComputerBuyer::getWeaponType(const char *type)
{
	std::vector<Accessory *> result;
	std::pair<std::multimap<std::string, std::string>::iterator,
		std::multimap<std::string, std::string>::iterator> findItor = 
		getTypes().equal_range(type);
	std::multimap<std::string, std::string>::iterator itor;
	for (itor = findItor.first;
		itor != findItor.second;
		itor++)
	{
		Accessory *accessory = ScorchedServer::instance()->
			getAccessoryStore().findByPrimaryAccessoryName(
				(*itor).second.c_str());
		if (currentTank_->getAccessories().getAccessoryCount(accessory) != 0)
		{
			result.push_back(accessory);
		}
	}
	return result;
}

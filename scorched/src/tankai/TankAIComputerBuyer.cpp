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


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <tankai/TankAIComputerBuyer.h>
#include <tank/Tank.h>
#include <math.h>

#define RAND ((float) rand() / (float) RAND_MAX)

TankAIComputerBuyer::Entry::Entry(const Entry &other)
{
	(*this) = other;
}

TankAIComputerBuyer::Entry &TankAIComputerBuyer::Entry::operator=(const Entry &other)
{
	level = other.level;
	std::list<Accessory *>::const_iterator aitor;
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

void TankAIComputerBuyer::clearAccessories()
{
	buyEntries_.clear();
}

void TankAIComputerBuyer::addAccessory(const char *accessoryName,
									int buyLevel)
{
	Accessory *accessory = AccessoryStore::instance()->findByAccessoryName(accessoryName);
	if (!accessory)
	{
		dialogMessage("TankAIComputerBuyer", 
			"ERROR: Failed to find accessory \"%s\"", accessoryName);
		exit(1);
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
			current.buyAccessories.push_back(accessory);
			return;
		}
	}

	Entry newEntry;
	newEntry.level = buyLevel;
	newEntry.buyAccessories.push_back(accessory);

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
		std::list<Accessory *>::iterator aitor;
		for (aitor = (*itor).buyAccessories.begin();
			aitor != (*itor).buyAccessories.end();
			aitor++)
		{
			// Check if the tank has each accessory
			if (!currentTank_->getAccessories().getAccessoryCount(*aitor))
			{
				// It does not have this one yet
				buyList.push_back(*aitor);
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

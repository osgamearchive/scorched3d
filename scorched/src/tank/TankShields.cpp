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

#include <tank/TankShields.h>
#include <weapons/AccessoryStore.h>
#include <common/OptionsGame.h>
#include <stdio.h>
#include <math.h>
#include <set>

TankShields::TankShields(ScorchedContext &context) :
	context_(context)
{
	reset();
}

TankShields::~TankShields()
{

}

void TankShields::reset()
{
	shields_.clear();
	newGame();

	std::list<Accessory *> accessories = 
		context_.accessoryStore->getAllOthers();
	std::list<Accessory *>::iterator itor;
	for (itor = accessories.begin();
		itor != accessories.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		if (accessory->getType() == AccessoryPart::AccessoryShield)
		{
			if (accessory->getMaximumNumber() > 0)
			{
				if (context_.optionsGame->getGiveAllWeapons() ||
					accessory->getStartingNumber() == -1)
				{ 
					addShield(accessory, -1);
				}
				else if (accessory->getStartingNumber() > 0)
				{
					addShield(accessory, accessory->getStartingNumber());
				}
			}
		}
	}
}

void TankShields::newGame()
{
	setCurrentShield(0);
}

void TankShields::setCurrentShield(Accessory *sh)
{
	std::map<Accessory *, int>::iterator itor = shields_.find(sh);
	if (itor != shields_.end())
	{
		power_ = 100.0f;
		currentShield_ = sh;
		rmShield(sh, 1);
	}
	else
	{
		currentShield_ = 0;
		power_ = 0;
	}
}

void TankShields::setShieldPower(float power)
{
	power_ = power;
	if (power_ <= 0.0f)
	{
		power_ = 0.0f;
		setCurrentShield(0);
	}
}

void TankShields::addShield(Accessory *sh, int count)
{
	DIALOG_ASSERT(sh->getType() == AccessoryPart::AccessoryShield);

	std::map<Accessory *, int>::iterator itor = shields_.find(sh);
	if (itor == shields_.end() || count < 0)
	{
		shields_[sh] = count;
	}
	else
	{
		shields_[sh] += count;
	}
}

void TankShields::rmShield(Accessory *sh, int count)
{
	std::map<Accessory *, int>::iterator itor = shields_.find(sh);
	if (itor != shields_.end())
	{
		if (shields_[sh] > 0)
		{
			shields_[sh] -= count;
			if (shields_[sh] <= 0)
			{
				shields_.erase(itor);
			}
		}
	}
}

int TankShields::getShieldCount(Accessory *shield)
{
	std::map<Accessory *, int>::iterator itor = shields_.find(shield);
	if (itor != shields_.end())
	{
		return (*itor).second;
	}
	return 0;
}

std::list<Accessory *> TankShields::getAllShields(bool sort)
{
	std::list<Accessory *> result;
	std::map<Accessory *, int>::iterator itor;
	for (itor = shields_.begin();
		itor != shields_.end();
		itor++)
	{
		result.push_back((*itor).first);
	}

	if (sort) AccessoryStore::sortList(result);
	return result;
}

bool TankShields::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(power_);
	buffer.addToBuffer((unsigned int)(currentShield_?currentShield_->getAccessoryId():0));

	std::map<Accessory *, int>::iterator itor;
	buffer.addToBuffer((int) shields_.size());
	for (itor = shields_.begin();
		itor != shields_.end();
		itor++)
	{
		buffer.addToBuffer((*itor).first->getAccessoryId());
		buffer.addToBuffer((*itor).second);
	}

	return true;
}

bool TankShields::readMessage(NetBufferReader &reader)
{
	unsigned int shieldId;
	if (!reader.getFromBuffer(power_)) return false;
	if (!reader.getFromBuffer(shieldId)) return false;
	if (shieldId == 0) currentShield_ = 0;
	else currentShield_ = context_.accessoryStore->findByAccessoryId(shieldId);

	std::set<Accessory *> coveredShields;

	int totalShields = 0;
	if (!reader.getFromBuffer(totalShields)) return false;
	for (int w=0; w<totalShields; w++)
	{
		int shieldCount = 0;
		if (!reader.getFromBuffer(shieldId)) return false;
		if (!reader.getFromBuffer(shieldCount)) return false;

		Accessory *shield = 
			context_.accessoryStore->findByAccessoryId(shieldId);
		if (!shield) return false;
		if (shield->getType() != AccessoryPart::AccessoryShield) return false;
		coveredShields.insert(shield);

		int actualCount = getShieldCount(shield);
		if (actualCount != shieldCount)
		{
			shields_[shield] = shieldCount;
		}
	}

	std::map<Accessory *, int> shieldCopy = shields_;
	std::map<Accessory *, int>::iterator itor;
	for (itor = shieldCopy.begin();
		itor != shieldCopy.end();
		itor++)
	{
		Accessory *shield = (*itor).first;
		std::set<Accessory *>::iterator findItor =
			coveredShields.find(shield);
		if (findItor == coveredShields.end())
		{
			shields_.erase(shield);
		}
	}

	return true;
}

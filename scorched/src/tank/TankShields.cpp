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


// TankShields.cpp: implementation of the TankShields class.
//
//////////////////////////////////////////////////////////////////////

#include <tank/TankShields.h>
#include <weapons/AccessoryStore.h>
#include <stdio.h>
#include <math.h>
#include <set>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TankShields::TankShields()
{
	newGame();
}

TankShields::~TankShields()
{

}

void TankShields::reset()
{
	shields_.clear();
	newGame();
}

void TankShields::newGame()
{
	setCurrentShield(0);
}

void TankShields::setCurrentShield(Shield *sh)
{
	std::map<Shield*, int>::iterator itor = shields_.find(sh);
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

void TankShields::addShield(Shield *sh, int count)
{
	std::map<Shield*, int>::iterator itor = shields_.find(sh);
	if (itor == shields_.end())
	{
		shields_[sh] = count;
	}
	else
	{
		shields_[sh] += count;
	}
}

void TankShields::rmShield(Shield *sh, int count)
{
	std::map<Shield*, int>::iterator itor = shields_.find(sh);
	if (itor != shields_.end())
	{
		shields_[sh] -= count;
		if (shields_[sh] <= 0)
		{
			shields_.erase(itor);
		}
	}
}

int TankShields::getShieldCount(Shield *shield)
{
	std::map<Shield*, int>::iterator itor = shields_.find(shield);
	if (itor != shields_.end())
	{
		return (*itor).second;
	}
	return 0;
}

bool TankShields::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(power_);
	buffer.addToBuffer(currentShield_?currentShield_->getName():"");

	std::map<Shield *, int>::iterator itor;
	buffer.addToBuffer(shields_.size());
	for (itor = shields_.begin();
		itor != shields_.end();
		itor++)
	{
		buffer.addToBuffer((*itor).first->getName());
		buffer.addToBuffer((*itor).second);
	}

	return true;
}

bool TankShields::readMessage(NetBufferReader &reader)
{
	std::string shieldName;
	if (!reader.getFromBuffer(power_)) return false;
	if (!reader.getFromBuffer(shieldName)) return false;
	currentShield_ = (Shield *)
		AccessoryStore::instance()->findByAccessoryName(shieldName.c_str());

	std::set<Shield *> coveredShields;

	int totalShields = 0;
	if (!reader.getFromBuffer(totalShields)) return false;
	for (int w=0; w<totalShields; w++)
	{
		std::string shieldName;
		int shieldCount = 0;
		if (!reader.getFromBuffer(shieldName)) return false;
		if (!reader.getFromBuffer(shieldCount)) return false;

		Shield *shield = (Shield *) 
			AccessoryStore::instance()->findByAccessoryName(shieldName.c_str());
		if (!shield) return false;
		coveredShields.insert(shield);

		int actualCount = getShieldCount(shield);
		if (actualCount < shieldCount)
		{
			addShield(shield, shieldCount - actualCount);
		}
		else if (actualCount > shieldCount)
		{
			rmShield(shield, actualCount - shieldCount);
		}
	}

	std::map<Shield *, int> shieldCopy = shields_;
	std::map<Shield *, int>::iterator itor;
	for (itor = shieldCopy.begin();
		itor != shieldCopy.end();
		itor++)
	{
		Shield *shield = (*itor).first;
		std::set<Shield *>::iterator findItor =
			coveredShields.find(shield);
		if (findItor == coveredShields.end())
		{
			rmShield(shield, getShieldCount(shield));
		}
	}

	return true;
}

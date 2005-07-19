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

#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <weapons/AccessoryStore.h>
#include <weapons/Weapon.h>
#include <tank/TankWeapon.h>
#include <engine/ScorchedContext.h>
#include <stdio.h>
#include <set>

TankWeapon::TankWeapon(ScorchedContext &context) : 
	currentWeapon_(0), context_(context)
{
}

TankWeapon::~TankWeapon()
{
}

void TankWeapon::reset()
{
	weapons_.clear();
	currentWeapon_ = 0;

	std::list<Accessory *> accessories = 
		context_.accessoryStore->getAllWeapons();
	std::list<Accessory *>::iterator itor;
	for (itor = accessories.begin();
		itor != accessories.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		if (accessory->getType() == AccessoryPart::AccessoryWeapon)
		{
			if (accessory->getMaximumNumber() > 0)
			{
				if (context_.optionsGame->getGiveAllWeapons() ||
					accessory->getStartingNumber() == -1)
				{
					addWeapon(accessory, -1);
				}
				else if (accessory->getStartingNumber() > 0)
				{
					addWeapon(accessory, accessory->getStartingNumber());
				}
			}
		}
	}
}

void TankWeapon::addWeapon(Accessory *wp, int count)
{
	DIALOG_ASSERT(wp->getType() == AccessoryPart::AccessoryWeapon);

	if (!currentWeapon_) setCurrentWeapon(wp);

	std::map<Accessory *, int>::iterator itor = weapons_.find(wp);
	if (itor == weapons_.end() || count < 0)
	{
		weapons_[wp] = count;
	}
	else
	{
		weapons_[wp] += count;
	}
}

void TankWeapon::rmWeapon(Accessory *wp, int count)
{
	std::map<Accessory *, int>::iterator itor = weapons_.find(wp);
	if (itor != weapons_.end())
	{
		if (weapons_[wp] > 0)
		{
			weapons_[wp] -= count;
			if (weapons_[wp] <= 0)
			{
				weapons_.erase(wp);

				if (wp == currentWeapon_)
				{
					if(!weapons_.empty())
					{
						setCurrentWeapon(weapons_.begin()->first);
					}
					else
					{
						setCurrentWeapon(0);
					}
				}
			}
		}
	}
}

bool TankWeapon::setWeapon(Accessory *wp)
{
	std::map<Accessory *, int>::iterator itor;
	for (itor = weapons_.begin();
		itor != weapons_.end();
		itor++)
	{
		if ((*itor).first == wp)
		{
			setCurrentWeapon(wp);
			return true;
		}
	}

	return false;
}

Accessory *TankWeapon::getCurrent()
{
	return currentWeapon_;
}

void TankWeapon::setCurrentWeapon(Accessory *wp)
{
	currentWeapon_ = wp;
}

void TankWeapon::nextWeapon()
{
	std::map<Accessory *, int>::iterator itor = weapons_.find(currentWeapon_);

	if (++itor == weapons_.end())
	{
		itor = weapons_.begin();
	}

	setCurrentWeapon(itor->first);
}

void TankWeapon::prevWeapon()
{
	std::map<Accessory *, int>::iterator itor = weapons_.find(currentWeapon_);

	if (itor == weapons_.begin())
	{
		itor = weapons_.end();
	}

	--itor;
	setCurrentWeapon(itor->first);
}

std::list<Accessory *> TankWeapon::getAllWeapons(bool sort)
{
	std::list<Accessory *> result;
	// Add all weapons ensuring they are in the
	// same order as in the store
	std::list<Accessory *> accessories = 
		context_.accessoryStore->getAllWeapons();
	std::list<Accessory *>::iterator itor;
	for (itor = accessories.begin();
		itor != accessories.end();
		itor++)
	{
		Accessory *weapon = (*itor);
		std::map<Accessory *, int>::iterator findItor =
			weapons_.find(weapon);
		if (findItor != weapons_.end())
		{
			result.push_back(weapon);
		}
	}
	
	if (sort) AccessoryStore::sortList(result);
	return result;
}

int TankWeapon::getWeaponCount(Accessory *weapon)
{
	std::map<Accessory *, int>::iterator foundWeapon =
		weapons_.find(weapon);
	int currentNumber = 0;
	if (foundWeapon != weapons_.end())
	{
		currentNumber = foundWeapon->second;
	}

	return currentNumber;
}

const char *TankWeapon::getWeaponString()
{
	static char buffer[256];
	int count = getWeaponCount(getCurrent());
	sprintf(buffer, ((count>0)?"%s (%i)":"%s (In)"),
		getCurrent()->getName(), count);
	return buffer;
}

bool TankWeapon::writeMessage(NetBuffer &buffer, bool writeAccessories)
{
	if (writeAccessories)
	{
		std::map<Accessory *, int>::iterator itor;
		buffer.addToBuffer((int) weapons_.size());
		for (itor = weapons_.begin();
			itor != weapons_.end();
			itor++)
		{
			Accessory *weapon = (*itor).first;
			int count = (*itor).second;
			buffer.addToBuffer(weapon->getAccessoryId());
			buffer.addToBuffer(count);
		}
	}
	else
	{
		buffer.addToBuffer((int) 0);
	}

	return true;
}

bool TankWeapon::readMessage(NetBufferReader &reader)
{
	std::set<Accessory *> coveredWeapons;

	int totalWeapons = 0;
	if (!reader.getFromBuffer(totalWeapons)) return false;
	for (int w=0; w<totalWeapons; w++)
	{
		unsigned int accessoryId = 0;
		int weaponCount = 0;
		if (!reader.getFromBuffer(accessoryId)) return false;
		if (!reader.getFromBuffer(weaponCount)) return false;

		Accessory *weapon = context_.accessoryStore->findByAccessoryId(accessoryId);
		if (!weapon) return false;
		if (weapon->getType() != AccessoryPart::AccessoryWeapon) return false;
		coveredWeapons.insert(weapon);

		int actualCount = getWeaponCount(weapon);
		if (actualCount != weaponCount)
		{
			weapons_[weapon] = weaponCount;
			if (!currentWeapon_) setCurrentWeapon(weapon);
		}
	}

	std::map<Accessory *, int> weaponCopy = weapons_;
	std::map<Accessory *, int>::iterator itor;
	for (itor = weaponCopy.begin();
		itor != weaponCopy.end();
		itor++)
	{
		Accessory *weapon = (*itor).first;
		std::set<Accessory *>::iterator findItor =
			coveredWeapons.find(weapon);
		if (findItor == coveredWeapons.end())
		{
			weapons_.erase(weapon);
			if (weapon == currentWeapon_)
			{
				DIALOG_ASSERT(!weapons_.empty());
				setCurrentWeapon(weapons_.begin()->first);
			}
		}
	}

	return true;
}

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
#include <server/ScorchedServer.h>
#include <engine/ScorchedContext.h>
#include <stdio.h>
#include <set>

TankWeapon::TankWeapon(ScorchedContext &context) : 
	currentWeapon_(0), context_(context)
{
	reset();
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
		if (accessory->getType() == Accessory::AccessoryWeapon)
		{
			if ((accessory->getPrice() == 0 && 
					accessory->getBundle() == 0) ||
					ScorchedServer::instance()->getOptionsGame().getGiveAllWeapons())
			{
				addWeapon((Weapon*) accessory, -1);
			}
		}
	}
}

void TankWeapon::addWeapon(Weapon *wp, int count)
{
	if (!currentWeapon_) setCurrentWeapon(wp);

	std::map<Weapon *, int>::iterator itor = weapons_.find(wp);
	if (itor == weapons_.end() || count < 0)
	{
		weapons_[wp] = count;
	}
	else
	{
		weapons_[wp] += count;
	}
}

void TankWeapon::rmWeapon(Weapon *wp, int count)
{
	std::map<Weapon *, int>::iterator itor = weapons_.find(wp);
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
					DIALOG_ASSERT(!weapons_.empty());
					setCurrentWeapon(weapons_.begin()->first);
				}
			}
		}
	}
}

bool TankWeapon::setWeapon(Weapon *wp)
{
	std::map<Weapon *, int>::iterator itor;
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

Weapon *TankWeapon::getCurrent()
{
	DIALOG_ASSERT(currentWeapon_);
	return currentWeapon_;
}

void TankWeapon::setCurrentWeapon(Weapon *wp)
{
	DIALOG_ASSERT(wp);
	currentWeapon_ = wp;
}

void TankWeapon::nextWeapon()
{
	std::map<Weapon *, int>::iterator itor = weapons_.find(currentWeapon_);

	if (++itor == weapons_.end())
	{
		itor = weapons_.begin();
	}

	setCurrentWeapon(itor->first);
}

void TankWeapon::prevWeapon()
{
	std::map<Weapon *, int>::iterator itor = weapons_.find(currentWeapon_);

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
	std::map<Weapon *, int>::iterator itor;
	for (itor = weapons_.begin();
		itor != weapons_.end();
		itor++)
	{
		result.push_back((*itor).first);
	}
	
	if (sort) AccessoryStore::sortList(result);
	return result;
}

int TankWeapon::getWeaponCount(Weapon *weapon)
{
	std::map<Weapon *, int>::iterator foundWeapon =
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

bool TankWeapon::writeMessage(NetBuffer &buffer)
{
	std::map<Weapon *, int>::iterator itor;
	buffer.addToBuffer((int) weapons_.size());
	for (itor = weapons_.begin();
		itor != weapons_.end();
		itor++)
	{
		buffer.addToBuffer((*itor).first->getAccessoryId());
		buffer.addToBuffer((*itor).second);
	}

	return true;
}

bool TankWeapon::readMessage(NetBufferReader &reader)
{
	std::set<Weapon *> coveredWeapons;

	int totalWeapons = 0;
	if (!reader.getFromBuffer(totalWeapons)) return false;
	for (int w=0; w<totalWeapons; w++)
	{
		unsigned int accessoryId = 0;
		int weaponCount = 0;
		if (!reader.getFromBuffer(accessoryId)) return false;
		if (!reader.getFromBuffer(weaponCount)) return false;

		Weapon *weapon = (Weapon *) 
			context_.accessoryStore->findByAccessoryId(accessoryId);
		if (!weapon) return false;
		coveredWeapons.insert(weapon);

		int actualCount = getWeaponCount(weapon);
		if (actualCount != weaponCount)
		{
			weapons_[weapon] = weaponCount;
			if (!currentWeapon_) setCurrentWeapon(weapon);
		}
	}

	std::map<Weapon *, int> weaponCopy = weapons_;
	std::map<Weapon *, int>::iterator itor;
	for (itor = weaponCopy.begin();
		itor != weaponCopy.end();
		itor++)
	{
		Weapon *weapon = (*itor).first;
		std::set<Weapon *>::iterator findItor =
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

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
#include <weapons/WeaponMoveTank.h>
#include <tank/TankWeapon.h>
#include <tank/TankContainer.h>
#include <engine/ScorchedContext.h>
#include <client/ScorchedClient.h>
#include <client/ClientState.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/MovementMap.h>
#include <dialogs/MessageDialog.h>

TankWeapon::TankWeapon(ScorchedContext &context) : 
	currentWeapon_(0), context_(context),
	tank_(0)
{
}

TankWeapon::~TankWeapon()
{
}

void TankWeapon::newMatch()
{
	setCurrentWeapon(0);
}

void TankWeapon::changed()
{
	int weaponCount = tank_->getAccessories().
		getAccessoryCount(currentWeapon_);
	if (weaponCount == 0 ||
		currentWeapon_ == 0)
	{
		std::list<Accessory *> &result =
			tank_->getAccessories().getAllAccessoriesByGroup("weapon");
		if (!result.empty())
		{
			setWeapon(result.front());
		}
		else
		{
			setCurrentWeapon(0);
		}
	}
}

bool TankWeapon::setWeapon(Accessory *wp)
{
	if (tank_->getAccessories().getAccessoryCount(wp) != 0)
	{
		setCurrentWeapon(wp);
		return true;
	}
	return false;
}

Accessory *TankWeapon::getCurrent()
{
	return currentWeapon_;
}

void TankWeapon::setCurrentWeapon(Accessory *wp)
{
	if (!context_.serverMode)
	{
		// Only show this information on this tanks client
		if (ScorchedClient::instance()->getTankContainer().getCurrentDestinationId() ==
			tank_->getDestinationId() &&
			ScorchedClient::instance()->getGameState().getState() == ClientState::StatePlaying)
		{

		// Turn off fuel display (if any)
		if (currentWeapon_ && 
			currentWeapon_->getPositionSelect() != Accessory::ePositionSelectNone)
		{
			Landscape::instance()->restoreLandscapeTexture();
		}
		// Turn on selection display (if any)
		if (wp &&
			wp->getPositionSelect() != Accessory::ePositionSelectNone)
		{
			if (wp->getPositionSelect() == Accessory::ePositionSelectFuel)
			{
				WeaponMoveTank *moveWeapon = (WeaponMoveTank *)
					context_.accessoryStore->findAccessoryPartByAccessoryId(
						wp->getAccessoryId(), "WeaponMoveTank");
				if (moveWeapon)
				{
					MovementMap mmap(
						context_.landscapeMaps->getGroundMaps().getMapWidth(),
						context_.landscapeMaps->getGroundMaps().getMapHeight());
					mmap.calculateForTank(
						tank_, 
						moveWeapon, 
						context_,
						!moveWeapon->getUseFuel());
					mmap.movementTexture();	
				}
			}
			else if (wp->getPositionSelect() == Accessory::ePositionSelectLimit)
			{
				MovementMap::limitTexture(tank_->getTargetPosition(), 
					wp->getPositionSelectLimit());
			}

			MessageDialog::instance()->clear();
			MessageDialog::instance()->addMessage(
				formatString("Click ground to activate %s", wp->getName()));
		}

		}
	}

	currentWeapon_ = wp;
}

void TankWeapon::nextWeapon()
{
	std::list<Accessory *> &result =
		tank_->getAccessories().getAllAccessoriesByType(
			AccessoryPart::AccessoryWeapon);
	context_.accessoryStore->sortList(result,
		OptionsDisplay::instance()->getSortAccessories());

	std::list<Accessory *>::iterator itor;
	for (itor = result.begin();
		itor != result.end();
		itor++)
	{
		if (currentWeapon_ == (*itor))
		{
			if (++itor == result.end())
			{
				itor = result.begin();
			}
			setCurrentWeapon(*itor);
			break;
		}
	}
}

void TankWeapon::prevWeapon()
{
	std::list<Accessory *> &result =
		tank_->getAccessories().getAllAccessoriesByType(
			AccessoryPart::AccessoryWeapon);
	context_.accessoryStore->sortList(result,
		OptionsDisplay::instance()->getSortAccessories());

	std::list<Accessory *>::iterator itor;
	for (itor = result.begin();
		itor != result.end();
		itor++)
	{
		if (currentWeapon_ == (*itor))
		{
			if (itor == result.begin())
			{
				itor = result.end();
			}

			--itor;
			setCurrentWeapon(*itor);
			break;
		}
	}
}

const char *TankWeapon::getWeaponString()
{
	if (!getCurrent()) return "";

	static char buffer[256];
	int count = tank_->getAccessories().getAccessoryCount(getCurrent());
	snprintf(buffer, 256, ((count>0)?"%s (%i)":"%s (In)"),
		getCurrent()->getName(), count);
	return buffer;
}

bool TankWeapon::writeMessage(NetBuffer &buffer, bool writeAccessories)
{
	return true;
}

bool TankWeapon::readMessage(NetBufferReader &reader)
{
	return true;
}

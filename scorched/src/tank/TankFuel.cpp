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

#include <tank/TankFuel.h>
#include <server/ScorchedServer.h>
#include <weapons/AccessoryStore.h>
#include <common/Defines.h>
#include <stdio.h>

TankFuel::TankFuel()
{
	reset();
}

TankFuel::~TankFuel()
{

}

void TankFuel::reset()
{
	fuelCount_ = 0;
	std::list<Accessory *> accessories = 
		AccessoryStore::instance()->getAllOthers();
	std::list<Accessory *>::iterator itor;
	for (itor = accessories.begin();
		itor != accessories.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		if (accessory->getType() == Accessory::AccessoryFuel)
		{
			if ((accessory->getPrice() == 0 && 
				accessory->getBundle() == 0) ||
				ScorchedServer::instance()->getOptionsGame().getGiveAllWeapons())
			{
				addFuel(ScorchedServer::instance()->getOptionsGame().
					getMaxNumberWeapons());
			}
		}
	}
}

void TankFuel::rmFuel(int no)
{
	fuelCount_ -= no;
	if (fuelCount_ < 0)
	{
		fuelCount_ = 0;
	}
}

void TankFuel::addFuel(int no)
{
	fuelCount_+=no;
}

bool TankFuel::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(fuelCount_);
	return true;
}

bool TankFuel::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(fuelCount_)) return false;
	return true;
}

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

#include <tank/TankParachutes.h>
#include <weapons/AccessoryStore.h>
#include <stdio.h>

TankParachutes::TankParachutes()
{
	reset();
}

TankParachutes::~TankParachutes()
{

}

void TankParachutes::setParachutesEnabled(bool enabled)
{
	if (enabled && (parachuteCount_ == -1 || parachuteCount_ > 0))
	{
		parachutesEnabled_ = true;
	}
	else
	{
		parachutesEnabled_ = false;
	}
}

void TankParachutes::reset()
{
	parachutesEnabled_ = false;
	parachuteCount_ = 0;
	parachuteThreshold_ = 0.0f;
	setParachutesEnabled(false);	

	std::list<Accessory *> accessories = 
		AccessoryStore::instance()->getAllWeapons();
	std::list<Accessory *>::iterator itor;
	for (itor = accessories.begin();
		itor != accessories.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		if (accessory->getType() == Accessory::AccessoryParachute &&
			accessory->getPrice() == 0 && 
			accessory->getBundle() == 0)
		{
			addParachutes(-1);
		}
	}
}

void TankParachutes::newGame()
{
	setParachutesEnabled(false);
}

void TankParachutes::useParachutes(int no)
{
	if (parachuteCount_ > 0)
	{
		parachuteCount_ -= no;
		if (parachuteCount_ > 0)
		{
			setParachutesEnabled(parachutesEnabled_);
		}
		else
		{
			parachuteCount_ = 0;
			setParachutesEnabled(false);
		}
	}
}

void TankParachutes::addParachutes(int no)
{
	parachuteCount_+=no;
	setParachutesEnabled(parachutesEnabled_);
}

bool TankParachutes::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(parachuteCount_);
	buffer.addToBuffer(parachutesEnabled_);
	return true;
}

bool TankParachutes::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(parachuteCount_)) return false;
	if (!reader.getFromBuffer(parachutesEnabled_)) return false;
	return true;
}

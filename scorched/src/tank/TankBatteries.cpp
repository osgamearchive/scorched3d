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

#include <tank/TankBatteries.h>
#include <weapons/AccessoryStore.h>
#include <common/Defines.h>
#include <stdio.h>

TankBatteries::TankBatteries(ScorchedContext &context) :
	context_(context), batteryCount_(0)
{
}

TankBatteries::~TankBatteries()
{
}

void TankBatteries::newMatch()
{
	batteryCount_ = 0;
}

void TankBatteries::rmBatteries(int no)
{
	if (batteryCount_ == -1) return;

	batteryCount_ -= no;
	if (batteryCount_ < 0)
	{
		batteryCount_ = 0;
	}
}

void TankBatteries::addBatteries(int no)
{
	batteryCount_+=no;
}

bool TankBatteries::writeMessage(NetBuffer &buffer, bool writeAccessories)
{
	if (writeAccessories) buffer.addToBuffer(batteryCount_);
	else buffer.addToBuffer((int) 0);
	return true;
}

bool TankBatteries::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(batteryCount_)) return false;
	return true;
}

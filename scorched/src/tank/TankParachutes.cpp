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

#include <tank/Tank.h>
#include <weapons/AccessoryStore.h>
#include <stdio.h>

TankParachutes::TankParachutes(ScorchedContext &context) :
	context_(context),
	parachuteCount_(0), 
	tank_(0)
{
}

TankParachutes::~TankParachutes()
{
}

void TankParachutes::newMatch()
{
	parachuteCount_ = 0;
}

void TankParachutes::useParachutes(int no)
{
	if (parachuteCount_ > 0)
	{
		parachuteCount_ -= no;
		if (parachuteCount_ <= 0)
		{
			parachuteCount_ = 0;
			tank_->getParachute().setParachutesEnabled(false);
		}
	}
}

void TankParachutes::addParachutes(int no)
{
	parachuteCount_+=no;
}

bool TankParachutes::writeMessage(NetBuffer &buffer, bool writeAccessories)
{
	if (writeAccessories)
	{
		buffer.addToBuffer(parachuteCount_);
	}
	else
	{
		buffer.addToBuffer((int) 0);
	}
	return true;
}

bool TankParachutes::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(parachuteCount_)) return false;
	return true;
}

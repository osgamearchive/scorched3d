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
#include <tank/TankAccessories.h>
#include <weapons/AccessoryStore.h>

TankParachutes::TankParachutes(ScorchedContext &context) :
	context_(context),
	tank_(0)
{
}

TankParachutes::~TankParachutes()
{
}

void TankParachutes::newMatch()
{
}

void TankParachutes::changed()
{
	if (getNoParachutes() == 0)
	{
		tank_->getParachute().setParachutesEnabled(false);
	}
}

int TankParachutes::getNoParachutes()
{
	std::list<Accessory *> result = 
		tank_->getAccessories().getAllAccessoriesByType(
			AccessoryPart::AccessoryParachute);
	if (result.empty()) return 0;
	return tank_->getAccessories().getAccessoryCount(result.front());
}

bool TankParachutes::writeMessage(NetBuffer &buffer, bool writeAccessories)
{
	return true;
}

bool TankParachutes::readMessage(NetBufferReader &reader)
{
	return true;
}

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

#include <tank/TankDeadContainer.h>

TankDeadContainer::TankDeadContainer() 
{

}

TankDeadContainer::~TankDeadContainer()
{

}

void TankDeadContainer::addTank(Tank *tank)
{
	Tank *old = getTank(tank->getUniqueId());
	if (old) delete old;

	deadTanks_[tank->getUniqueId()] = tank;
}

Tank *TankDeadContainer::getTank(const char *uniqueId)
{
	std::map<std::string, Tank *>::iterator finditor =
		deadTanks_.find(uniqueId);
	if (finditor != deadTanks_.end())
	{
		Tank *tank = (*finditor).second;
		deadTanks_.erase(finditor);
		return tank;
	}
	return 0;
}

void TankDeadContainer::clearTanks()
{
	std::map<std::string, Tank *>::iterator itor;
	for (itor = deadTanks_.begin();
		itor != deadTanks_.end();
		itor++)
	{
		Tank *current = (*itor).second;
		delete current;
	}
	deadTanks_.clear();
}


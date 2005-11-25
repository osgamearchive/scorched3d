////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <ships/ScorchedShips.h>
#include <common/OptionsDisplay.h>

ScorchedShips::ScorchedShips()
{
}

ScorchedShips::~ScorchedShips()
{
}

void ScorchedShips::generate()
{
	while (!groups_.empty())
	{
		ShipGroup *group = groups_.back();
		groups_.pop_back();
		delete group;
	}

	/*
	ShipGroup *group = new ShipGroup;
	group->generate();
	groups_.push_back(group);*/
}

void ScorchedShips::simulate(float frameTime)
{
	if (!OptionsDisplay::instance()->getNoShips())
	{
		std::vector<ShipGroup *>::iterator itor;
		for (itor = groups_.begin();
			itor != groups_.end();
			itor++)
		{
			ShipGroup *group = *itor;
			group->simulate(frameTime);
		}
	}
}

void ScorchedShips::draw()
{
	if (!OptionsDisplay::instance()->getNoShips())
	{
		std::vector<ShipGroup *>::iterator itor;
		for (itor = groups_.begin();
			itor != groups_.end();
			itor++)
		{
			ShipGroup *group = *itor;
			group->draw();
		}
	}
}

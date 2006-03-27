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
#include <landscape/LandscapeMaps.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeDefn.h>
#include <landscapedef/LandscapeShips.h>
#include <client/ScorchedClient.h>
#include <GLEXT/GLState.h>

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

	LandscapeTex &tex = 
		*ScorchedClient::instance()->getLandscapeMaps().
			getDefinitions().getTex();
	LandscapeDefn &defn = 
		*ScorchedClient::instance()->getLandscapeMaps().
			getDefinitions().getDefn();

	addShips(tex.texDefn.ships);
	addShips(defn.texDefn.ships);
}

void ScorchedShips::addShips(std::vector<LandscapeShips *> &ships)
{
	std::vector<LandscapeShips *>::iterator itor;
	for (itor = ships.begin();
		itor != ships.end();
		itor++)
	{
		LandscapeShips *ships = (*itor);
		addShipGroups(ships->objects);
	}
}

void ScorchedShips::addShipGroups(std::vector<LandscapeShipGroup *> &shipgroups)
{
	std::vector<LandscapeShipGroup *>::iterator itor;
	for (itor = shipgroups.begin();
		itor != shipgroups.end();
		itor++)
	{
		LandscapeShipGroup *texShipGroup = (*itor);
		ShipGroup *group = new ShipGroup;
		group->generate(texShipGroup);
		groups_.push_back(group);
	}
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
		GLState state(GLState::BLEND_ON | GLState::TEXTURE_ON | GLState::DEPTH_ON);
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

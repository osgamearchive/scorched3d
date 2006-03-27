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

#include <landscapedef/LandscapeTexDefn.h>
#include <landscapedef/LandscapeDefinitions.h>

LandscapeTexDefn::LandscapeTexDefn()
{
}

LandscapeTexDefn::~LandscapeTexDefn()
{
}

bool LandscapeTexDefn::readXML(LandscapeDefinitions *definitions, XMLNode *node)
{
	{
		XMLNode *eventsNode;
		if (node->getNamedChild("events", eventsNode, false))
		{
			std::string event;
			while (eventsNode->getNamedChild("event", event, false))
			{
				LandscapeEvents *landscapeEvents = 
					definitions->getEvents(event.c_str(), true);
				if (!landscapeEvents) return false;
				events.push_back(landscapeEvents);
			}
			if (!eventsNode->failChildren()) return false;
		}
	}
	{
		XMLNode *boidsNode;
		if (node->getNamedChild("boids", boidsNode, false))
		{
			std::string boid;
			while (boidsNode->getNamedChild("boid", boid, false))
			{
				LandscapeBoids *landscapeBoids = 
					definitions->getBoids(boid.c_str(), true);
				if (!landscapeBoids) return false;
				boids.push_back(landscapeBoids);
			}
			if (!boidsNode->failChildren()) return false;
		}
	}
	{
		XMLNode *shipgoupsNode;
		if (node->getNamedChild("shipgroups", shipgoupsNode, false))
		{
			std::string shipgroup;
			while (shipgoupsNode->getNamedChild("shipgroup", shipgroup, false))
			{
				LandscapeShips *landscapeShips = 
					definitions->getShips(shipgroup.c_str(), true);
				if (!landscapeShips) return false;
				ships.push_back(landscapeShips);
			}
			if (!shipgoupsNode->failChildren()) return false;
		}
	}
	{
		XMLNode *placementsNode;
		if (node->getNamedChild("placements", placementsNode, false))
		{
			std::string placement;
			while (placementsNode->getNamedChild("placement", placement, false))
			{
				LandscapePlace *landscapePlace = 
					definitions->getPlace(placement.c_str(), true);
				if (!landscapePlace) return false;
				placements.push_back(landscapePlace);
			}
			if (!placementsNode->failChildren()) return false;
		}
	}
	{
		XMLNode *soundsNode;
		if (node->getNamedChild("ambientsounds", soundsNode, false))
		{
			std::string sound;
			while (soundsNode->getNamedChild("ambientsound", sound, false))
			{
				LandscapeSound *landscapeSound = 
					definitions->getSound(sound.c_str(), true);
				if (!landscapeSound) return false;
				sounds.push_back(landscapeSound);
			}
			if (!soundsNode->failChildren()) return false;
		}
	}

	return true;
}

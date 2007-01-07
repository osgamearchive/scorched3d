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

#include <landscapedef/LandscapeInclude.h>

LandscapeInclude::LandscapeInclude()
{
}

LandscapeInclude::~LandscapeInclude()
{
	{
		while (!events.empty())
		{
			delete events.back();
			events.pop_back();
		}
	}
	{
		while (!movements.empty())
		{
			delete movements.back();
			movements.pop_back();
		}
	}
	{
		while (!placements.empty())
		{
			delete placements.back();
			placements.pop_back();
		}
	}
	{
		while (!sounds.empty())
		{
			delete sounds.back();
			sounds.pop_back();
		}
	}
}

bool LandscapeInclude::readXML(LandscapeDefinitions *definitions, XMLNode *node)
{
	{
		XMLNode *eventNode;
		while (node->getNamedChild("event", eventNode, false))
		{
			LandscapeEvent *event = new LandscapeEvent;
			if (!event->readXML(eventNode)) return false;
			events.push_back(event);
		}
	}
	{
		XMLNode *soundNode;
		while (node->getNamedChild("sound", soundNode, false))
		{
			LandscapeSoundType *sound = new LandscapeSoundType;
			if (!sound->readXML(soundNode)) return false;
			sounds.push_back(sound);
		}
	}
	{
		XMLNode *placementNode;
		while (node->getNamedChild("placement", placementNode, false))
		{
			std::string placementtype;
			PlacementType *placement = 0;
			if (!placementNode->getNamedParameter("type", placementtype)) return false;
			if (!(placement = PlacementType::create(placementtype.c_str()))) return false;
			if (!placement->readXML(placementNode)) return false;
			placements.push_back(placement);
		}
	}
	{
		XMLNode *movement;
		while (node->getNamedChild("movement", movement, false))
		{
			std::string type;
			if (!movement->getNamedParameter("type", type)) return false;
			LandscapeMovementType *object = LandscapeMovementType::create(type.c_str());
			if (!object) return false;
			if (!object->readXML(movement)) return false;
			movements.push_back(object);
		}
	}
	return node->failChildren();
}

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

#include <engine/EventContainer.h>

EventContainer::EventContainer()
{
}

EventContainer::~EventContainer()
{
}

void EventContainer::initialize(LandscapeTex *tex)
{
	events_.clear();
	std::vector<LandscapeTexEvent *> &events = tex->events;
	std::vector<LandscapeTexEvent *>::iterator itor;
	for (itor = events.begin();
		itor != events.end();
		itor++)
	{
		LandscapeTexEvent *event = (*itor);
		float nextTime = event->condition->getNextEventTime();
		events_[event] = nextTime;
	}
}

void EventContainer::simulate(float frameTime, ScorchedContext &context)
{
	std::map<LandscapeTexEvent *, float>::iterator itor;
	for (itor = events_.begin();
		itor != events_.end();
		itor++)
	{
		LandscapeTexEvent *event = (*itor).first;
		float &nextTime = (*itor).second;
		
		nextTime -= frameTime;
		if (nextTime < 0.0f)
		{
			event->action->fireAction(context);
			nextTime = event->condition->getNextEventTime();
		}
	}	
}

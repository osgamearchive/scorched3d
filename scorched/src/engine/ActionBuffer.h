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

#if !defined(__INCLUDE_ActionBufferh_INCLUDE__)
#define __INCLUDE_ActionBufferh_INCLUDE__

#include <engine/ActionMeta.h>
#include <list>
#include <map>

class ScorchedContext;
class ActionController;
class ActionBuffer
{
public:
	ActionBuffer();
	virtual ~ActionBuffer();

	void clear();
	bool empty();
	int size();

	void clientAdd(float time, ActionMeta *action);
	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);

protected:
	friend class ActionController;

	void serverAdd(float time, ActionMeta *action);
	ActionMeta *getActionForTime(float time);
	void setContext(ScorchedContext *c) { context_ = c; }

protected:
	NetBuffer actionBuffer_;
	std::list<std::pair<float, ActionMeta *> > actionList_;
	std::multimap<float, ActionMeta *> tmpOrderedList;
	ScorchedContext *context_;
	float clientTime_;

};


#endif

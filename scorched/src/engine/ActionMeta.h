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

#if !defined(__INCLUDE_ActionMetah_INCLUDE__)
#define __INCLUDE_ActionMetah_INCLUDE__

#include <coms/NetBuffer.h>
#include <engine/Action.h>
#include <engine/MetaClass.h>
#include <string>
#include <map>

#define REGISTER_ACTION_HEADER(x) REGISTER_CLASS_HEADER(x)
#define REGISTER_ACTION_SOURCE(x) REGISTER_CLASS_SOURCE(x)

class ActionMeta : public Action, public MetaClass
{
public:
	ActionMeta();
	virtual ~ActionMeta();

	virtual bool getReferenced() { return true; }
	virtual bool getServerOnly() { return true; }

	// Needs to be implemented by inherited actions
	virtual bool writeAction(NetBuffer &buffer) = 0;
	virtual bool readAction(NetBufferReader &reader) = 0;

	// Automatically given by the 
	// REGISTER_ACTION_HEADER and
	// REGISTER_ACTION_SOURCE macros
	//virtual const char *getClassName() = 0;
	//virtual MetaClass *getClassCopy() = 0;
};

#endif

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

#if !defined(__INCLUDE_ActionVectorh_INCLUDE__)
#define __INCLUDE_ActionVectorh_INCLUDE__

#include <engine/ActionMeta.h>
#include <list>
#include <map>

class ActionVector : public ActionMeta
{
public:
	ActionVector();
	ActionVector(unsigned int actionId);
	virtual ~ActionVector();

	void addPoint(unsigned int point);
	void addPointF(float point);
	unsigned int getPoint();
	float getPointF();
	bool empty() { return points_.empty(); }
	void remove() { remove_ = true; }

	virtual bool getReferenced() { return false; }
	virtual void simulate(float frameTime, bool &removeAction);
	virtual void init();

	// Needs to be implemented by inherited actions
	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);

	REGISTER_ACTION_HEADER(ActionVector);

protected:
	bool remove_;
	unsigned int actionId_;
	std::list<unsigned int> points_;
};

class ActionVectorHolder
{
public:
	static unsigned int getNextActionId() { return ++nextActionId_; }
	static ActionVector *getActionVector(unsigned int id);
	static void addActionVector(unsigned int, ActionVector *);

protected:
	static unsigned int nextActionId_;
	static std::map<unsigned int, ActionVector *> actions_;
};

#endif

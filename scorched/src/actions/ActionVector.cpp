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

#include <actions/ActionVector.h>

std::map<unsigned int, ActionVector *> ActionVectorHolder::actions_;
unsigned int ActionVectorHolder::nextActionId_ = 0;

REGISTER_ACTION_SOURCE(ActionVector);

ActionVector *ActionVectorHolder::getActionVector(unsigned int id)
{
	std::map<unsigned int, ActionVector *>::iterator itor =
		actions_.find(id);
	if (itor == actions_.end()) return 0;
	return (*itor).second;
}

void ActionVectorHolder::addActionVector(unsigned int id, ActionVector *action)
{
	actions_[id] = action;
}

ActionVector::ActionVector() :
	actionId_(0), remove_(false)
{
}

ActionVector::ActionVector(unsigned int actionId) :
	actionId_(actionId), remove_(false)
{
}

ActionVector::~ActionVector()
{

}

void ActionVector::init()
{
	ActionVectorHolder::addActionVector(actionId_, this);
}

void ActionVector::simulate(float frameTime, bool &removeAction)
{
	if (remove_) removeAction = true;
}

bool ActionVector::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(actionId_);
	buffer.addToBuffer((unsigned int) points_.size());
	std::list<unsigned int>::iterator itor;
	for (itor = points_.begin();
		itor != points_.end();
		itor++)
	{
		buffer.addToBuffer((unsigned int) (*itor));
	}

	return true;
}

bool ActionVector::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(actionId_)) return false;
	unsigned int count = 0;
	if (!reader.getFromBuffer(count)) return false;
	for (unsigned int c=0; c<count; c++)
	{
		unsigned int point;
		if (!reader.getFromBuffer(point)) return false;
		points_.push_back(point);
	}
	return true;
}

void ActionVector::addPoint(unsigned int point)
{
	points_.push_back(point);
}

unsigned int ActionVector::getPoint()
{
	unsigned int result = 0;
	if (!points_.empty())
	{
		result = points_.front();
		points_.pop_front();
	}
	return result;
}

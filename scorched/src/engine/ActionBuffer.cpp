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

#include <engine/ActionBuffer.h>
#include <common/Logger.h>

ActionBuffer::ActionBuffer() : context_(0), clientTime_(0.0f)
{
}

ActionBuffer::~ActionBuffer()
{
}

void ActionBuffer::clear()
{
	actionBuffer_.reset();
	std::list<std::pair<float, ActionMeta *> >::iterator itor;
	for (itor = actionList_.begin();
		itor != actionList_.end();
		itor++)
	{
		delete (*itor).second;
	}
	actionList_.clear();
}

int ActionBuffer::size()
{
	return (int) actionList_.size();
}

bool ActionBuffer::empty()
{
	return actionList_.empty();
}

void ActionBuffer::serverAdd(float time, ActionMeta *action)
{
	if (time > 60.0f)
	{
		Logger::log( "Warning: Action added for long time %.2f", time);
	}

	// Add this action into the buffer to be sent to 
	// the client
	unsigned int usedBefore = actionBuffer_.getBufferUsed();
	actionBuffer_.addToBuffer(time);
	actionBuffer_.addToBuffer(action->getClassName());
	action->setScorchedContext(context_);
	action->writeAction(actionBuffer_);
	unsigned int usedAfter = actionBuffer_.getBufferUsed();

	// Log buffer usage
	if (false)
	{
		Logger::log( "Action size %s = %u", 
			action->getClassName(),
			usedAfter - usedBefore);
	}
}

ActionMeta *ActionBuffer::getActionForTime(float time)
{
	if (actionList_.empty()) return 0;
	std::pair<float, ActionMeta *> front = actionList_.front();
	if (front.first <= time)
	{
		actionList_.pop_front();
		return front.second;
	}
	return 0;
}

bool ActionBuffer::writeMessage(NetBuffer &buffer)
{
	buffer.addDataToBuffer(actionBuffer_.getBuffer(), 
		actionBuffer_.getBufferUsed());
	return true;
}

void ActionBuffer::clientAdd(float time, ActionMeta *action)
{
	action->setScorchedContext(context_);
	tmpOrderedList.insert(
		std::pair<float, ActionMeta *>(time + clientTime_, action));
}

bool ActionBuffer::readMessage(NetBufferReader &reader)
{
	// Clear client list
	tmpOrderedList.clear();

	float time;
	while (reader.getFromBuffer(time))
	{
		clientTime_ = time;

		// Create and read the action
		std::string actionName;
		if (!reader.getFromBuffer(actionName))
		{
			Logger::log( "ActionController - "
				"Failed to deserailize action name");
			return false;
		}
		ActionMeta *newAction = (ActionMeta *)
			MetaClassRegistration::getNewClass(actionName.c_str());
		if (!newAction)
		{
			Logger::log( "ActionController - "
				"Failed to find action %s", actionName.c_str());
			return false;
		}
		newAction->setScorchedContext(context_);
		if (!newAction->readAction(reader))
		{
			Logger::log( "ActionController - "
				"Failed to read action %s", actionName.c_str());
			return false;
		}

		// Put the action onto the list
		clientAdd(0.0f, newAction);
	}

	// Now order the list (This is so you can add actions for any
	// time interval
	std::multimap<float, ActionMeta *>::iterator itor;
	std::multimap<float, ActionMeta *>::iterator endItor = tmpOrderedList.end();
	for (itor = tmpOrderedList.begin();
			itor != endItor;
			itor++)
	{
		std::pair<float, ActionMeta *> pair = (*itor);
		actionList_.push_back(pair);
	}

	return true;
}

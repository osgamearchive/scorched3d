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

#include <engine/ActionController.h>
#include <landscape/HeightMapCollision.h>
#include <list>

ActionController::ActionController() : 
	speed_(1.0f), referenceCount_(0), time_(0.0f), context_(0)
{

}

ActionController::~ActionController()
{

}

void ActionController::clear()
{
	// Action Buffer
	buffer_.clear();

	// New actions
	std::list<Action *>::iterator newItor;
	for (newItor = newActions_.begin();
		newItor != newActions_.end();
		newItor++)
	{
		delete *newItor;
	}
	newActions_.clear();

	// Current actions
	std::set<Action *>::iterator cItor;
	for (cItor = actions_.begin();
		cItor != actions_.end();
		cItor++)
	{
		delete *cItor;
	}
	actions_.clear();

	// Ref count
	referenceCount_ = 0;
}

bool ActionController::noReferencedActions()
{
	return newActions_.empty() && 
		(referenceCount_ == 0) &&
		buffer_.empty();
}

void ActionController::resetTime()
{
	time_ = 0.0f;
}

void ActionController::setScorchedContext(ScorchedContext *context)
{
	context_ = context;
}

void ActionController::setFast(float speedMult)
{
	speed_ = speedMult;
}

void ActionController::addAction(Action *action)
{
	action->setScorchedContext(context_);
	newActions_.push_back(action);
}

void ActionController::addNewActions()
{
	if (!context_->serverMode)
	{
		ActionMeta *action = 
			buffer_.getActionForTime(time_);
		while (action)
		{
			referenceCount_++;
			action->setScorchedContext(context_);
			action->init();
			actions_.insert(action);
			action = buffer_.getActionForTime(time_);
		}
	}

	while (!newActions_.empty())
	{
		Action *action = newActions_.front(); 
		if (context_->serverMode)
		{
			if (action->getReferenced()) referenceCount_ ++;
			if (action->getServerOnly()) 
			{
				buffer_.serverAdd(time_, (ActionMeta*) action);
			}
			action->init();
			actions_.insert(action);
		}
		else
		{
			if (action->getServerOnly())
			{	
				delete action;
			}
			else
			{
				action->init();
				actions_.insert(action);
			}
		}

		newActions_.pop_front();
	}
}

void ActionController::draw(const unsigned state)
{
	// Itterate and draw all of the actions
	std::set<Action *>::iterator itor;
	for (itor = actions_.begin();
		itor != actions_.end();
		itor++)
	{
		Action *act = *itor;
		act->draw();
	}
}

void ActionController::simulate(const unsigned state, float frameTime)
{
	if (frameTime > 1.0f) return;
	frameTime *= speed_;

	// As this simulator gives differing results dependant on
	// step size, always ensure step size is the same
	static float timePassed = 0.0f;
	timePassed += frameTime;

	// step size = 33 fps
	const float stepSize = 0.033f;

	HeightMapCollision::setContext(context_);
	while (timePassed >= stepSize)
	{
		time_ += stepSize;
		stepActions(stepSize);
		physicsEngine_.stepSimulation(stepSize);

		// More time has passed
		timePassed -= stepSize;
	}
}

void ActionController::stepActions(float frameTime)
{
	// Ensure any new actions are added
	addNewActions();

	static std::list<Action *> removeActions;
	// Itterate and draw all of the actions
	std::set<Action *>::iterator itor;
	for (itor = actions_.begin();
		itor != actions_.end();
		itor++)
	{
		Action *act = *itor;

		// If this action has finished add to list to be removed
		bool remove = false;
		act->simulate(frameTime, remove);
		if (remove) removeActions.push_back(act);
	}

	// Remove removed shots from the action list
	if (!removeActions.empty())
	{
		std::list<Action *>::iterator ritor;
		for (ritor = removeActions.begin();
			ritor != removeActions.end();
			ritor++)
		{
			Action *action = *ritor;
			if (action->getReferenced())
			{
				referenceCount_--;
				if (referenceCount_<0) referenceCount_ = 0;
			}
			actions_.erase(action);
			delete *ritor;
		}
		removeActions.clear();
	}
}

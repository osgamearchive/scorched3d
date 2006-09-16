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
#include <common/Logger.h>
#include <common/OptionsGame.h>
#include <list>

ActionController::ActionController() : 
	GameStateI("ActionController"),
	speed_(1.0f), referenceCount_(0), time_(0.0f), 
	context_(0), lastTraceTime_(0.0f),
	actionTracing_(false), totalTime_(0.0f),
	actionEvents_(false), actionProfiling_(false)
{

}

ActionController::~ActionController()
{

}

void ActionController::clear(bool warn)
{
	// Action Buffer
	buffer_.clear();

	// New actions
	std::list<Action *>::iterator newItor;
	for (newItor = newActions_.begin();
		newItor != newActions_.end();
		newItor++)
	{
		Action *act = *newItor;
		if (warn)
		{
			Logger::log(formatString("Warning: removing added timed out action %s, %s",
				act->getActionType(), (act->getReferenced()?"Ref":"UnRef")));
		}
		delete act;
	}
	newActions_.clear();

	// Current actions
	std::set<Action *>::iterator cItor;
	for (cItor = actions_.begin();
		cItor != actions_.end();
		cItor++)
	{
		Action *act = *cItor;
		if (warn)
		{
			Logger::log(formatString("Warning: removing added timed out action %s, %s",
				act->getActionType(), (act->getReferenced()?"Ref":"UnRef")));
		}
		delete act;
	}
	actions_.clear();

	// Ref count
	referenceCount_ = 0;
}

bool ActionController::allEvents()
{
	std::list<Action *>::iterator newItor;
	for (newItor = newActions_.begin();
		newItor != newActions_.end();
		newItor++)
	{
		Action *act = *newItor;
		if (!act->getActionEvent()) return false;
	}
	std::set<Action *>::iterator cItor;
	for (cItor = actions_.begin();
		cItor != actions_.end();
		cItor++)
	{
		Action *act = *cItor;
		if (!act->getActionEvent()) return false;
	}
	return true;
}

void ActionController::logActions()
{
	Logger::log(formatString("ActionLog : Time %.2f, New %i, Ref %i, Buf %i",
		time_,
		(int) newActions_.size(), 
		referenceCount_,
		buffer_.size()));
	std::set<Action *>::iterator itor;
	for (itor = actions_.begin();
		itor != actions_.end();
		itor++)
	{
		Action *act = *itor;
		Logger::log(formatString("Action : %s", act->getActionType()));
	}
}

void ActionController::logProfiledActions()
{
	if (!actionProfiling_) return;

	Logger::log("Logging Profiled Actions --------------------");
	int totalCount = 0;
	std::map<std::string, int>::iterator itor;
	for (itor =  actionProfile_.begin();
		itor != actionProfile_.end();
		itor++)
	{
		const std::string &name = (*itor).first;
		int count = (*itor).second;
		totalCount += count;
		Logger::log(formatString("%s - %i", name.c_str(), count));
	}
	Logger::log(formatString("Total - %i", totalCount));
	Logger::log("---------------------------------------------");

	actionProfile_.clear();
}

bool ActionController::noReferencedActions()
{
	bool finished = (newActions_.empty() && 
		(referenceCount_ == 0) &&
		buffer_.empty());

	if (actionTracing_)
	{
		if (time_ - lastTraceTime_ > 5.0f)
		{
			lastTraceTime_ = time_;
			logActions();
		}
	}

	return finished;
}

void ActionController::resetTime()
{
	time_ = 0.0f;
	lastTraceTime_ = 0.0f;
	totalTime_ = 0.0f;
}

void ActionController::setScorchedContext(ScorchedContext *context)
{
	context_ = context;
	buffer_.setContext(context);
	physicsEngine_.setScorchedContext(context);
}

void ActionController::setFast(float speedMult)
{
	speed_ = speedMult;
}

void ActionController::addAction(Action *action)
{
	action->setScorchedContext(context_);
	action->setActionStartTime(time_);
	action->setActionEvent(actionEvents_);
	newActions_.push_back(action);

	if (actionProfiling_)
	{
		std::map<std::string, int>::iterator findItor =
			actionProfile_.find(action->getActionType());
		if (findItor == actionProfile_.end())
		{
			actionProfile_[action->getActionType()] = 1;
		}
		else
		{
			(*findItor).second++;
		}	
	}
}

void ActionController::addNewActions()
{
	if (!context_->serverMode)
	{
		ActionMeta *action = 
			buffer_.getActionForTime(time_);
		while (action)
		{
			if (action->getReferenced()) referenceCount_ ++;
			action->setScorchedContext(context_);
			action->setActionStartTime(time_);
			action->init();
			actions_.insert(action);
			action = buffer_.getActionForTime(time_);
		}
	}

	while (!newActions_.empty())
	{
		Action *action = newActions_.front(); 

		action->setScorchedContext(context_);
		action->setActionStartTime(time_);
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
				if (action->getReferenced()) referenceCount_ ++;
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
	frameTime *= speed_;

	// As this simulator gives differing results dependant on
	// step size, always ensure step size is the same
	static float timePassed = 0.0f;
	timePassed += frameTime;

	// step size = 1.0 / physics fps = steps per second
	physicsEngine_.resetContext();
	const float stepSize = 1.0f / float(context_->optionsGame->getPhysicsFPS());
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

	std::list<Action *> removeActions;
	// Itterate and draw all of the actions
	std::set<Action *>::iterator itor;
	for (itor = actions_.begin();
		itor != actions_.end();
		itor++)
	{
		Action *act = *itor;

		bool remove = false;
		act->simulate(frameTime, remove);

		// Ensure that no referenced actions over do their time
		if (act->getReferenced())
		{
			if ((time_ - act->getActionStartTime() > 30.0f) ||
				(totalTime_ > 0.0f && time_ > totalTime_ + 15.0f))
			{
				Logger::log(formatString("Warning: removing timed out action %s",
					act->getActionType()));
				remove = true;
			}
		}

		// If this action has finished add to list to be removed
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
			delete action;
		}
		removeActions.clear();
	}
}


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

#include <engine/ScorchedContext.h>
#include <actions/CameraPositionAction.h>
#include <common/OptionsParam.h>
#include <dialogs/ScoreDialog.h>

REGISTER_ACTION_SOURCE(CameraPositionAction);

CameraPositionAction::CameraPositionAction() : totalTime_(0.0f)
{
}

CameraPositionAction::CameraPositionAction(Vector &showPosition,
		float showTime,
		unsigned int priority) : 
		totalTime_(0.0f), showTime_(showTime),
		showPosition_(showPosition), showPriority_(priority)
{

}

CameraPositionAction::~CameraPositionAction()
{
	CameraPositionActionRegistry::rmCameraPositionAction(this);
}

void CameraPositionAction::init()
{
	CameraPositionActionRegistry::addCameraPositionAction(this);
	startTime_ = context_->actionController.getActionTime();
}

void CameraPositionAction::simulate(float frameTime, bool &remove)
{
	totalTime_ += frameTime;
	if (totalTime_ > showTime_)
	{
		remove = true;
	}

	Action::simulate(frameTime, remove);
}

bool CameraPositionAction::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(showTime_);
	buffer.addToBuffer(showPosition_[0]);
	buffer.addToBuffer(showPosition_[1]);
	buffer.addToBuffer(showPosition_[2]);
	buffer.addToBuffer(showPriority_);
	return true;
}

bool CameraPositionAction::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(showTime_)) return false;
	if (!reader.getFromBuffer(showPosition_[0])) return false;
	if (!reader.getFromBuffer(showPosition_[1])) return false;
	if (!reader.getFromBuffer(showPosition_[2])) return false;
	if (!reader.getFromBuffer(showPriority_)) return false;
	return true;
}

std::set<CameraPositionAction *> CameraPositionActionRegistry::actions_;
CameraPositionAction *CameraPositionActionRegistry::currentAction_ = 0;

void CameraPositionActionRegistry::addCameraPositionAction(CameraPositionAction *action)
{
	actions_.insert(action);
}

void CameraPositionActionRegistry::rmCameraPositionAction(CameraPositionAction *action)
{
	actions_.erase(action);
}

CameraPositionAction *CameraPositionActionRegistry::getCurrentAction()
{
	// Check if the current action is still active
	if (actions_.find(currentAction_) == actions_.end())
	{
		// If the current action is not still going get another one
		currentAction_ = getCurrentBest();
	}
	else 
	{
		// If it is check that it is the best action
		CameraPositionAction *bestAction = getCurrentBest();
		if (bestAction)
		{
			if (bestAction->getShowPriority() > currentAction_->getShowPriority())
			{
				currentAction_ = bestAction;
			}
		}
	}

	return currentAction_;
}

CameraPositionAction *CameraPositionActionRegistry::getCurrentBest()
{
	CameraPositionAction *currentBest = 0;
	if (!actions_.empty())
	{
		std::set<CameraPositionAction *>::iterator itor;
		for (itor = actions_.begin();
			itor != actions_.end();
			itor++)
		{
			CameraPositionAction *action = (*itor);
			
			// Check that this action is near the beginning
			float currentTime = action->getScorchedContext()->
				actionController.getActionTime();
			float actionTime = action->getStartTime();
			if (currentTime - actionTime < 1.0f)
			{
				// Is there an action to beat
				if (!currentBest) currentBest = action;
				else
				{
					// Yes, so check if it beats it
					if (currentBest->getShowPriority() < action->getShowPriority())
					{
						// There is a new current best
						currentBest = action;
					}
				}
			}
		}
	}
	return currentBest;
}
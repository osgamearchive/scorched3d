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
#include <common/WindowManager.h>
#include <common/OptionsParam.h>
#include <dialogs/ScoreDialog.h>

Vector CameraPositionAction::position_(122.0f, 122.0f, 10.0f);
unsigned int CameraPositionAction::priority_ = 0;

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
}

void CameraPositionAction::init()
{

}

void CameraPositionAction::simulate(float frameTime, bool &remove)
{
	if (!context_->serverMode)
	{
		if (showPriority_ > priority_)
		{
			priority_ = showPriority_;
			position_ = showPosition_;
		}
	}

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

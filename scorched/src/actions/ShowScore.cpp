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
#include <actions/ShowScore.h>
#include <GLW/GLWWindowManager.h>
#include <common/OptionsParam.h>
#include <dialogs/ScoreDialog.h>

REGISTER_ACTION_SOURCE(ShowScore);

ShowScore::ShowScore() : totalTime_(0.0f)
{
}

ShowScore::~ShowScore()
{
}

void ShowScore::init()
{
	if (!context_->serverMode)
	{
		GLWWindowManager::instance()->showWindow(
			ScoreDialog::instance()->getId());
	}
}

void ShowScore::simulate(float frameTime, bool &remove)
{

	totalTime_ += frameTime;
	if (totalTime_ > 5.0f)
	{
		remove = true;
		if (!context_->serverMode)
		{
			GLWWindowManager::instance()->hideWindow(
				ScoreDialog::instance()->getId());
		}
	}
	Action::simulate(frameTime, remove);
}

bool ShowScore::writeAction(NetBuffer &buffer)
{
	return true;
}

bool ShowScore::readAction(NetBufferReader &reader)
{
	return true;
}

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

#include <client/SpeedChange.h>
#include <client/ScorchedClient.h>
#include <common/Keyboard.h>
#include <GLW/GLWFont.h>
#include <stdio.h>

SpeedChange *SpeedChange::instance_ = 0;

SpeedChange *SpeedChange::instance()
{
	if (!instance_)
	{
		instance_ = new SpeedChange;
	}
	return instance_;
}

SpeedChange::SpeedChange()
{
}

SpeedChange::~SpeedChange()
{
}

void SpeedChange::resetSpeed()
{
	ScorchedClient::instance()->getActionController().setFast(1.0f);
}

void SpeedChange::draw(const unsigned state)
{
	float speed = ScorchedClient::instance()->getActionController().getFast();
	if (speed > 1.0f)
	{
		GLState state(GLState::BLEND_ON | GLState::TEXTURE_OFF); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		static char buffer[10];
		sprintf(buffer, "%iX", (int) speed);

		static Vector fontColor(0.7f, 0.7f, 0.2f);
		GLWFont::instance()->getFont()->draw(fontColor, 20, 10.0f, 10.0f, 0.0f, buffer);
	}
}

void SpeedChange::keyboardCheck(const unsigned state, float frameTime, 
							   char *buffer, unsigned int keyState,
							   KeyboardHistory::HistoryElement *history, int hisCount, 
							   bool &skipRest)
{
	KEYBOARDKEY("SIMULATION_SPEED_X1", x1Key);
	KEYBOARDKEY("SIMULATION_SPEED_X2", x2Key);
	KEYBOARDKEY("SIMULATION_SPEED_X3", x3Key);
	KEYBOARDKEY("SIMULATION_SPEED_X4", x4Key);

	if (x1Key->keyDown(buffer, keyState))
	{
		resetSpeed();
	}
	else if (x2Key->keyDown(buffer, keyState))
	{
		ScorchedClient::instance()->getActionController().setFast(2.0f);
	}
	else if (x3Key->keyDown(buffer, keyState))
	{
		ScorchedClient::instance()->getActionController().setFast(4.0f);
	}
	else if (x4Key->keyDown(buffer, keyState))
	{
		ScorchedClient::instance()->getActionController().setFast(8.0f);
	}
}

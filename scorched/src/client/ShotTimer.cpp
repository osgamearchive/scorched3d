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


#include <client/ShotTimer.h>
#include <common/OptionsGame.h>
#include <GLW/GLWFont.h>

ShotTimer *ShotTimer::instance_ = 0;

ShotTimer *ShotTimer::instance()
{
	if (!instance_)
	{
		instance_ = new ShotTimer;
	}
	return instance_;
}

ShotTimer::ShotTimer() : counter_(0.0f), showTime_(true)
{
}

ShotTimer::~ShotTimer()
{
}

void ShotTimer::enterState(const unsigned state)
{
	counter_ = 0.0f;
	blinkTimer_ = 0.0f;
	showTime_ = true;
}

void ShotTimer::simulate(const unsigned state, float simTime)
{
	counter_ += simTime;
	blinkTimer_ += simTime;
}

void ShotTimer::draw(const unsigned currentstate)
{
	int shotTime = OptionsGame::instance()->getShotTime();
	if (shotTime == 0) return;

	// The remaining time for this shot
	int timeLeft = (shotTime - int(counter_));

	// Split into seconds and minutes
	div_t split = div(timeLeft, 60);

	GLState state(GLState::BLEND_ON | GLState::TEXTURE_OFF | GLState::DEPTH_OFF); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	static float fVPort[4];
	glGetFloatv(GL_VIEWPORT, fVPort);

	static Vector fontColor;
	fontColor = Vector(0.7f, 0.7f, 0.2f);
	if (timeLeft <= 5)
	{
		fontColor = Vector(0.7f, 0.0f, 0.0f);
		if (blinkTimer_ > 0.25f)
		{
			showTime_ = !showTime_;
			blinkTimer_ = 0.0f;
		}
	}

	if (showTime_)
	{
		const char *format = "%02i:%02i";
		if (timeLeft < 0) format = "--:--";
		GLWFont::instance()->getFont()->draw(
			fontColor, 20, (fVPort[2]/2.0f) - 30.0f, fVPort[3] - 43.0f, 0.0f, format, 
			split.quot,
			split.rem);
	}
}

bool ShotTimer::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	int shotTime = OptionsGame::instance()->getShotTime();
	if (shotTime == 0) return false; // ShotTime == 0 is infinite time

	int timeLeft = (shotTime - int(counter_));
	return (timeLeft < 0);
}
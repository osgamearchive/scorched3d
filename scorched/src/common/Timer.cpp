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


// Timer.cpp: implementation of the Timer class.
//
//////////////////////////////////////////////////////////////////////

#include <common/Defines.h>
#include <common/Timer.h>
#include <SDL/SDL.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Timer::Timer() : isPaused_(false)
{
	dwLastTime_ = SDL_GetTicks();
}

Timer::~Timer()
{
}

void Timer::pause()
{
	if (!isPaused_)
	{
		unsigned int  dwCurrentTime = SDL_GetTicks();
		unsigned int  dwTimeDiff = dwCurrentTime - dwLastTime_;
		dwLastTime_ = dwTimeDiff;
		isPaused_ = true;
	}
}


void Timer::resume()
{
	if (isPaused_)
	{
		unsigned int  dwCurrentTime = SDL_GetTicks();
		dwLastTime_ = dwCurrentTime - dwLastTime_;

		isPaused_ = false;
	}
}

float Timer::getTimeDifference()
{

	if (isPaused_) return 0.0f;

	unsigned int  dwCurrentTime = SDL_GetTicks();
	unsigned int  dwTimeDiff = dwCurrentTime - dwLastTime_;
	if (dwTimeDiff < 0)
	{
		dwTimeDiff = 1;
	}

	dwLastTime_ = dwCurrentTime;
	return ((float) dwTimeDiff) / 1000.0f;

}


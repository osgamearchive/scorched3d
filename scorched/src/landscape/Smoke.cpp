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


// Smoke.cpp: implementation of the Smoke class.
//
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <landscape/Smoke.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SmokeCounter::SmokeCounter(float minTime, float timeDiff) :
	currentTime_(0.0f), minTime_(minTime), timeDiff_(timeDiff)
{
	genNextTime();
}

bool SmokeCounter::nextDraw(float frameTime)
{
	currentTime_ += frameTime;
	if (currentTime_ > nextTime_)
	{
		genNextTime();
		currentTime_ = 0.0f;
		return true;
	}

	return false;
}

void SmokeCounter::genNextTime()
{
	nextTime_ = RAND * timeDiff_ + minTime_;
}

Smoke::Smoke() : chain_(1500)
{

}

Smoke::~Smoke()
{

}

void Smoke::addSmoke(float x, float y, float z, 
					 float dx, float dy, float dz,
					 float maxSize, float maxTime)
{
	SmokeChainEntry *entry = chain_.getNextEntry();
	if (entry)
	{
		static Vector setVector;
		setVector[0] = x;
		setVector[1] = y;
		setVector[2] = z;

		static Vector moveVector;
		moveVector[0] = dx;
		moveVector[1] = dy;
		moveVector[2] = dz;

		entry->set(setVector, moveVector, maxSize, maxTime);
	}
}

void Smoke::simulate(float frameTime)
{
	SmokeChainEntry *entry = chain_.getFirst();
	if (!entry) return;

	int removeEntry = 0;
	for (;entry; entry = chain_.getNext())
	{
		if (!entry->move(frameTime))
		{
			removeEntry ++;
		}
	}

	while (removeEntry > 0) 
	{
		chain_.removeEntry();
		removeEntry--;
	}
}

void Smoke::draw()
{
	SmokeChainEntry *entry = chain_.getFirst();
	if (!entry) return;

	for (;entry; entry = chain_.getNext())
	{
		entry->draw();
	}
}

bool Smoke::noSmoke()
{
	return (chain_.getNoEntries() == 0);
}

void Smoke::removeAllSmokes()
{
	while (chain_.getNoEntries() > 0)
	{
		chain_.removeEntry();
	}
}
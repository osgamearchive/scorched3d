////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <landscape/LandscapeObjectsGroupEntry.h>
#include <common/Defines.h>
#include <common/Vector.h>

LandscapeObjectsGroupEntry::LandscapeObjectsGroupEntry() :
	count_(0)
{
	for (int a=0; a<64; a++)
	{
		for (int b=0; b<64; b++)
		{
			distance[a + b * 64] = 255.0f;
		}
	}
}

LandscapeObjectsGroupEntry::~LandscapeObjectsGroupEntry()
{
}

int LandscapeObjectsGroupEntry::getObjectCount()
{
	return count_;
}

float LandscapeObjectsGroupEntry::getDistance(int x, int y)
{
	x /= 4;
	y /= 4;

	if (x >=0 && x < 64 && y >=0 && y < 64)
	{
		return distance[x + y * 64];
	}
	return 255.0f;
}

void LandscapeObjectsGroupEntry::addObject(int x, int y)
{
	count_ ++;
	x /= 4;
	y /= 4;

	if (x >=0 && x < 64 && y >=0 && y < 64)
	{
		Vector posA(x, y, 0);
		for (int a=0; a<64; a++)
		{
			for (int b=0; b<64; b++)
			{
				Vector posB(a, b, 0);
				float d = (posB - posA).Magnitude();
				distance[a + b * 64] = MIN(distance[a + b * 64], d);
			}
		}
	}
}

void LandscapeObjectsGroupEntry::removeObject(int x, int y)
{
	x /= 4;
	y /= 4;
	if (x >=0 && x < 64 && y >=0 && y < 64)
	{
		distance[x + y * 64] += 2.0f;
	}

	count_ --;
}

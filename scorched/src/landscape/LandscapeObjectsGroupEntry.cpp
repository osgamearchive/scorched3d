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
#include <landscape/HeightMap.h>
#include <common/Defines.h>
#include <common/Vector.h>

LandscapeObjectsGroupEntry::LandscapeObjectsGroupEntry(HeightMap &map) :
	count_(0), distance_(0)
{
	mapWidthMult_ = 4;
	mapHeightMult_ = 4;
	mapWidth_ = map.getMapWidth() / mapWidthMult_;
	mapHeight_ = map.getMapHeight() / mapHeightMult_;

	distance_ = new float[mapWidth_ * mapHeight_];
	for (int a=0; a<mapWidth_; a++)
	{
		for (int b=0; b<mapHeight_; b++)
		{
			distance_[a + b * mapWidth_] = 255.0f;
		}
	}
}

LandscapeObjectsGroupEntry::~LandscapeObjectsGroupEntry()
{
	delete [] distance_;
}

int LandscapeObjectsGroupEntry::getObjectCount()
{
	return count_;
}

float LandscapeObjectsGroupEntry::getDistance(int x, int y)
{
	x /= mapWidthMult_;
	y /= mapHeightMult_;

	if (x >=0 && x < mapWidth_ && y >=0 && y < mapHeight_)
	{
		return distance_[x + y * mapWidth_];
	}
	return 255.0f;
}

void LandscapeObjectsGroupEntry::addObject(int x, int y)
{
	count_ ++;
	x /= mapWidthMult_;
	y /= mapHeightMult_;

	if (x >=0 && x < mapWidth_ && y >=0 && y < mapHeight_)
	{
		Vector posA(x, y, 0);
		for (int a=0; a<mapWidth_; a++)
		{
			for (int b=0; b<mapHeight_; b++)
			{
				Vector posB(a, b, 0);
				posB -= posA;
				float d = posB.Magnitude();
				distance_[a + b * mapWidth_] = MIN(distance_[a + b * mapWidth_], d);
			}
		}
	}
}

void LandscapeObjectsGroupEntry::removeObject(int x, int y)
{
	x /= mapWidthMult_;
	y /= mapHeightMult_;
	if (x >=0 && x < mapWidth_ && y >=0 && y < mapHeight_)
	{
		distance_[x + y * mapWidth_] += 2.0f;
	}

	count_ --;
}

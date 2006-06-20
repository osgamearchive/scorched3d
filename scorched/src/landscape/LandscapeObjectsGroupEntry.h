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

#if !defined(__INCLUDE_LandscapeObjectsGroupEntryh_INCLUDE__)
#define __INCLUDE_LandscapeObjectsGroupEntryh_INCLUDE__

#include <landscape/LandscapeObjectsSetEntry.h>

class HeightMap;
class LandscapeObjectsGroupEntry : public LandscapeObjectsSetEntry
{
public:
	LandscapeObjectsGroupEntry(HeightMap &map);
	virtual ~LandscapeObjectsGroupEntry();

	virtual float getDistance(int x, int y);

	virtual void addObject(LandscapeObjectEntryBase *object, bool thin);
	virtual bool removeObject(LandscapeObjectEntryBase *object);

protected:
	float *distance_;
	int mapWidth_, mapHeight_;
	int mapWidthMult_, mapHeightMult_;
};

#endif // __INCLUDE_LandscapeObjectsGroupEntryh_INCLUDE__

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

#if !defined(__INCLUDE_MovementMaph_INCLUDE__)
#define __INCLUDE_MovementMaph_INCLUDE__

#include <landscape/HeightMap.h>
#include <list>

class Tank;
class ScorchedContext;
class MovementMap
{
public:
	enum MovementMapEntryType
	{
		eNotSeen = 0,
		eNoMovement = 1,
		eMovement = 2
	};
	struct MovementMapEntry
	{
		MovementMapEntry() {}
		MovementMapEntry(
			MovementMapEntryType t,
			float d,
			unsigned int s,
			unsigned int e) : type(t), dist(d), srcEntry(s), epoc(e) {}

		MovementMapEntryType type;
		float dist;
		unsigned int srcEntry;
		unsigned int epoc;
	};

	MovementMap(int width, int height);
	virtual ~MovementMap();

	void calculateForTank(Tank *tank, 
		unsigned int fuelId,
		ScorchedContext &context, 
		bool maxFuel = false);
	void movementTexture();
	static void limitTexture(Vector &center, int limit);
	MovementMapEntry &getEntry(int w, int h);

protected:
	MovementMapEntry *entries_;
	int width_, height_;

	unsigned int POINT_TO_UINT(unsigned int x, unsigned int y);
	void addPoint(unsigned int x, unsigned int y, 
					 float height, float dist,
					 std::list<unsigned int> &edgeList,
					 unsigned int sourcePt,
					 ScorchedContext &context,
					 float minHeight,
					 unsigned int epoc);

};

#endif

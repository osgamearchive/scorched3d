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

#include <common/Defines.h>

const float MaxTankClimbHeight = 0.4f;

class Tank;
class MovementMap
{
public:
	enum MovementMapEntryType
	{
		eNoMovement = 0,
		eMovement = 1
	};
	struct MovementMapEntry
	{
		MovementMapEntry() {}
		MovementMapEntry(
			MovementMapEntryType t,
			float d,
			unsigned int s) : type(t), dist(d), srcEntry(s) {}

		MovementMapEntryType type;
		float dist;
		unsigned int srcEntry;
	};

	MovementMap(int width);
	virtual ~MovementMap();

	void clear();
	void calculateForTank(Tank *tank);
	void movementTexture();

	MovementMapEntry &getEntry(int w, int h) { 
		DIALOG_ASSERT(w >= 0 && h >= 0 && w<=width_ && h<=width_); 
		return entries_[(width_+1) * h + w]; }

protected:
	int width_;
	MovementMapEntry *entries_;

};


#endif

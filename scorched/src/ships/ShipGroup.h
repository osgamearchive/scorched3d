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

#if !defined(__INCLUDE_ShipGrouph_INCLUDE__)
#define __INCLUDE_ShipGrouph_INCLUDE__

#include <common/SplinePath.h>
#include <ships/Ship.h>

class RandomGenerator;
class LandscapeShipGroup;
class ShipGroup
{
public:
	ShipGroup();
	virtual ~ShipGroup();

	void generate(RandomGenerator &random, LandscapeShipGroup *shipGroup);
	void draw();
	void simulate(float frameTime);

protected:
	SplinePath path_;
	std::vector<Ship *> ships_;
};

#endif // __INCLUDE_ShipGrouph_INCLUDE__

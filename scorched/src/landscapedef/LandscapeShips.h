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

#if !defined(__INCLUDE_LandscapeShipsh_INCLUDE__)
#define __INCLUDE_LandscapeShipsh_INCLUDE__

#include <list>
#include <vector>
#include <common/Vector.h>
#include <common/ModelID.h>

class LandscapeDefinitions;
class LandscapeShip
{
public:
	ModelID model;
	float scale;

	virtual bool readXML(XMLNode *node);
};

class LandscapeShipGroup
{
public:
	float speed;
	int controlpoints;
	float controlpointswidth;
	float controlpointsheight;
	float controlpointsrand;
	float starttime;

	std::vector<LandscapeShip *> ships;

	virtual bool readXML(XMLNode *node);
};

class LandscapeShips
{
public:
	LandscapeShips();
	virtual ~LandscapeShips();

	std::vector<LandscapeShipGroup *> objects;

	bool readXML(LandscapeDefinitions *definitions, XMLNode *node);

private:
	LandscapeShips(const LandscapeShips &other);
	LandscapeShips &operator=(LandscapeShips &other);
};


#endif // __INCLUDE_LandscapeShipsh_INCLUDE__

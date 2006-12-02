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

#include <ships/ShipGroup.h>
#include <common/Defines.h>
#include <common/RandomGenerator.h>
#include <graph/OptionsDisplay.h>
#include <client/ScorchedClient.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapedef/LandscapeShips.h>
#include <landscapedef/LandscapeTex.h>

ShipGroup::ShipGroup()
{
}

ShipGroup::~ShipGroup()
{
	while(!ships_.empty())
	{
		Ship *ship = ships_.back();
		ships_.pop_back();
		delete ship;
	}
}

void ShipGroup::generate(RandomGenerator &random, LandscapeShipGroup *shipGroup)
{
	int mapWidth = 
		ScorchedClient::instance()->getLandscapeMaps().
			getGroundMaps().getMapWidth();
	int mapHeight =
		ScorchedClient::instance()->getLandscapeMaps().
			getGroundMaps().getMapHeight();
	float waterHeight = 0.0f;
	LandscapeTex &tex = 
		*ScorchedClient::instance()->getLandscapeMaps().
			getDefinitions().getTex();

	if (tex.border->getType() == LandscapeTexType::eWater)
	{
		LandscapeTexBorderWater *water = 
			(LandscapeTexBorderWater *) tex.border;
       	waterHeight = water->height;
	}

	std::vector<Vector> controlPoints;
	controlPoints.push_back(Vector::nullVector);
	float diff = 360.0f / float(shipGroup->controlpoints);
	for (float i=0.0f; i<360.0f; i+=diff)
	{
		float distWidth = random.getRandFloat() * shipGroup->controlpointsrand + shipGroup->controlpointswidth;
		float distHeight = random.getRandFloat() * shipGroup->controlpointsrand + shipGroup->controlpointsheight;
		float x = getFastSin(i / 180.0f * PI) * distWidth + float(mapWidth) / 2.0f;
		float y = getFastCos(i / 180.0f * PI) * distHeight + float(mapHeight) / 2.0f;

		Vector pt(x,y,waterHeight);
		controlPoints.push_back(pt);
	}

	Vector midPt = (controlPoints[1] + controlPoints.back()) / 2.0;
	controlPoints.push_back(midPt);
	controlPoints.front() = midPt;

	path_.generate(controlPoints, 200, 3, shipGroup->speed);
	path_.simulate(shipGroup->starttime);

	std::vector<LandscapeShip *>::iterator itor;
	for (itor = shipGroup->ships.begin();
		itor != shipGroup->ships.end();
		itor++)
	{
		LandscapeShip *texShip = (*itor);

		Ship *ship = new Ship(texShip);
		float offX = random.getRandFloat() * 200.0f;
		float offY = random.getRandFloat() * 200.0f;
		Vector offset(offX, offY - 100.0f);
		ship->getOffSet() = offset;
		ships_.push_back(ship);
	}
}

void ShipGroup::simulate(float frameTime)
{
	path_.simulate(frameTime);

	std::vector<Ship *>::iterator itor;
	for (itor = ships_.begin();
		itor != ships_.end();
		itor++)
	{
		Ship *ship = (*itor);
		ship->simulate(frameTime);
	}
}

void ShipGroup::draw()
{
	if (OptionsDisplay::instance()->getDrawShipPaths())
	{
		path_.draw();
	}

	Vector position;
	Vector direction;
	path_.getPathAttrs(position, direction);
	Vector directionPerp = direction.get2DPerp();

	std::vector<Ship *>::iterator itor;
	for (itor = ships_.begin();
		itor != ships_.end();
		itor++)
	{
		Ship *ship = (*itor);
		ship->draw(position, direction, directionPerp);
	}
}

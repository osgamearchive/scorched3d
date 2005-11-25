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
#include <client/ScorchedClient.h>
#include <landscape/LandscapeMaps.h>

ShipGroup::ShipGroup()
{
}

ShipGroup::~ShipGroup()
{
}

void ShipGroup::generate()
{
	int mapWidth = 
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapWidth();
	int mapHeight =
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapHeight();

	std::vector<Vector> controlPoints;
	for (float i=0.0f; i<360.0f; i+=45.0f)
	{
		float dist = RAND * 400 + 250.0f;
		float x = getFastSin(i / 180.0f * PI) * dist + float(mapWidth) / 2.0f;
		float y = getFastCos(i / 180.0f * PI) * dist + float(mapHeight) / 2.0f;

		Vector pt(x,y,10.0f);
		controlPoints.push_back(pt);
	}
	controlPoints.push_back(controlPoints[0]);

	path_.generate(controlPoints, 200, 3, 5.0f);
}

void ShipGroup::simulate(float frameTime)
{
	path_.simulate(frameTime);
}

void ShipGroup::draw()
{
	path_.draw();
}

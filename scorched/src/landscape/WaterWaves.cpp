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

#include <landscape/WaterWaves.h>
#include <landscape/Landscape.h>
#include <client/ScorchedClient.h>

WaterWaves::WaterWaves()
{

}

WaterWaves::~WaterWaves()
{
}

void WaterWaves::generateWaves()
{
	memset(wavePoints_, 0, 256 * 256 * sizeof(bool));
	paths_.clear();

	// Find all of the points that are equal to a certain height (the water height)
	findPoints();

	// Find the list of points that are next to eachother
	while (findNextPath()) {}
}

void WaterWaves::findPoints()
{
	const float waterHeight = 5.0f;
	for (int y=1; y<255; y++)
	{
		for (int x=1; x<255; x++)
		{
			float height =
				ScorchedClient::instance()->getLandscapeMaps().getHMap().getHeight(x, y);
			if (height > waterHeight - 4.0f && height < waterHeight)
			{
				float height2=
					ScorchedClient::instance()->getLandscapeMaps().getHMap().getHeight(x+1, y);
				float height3=
					ScorchedClient::instance()->getLandscapeMaps().getHMap().getHeight(x-1, y);
				float height4=
					ScorchedClient::instance()->getLandscapeMaps().getHMap().getHeight(x, y+1);
				float height5=
					ScorchedClient::instance()->getLandscapeMaps().getHMap().getHeight(x, y-1);

				if (height2 > waterHeight || height3 > waterHeight ||
					height4 > waterHeight || height5 > waterHeight)
				{
					wavePoints_[x + y * 256] = true;
				}
			}
		}
	}
}

bool WaterWaves::findNextPath()
{
	static std::vector<Vector> points;
	points.clear();

	for (int y=1; y<255; y++)
	{
		for (int x=1; x<255; x++)
		{
			if (wavePoints_[x + y * 256]) 
			{
				findPath(points, x, y);
				constructLines(points);
				return true;			
			}
		}
	}

	return false;
}

void WaterWaves::findPath(std::vector<Vector> &points, int x, int y)
{
	points.push_back(Vector(x, y, 4));

	wavePoints_[x + y * 256] = false;
	if (wavePoints_[(x+1) + y * 256]) findPath(points, x+1, y);
	else if (wavePoints_[(x-1) + y * 256]) findPath(points, x-1, y);
	else if (wavePoints_[x + (y-1) * 256]) findPath(points, x, y-1);
	else if (wavePoints_[x + (y+1) * 256]) findPath(points, x, y+1);
	else if (wavePoints_[(x-1) + (y-1) * 256]) findPath(points, x-1, y-1);
	else if (wavePoints_[(x-1) + (y+1) * 256]) findPath(points, x-1, y+1);
	else if (wavePoints_[(x+1) + (y+1) * 256]) findPath(points, x+1, y+1);
	else if (wavePoints_[(x+1) + (y-1) * 256]) findPath(points, x+1, y-1);
}

void WaterWaves::constructLines(std::vector<Vector> &points)
{
	Vector point = points.front();
	for (int i=5; i<(int)points.size(); i+=5)
	{
		Vector &current = points[i];
		int diffX = int(current[0]) - int(point[0]);
		int diffY = int(current[1]) - int(point[1]);
		if (diffX * diffX + diffY * diffY > 5)
		{
			std::pair<Vector, Vector> pt;
			pt.first = point;
			pt.second = current;
			paths_.push_back(pt);

			point = current;
		}
	}
}

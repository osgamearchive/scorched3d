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

#include <landscape/DeformLandscape.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/LandscapeTex.h>
#include <landscape/LandscapeDefinition.h>
#include <engine/ScorchedContext.h>
#include <common/Defines.h>
#include <math.h>

bool DeformLandscape::deformLandscape(
	ScorchedContext &context,
	Vector &pos, float radius, bool down, DeformPoints &map)
{
	HeightMap &hmap = context.landscapeMaps->getHMap();

	bool hits = false;
	int iradius = (int) radius + 1;
	if (iradius > 49) iradius = 49;

	float lowestHeight = 
		context.landscapeMaps->getTex(context).lowestlandheight;

	// Take out or add a chunk into the landsacpe
	for (int x=-iradius; x<=iradius; x++)
	{
		for (int y=-iradius; y<=iradius; y++)
		{
			map.map[x+iradius][y+iradius] = -1.0f;

			Vector newPos(pos[0] + x, pos[1] + y, pos[2]);
			if (((int) newPos[0] >=0) && (newPos[0] < hmap.getWidth()) &&
				((int) newPos[1] >=0) && (newPos[1] < hmap.getWidth()))
			{
				float dist = (pos - newPos).Magnitude();

				if (dist < radius)
				{
					float distToRadius = radius - dist;
					float currentHeight = hmap.getHeight((int) newPos[0], (int) newPos[1]);
					float explosionDepth = (float) sin((distToRadius / radius) * 1.57) * radius;

					float newHeight = currentHeight;
					float newMap = -1.0f;
					if (down)
					{
						if (currentHeight > newPos[2] - explosionDepth)
						{
							newMap = 1.0f - (dist / radius);
							newMap *= 3.0f;
							if (newMap > 1.0f) newMap = 1.0f;

							if (currentHeight > newPos[2] + explosionDepth)
							{
								newHeight -= explosionDepth + explosionDepth;
							}
							else
							{
								newHeight = newPos[2] - explosionDepth;
							}

							if (newHeight < lowestHeight)
							{
								if (currentHeight < lowestHeight)
								{
									newHeight = currentHeight;
								}
								else
								{
									newHeight = lowestHeight;
								}
							}
						}
					}
					else
					{
						newMap = 1.0f - (dist / radius);
						newMap *= 3.0f;
						if (newMap > 1.0f) newMap = 1.0f;

						if (currentHeight < newPos[2] + explosionDepth)
						{
							if (newPos[0] == 0 || newPos[1] == 0 ||
								newPos[0] == hmap.getWidth() -1 ||
								newPos[1] == hmap.getWidth() -1)
							{
							}
							else
							{
								newHeight = newPos[2] + explosionDepth;
							}
						}
					}

					if (newHeight != currentHeight)	hits = true;
					hmap.setHeight((int) newPos[0], (int) newPos[1], newHeight);
					map.map[x+iradius][y+iradius] = newMap;
				}
			}
		}
	}

	if (hits)
	{
		// Recalcualte the normals
		hmap.generateNormals(
			MAX(0, (int) (pos[0] - radius - 3.0f)), MIN(hmap.getWidth(), (int) (pos[0] + radius + 4.0f)),
			MAX(0, (int) (pos[1] - radius - 3.0f)), MIN(hmap.getWidth(), (int) (pos[1] + radius + 3.0f)));
	}

	return hits;
}

void DeformLandscape::flattenArea(ScorchedContext &context, Vector &tankPos)
{
	HeightMap &hmap = context.landscapeMaps->getHMap();
	int posX = (int) tankPos[0];
	int posY = (int) tankPos[1];

	// Flatten a small area around the tank
	for (int x=-2; x<=2; x++)
	{
		for (int y=-2; y<=2; y++)
		{
			int ix = posX + x;
			int iy = posY + y;
			if (ix >= 0 && iy >= 0 &&
				ix < hmap.getWidth() &&
				iy < hmap.getWidth())
			{
				hmap.setHeight(ix, iy, tankPos[2]);
			}
		}
	}

	// Recalcualte the normals
	hmap.generateNormals(
		MAX(0, posX - 3), MIN(hmap.getWidth(), posX + 3),
		MAX(0, posY - 3), MIN(hmap.getWidth(), posY + 3));
}

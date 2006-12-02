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

#include <landscapemap/DeformLandscape.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeDefinition.h>
#include <target/TargetContainer.h>
#include <target/TargetLife.h>
#include <engine/ScorchedContext.h>
#include <common/Defines.h>
#include <math.h>

bool DeformLandscape::deformLandscape(
	ScorchedContext &context,
	Vector &pos, float radius, bool down, DeformPoints &map,
	unsigned int playerId)
{
	HeightMap &hmap = context.landscapeMaps->getGroundMaps().getHeightMap();

	bool hits = false;
	int iradius = (int) radius + 1;
	if (iradius > 49) iradius = 49;

	float lowestHeight = 
		context.landscapeMaps->getDefinitions().getTex()->lowestlandheight;

	// Take out or add a chunk into the landsacpe
	for (int x=-iradius; x<=iradius; x++)
	{
		for (int y=-iradius; y<=iradius; y++)
		{
			map.map[x+iradius][y+iradius] = -1.0f;

			Vector newPos(pos[0] + x, pos[1] + y, pos[2]);
			if (((int) newPos[0] >=0) && (newPos[0] < hmap.getMapWidth()) &&
				((int) newPos[1] >=0) && (newPos[1] < hmap.getMapHeight()))
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
								newPos[0] == hmap.getMapWidth() -1 ||
								newPos[1] == hmap.getMapHeight() -1)
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
			MAX(0, (int) (pos[0] - radius - 3.0f)), MIN(hmap.getMapWidth(), (int) (pos[0] + radius + 4.0f)),
			MAX(0, (int) (pos[1] - radius - 3.0f)), MIN(hmap.getMapHeight(), (int) (pos[1] + radius + 3.0f)));
	}

	return hits;
}

void DeformLandscape::flattenArea(
	ScorchedContext &context, Vector &tankPos, unsigned int playerId,
	bool removeObjects, float size)
{
	int iSize = (int) size;
	HeightMap &hmap = context.landscapeMaps->getGroundMaps().getHeightMap();
	int posX = (int) tankPos[0];
	int posY = (int) tankPos[1];

	// Flatten a small area around the tank
	for (int x=-iSize; x<=iSize; x++)
	{
		for (int y=-iSize; y<=iSize; y++)
		{
			int ix = posX + x;
			int iy = posY + y;
			if (ix >= 0 && iy >= 0 &&
				ix < hmap.getMapWidth() &&
				iy < hmap.getMapHeight())
			{
				hmap.setHeight(ix, iy, tankPos[2]);
			}
		}
	}

	// Recalcualte the normals
	hmap.generateNormals(
		MAX(0, posX - iSize - 1), MIN(hmap.getMapWidth(), posX + iSize + 1),
		MAX(0, posY - iSize - 1), MIN(hmap.getMapHeight(), posY + iSize + 1));

	if (removeObjects)
	{
		// Get the tanks
		std::map<unsigned int, Target *> targets = // A Copy
			context.targetContainer->getTargets();
		std::map<unsigned int, Target *>::iterator targetItor;

		for (targetItor = targets.begin();
			targetItor != targets.end();
			targetItor++)
		{
			Target *target = (*targetItor).second;
			if (target->getAlive() &&
				target->isTarget())
			{
				Vector position = target->getTargetPosition();
				if (position[0] > tankPos[0] - size - target->getLife().getSize()[0] / 2.0f &&
					position[0] < tankPos[0] + size + target->getLife().getSize()[0] / 2.0f &&
					position[1] > tankPos[1] - size - target->getLife().getSize()[1] / 2.0f &&
					position[1] < tankPos[1] + size + target->getLife().getSize()[1] / 2.0f)
				{
					Target *removedTarget = 
						context.targetContainer->removeTarget(target->getPlayerId());
					delete removedTarget;
				}
			}
		}
	}
}


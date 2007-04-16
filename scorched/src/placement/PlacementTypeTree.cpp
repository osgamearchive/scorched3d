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

#include <placement/PlacementTypeTree.h>
#include <landscapemap/LandscapeMaps.h>
#include <engine/ScorchedContext.h>
#include <common/ProgressCounter.h>
#include <common/RandomGenerator.h>
#include <common/Defines.h>
#include <XML/XMLParser.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLImageFactory.h>

PlacementTypeTree::PlacementTypeTree() : 
	mincloseness(0.0f), maxobjects(2000)
{
}

PlacementTypeTree::~PlacementTypeTree()
{
}

bool PlacementTypeTree::readXML(XMLNode *node)
{
	node->getNamedChild("mask", mask, false);
	if (!node->getNamedChild("numobjects", numobjects)) return false;
	if (!node->getNamedChild("numclusters", numclusters)) return false;
	if (!node->getNamedChild("minheight", minheight)) return false;
	if (!node->getNamedChild("maxheight", maxheight)) return false;
	node->getNamedChild("mincloseness", mincloseness, false);
	node->getNamedChild("maxobjects", maxobjects, false);
	return PlacementType::readXML(node);
}

void PlacementTypeTree::getPositions(ScorchedContext &context,
	RandomGenerator &generator,
	std::list<Position> &returnPositions,
	ProgressCounter *counter)
{
	GLBitmap bmap(256, 256);
	GLImageHandle map = bmap;
	if (mask.c_str()[0])
	{	
		map = GLImageFactory::loadImageHandle(getDataFile(mask.c_str()));
		if (!map.getBits())
		{
			dialogExit("PlacementTypeTree",
				formatString("Error: failed to find mask \"%s\"",
				mask.c_str()));
		}
	}

	// Generate a map of where the trees should go
	unsigned char objectMap[64 * 64];
	memset(objectMap, 0, sizeof(unsigned char) * 64 * 64);

	int groundMapWidth = context.landscapeMaps->getGroundMaps().getMapWidth();
	int groundMapHeight = context.landscapeMaps->getGroundMaps().getMapHeight();

	// A few points where trees will be clustered around
	int treeMapMultWidth  = groundMapWidth / 64;
	int treeMapMultHeight = groundMapHeight / 64;
	for (int i=0; i<numclusters; i++)
	{
		// Get a random point
		int x = int(generator.getRandFloat() * 64.0f);
		int y = int(generator.getRandFloat() * 64.0f);

		// Check point is in the correct height band
		float height = 
			context.landscapeMaps->
				getGroundMaps().getHeight(
					x * treeMapMultWidth, y * treeMapMultHeight);
		Vector &normal =
			context.landscapeMaps->
				getGroundMaps().getNormal(
					x * treeMapMultWidth, y * treeMapMultHeight);

		int mx = int(map.getWidth() * (float(x) / 64.0f));
		int my = int(map.getHeight() * (float(y) / 64.0f));
		unsigned char *bits = map.getBits() +
			mx * 3 + my * map.getWidth() * 3;
		if (bits[0] > 127 &&
			height > minheight && 
			height < maxheight && 
			normal[2] > 0.7f)
		{
			// Group other areas around this point that are likely to get trees
			// Do a few groups
			int n = int(generator.getRandFloat() * 10) + 5;
			for (int j=0; j<n; j++)
			{
				// Check groups is within bounds
				int newX = x + int(generator.getRandFloat() * 8.0f) - 4;
				int newY = y + int(generator.getRandFloat() * 8.0f) - 4;
				if (newX >= 0 && newX < 64 &&
					newY >= 0 && newY < 64)
				{
					Vector &normal =
						context.landscapeMaps->
						getGroundMaps().getNormal(
							newX * treeMapMultWidth, newY * treeMapMultHeight);
					height = 
						context.landscapeMaps->
						getGroundMaps().getHeight(
							newX * treeMapMultWidth, newY * treeMapMultHeight);
					if (height > minheight && 
						height < maxheight && 
						normal[2] > 0.7f)
					{
						objectMap[newX + 64 * newY] = 64;
					}
				}
			}

			// Add the original point
			objectMap[x + 64 * y] = 255;
		}
	}

	// Smooth the treemap
	unsigned char objectMapCopy[64 * 64];
	memcpy(objectMapCopy, objectMap, sizeof(unsigned char) * 64 * 64);
	float matrix[3][3];
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
		{
			matrix[i][j] = 0.7f; // How much smoothing is done (> is more)
			if (i==1 && j==1) matrix[i][j] = 4.0f;
		}
	}
	for (int y=0; y<64; y++)
	{
		for (int x=0; x<64; x++)
		{
			// Total is used to catch corner cases
			float total = 0.0f;
			float inc = 0.0f;
			for (int i=0; i<3; i++)
			{
				for (int j=0; j<3; j++)
				{
					int newX = i + x - 1;
					int newY = j + y - 1;
					
					if (newX >= 0 && newX < 64 &&
						newY >= 0 && newY < 64)
					{
						inc += objectMapCopy[newX + 64 * newY];
						total += matrix[i][j];
					}
				}
			}
	 
			objectMap[x + 64 * y] = (unsigned char)(inc/total);
		}
	}

	// Add lots of trees, more chance of adding a tree where
	// the map is stongest
	int objectCount = 0;
	const int NoIterations = numobjects;
	for (int i=0; i<NoIterations && objectCount < maxobjects; i++)
	{
		if (i % 1000 == 0) if (counter) 
			counter->setNewPercentage(float(i)/float(NoIterations)*100.0f);

		float lx = generator.getRandFloat() * float(groundMapWidth);
		float ly = generator.getRandFloat() * float(groundMapHeight);
		int rx = int(lx);
		int ry = int(ly);
		int nx = rx / treeMapMultWidth;
		int ny = ry / treeMapMultHeight;
		int r = objectMap[nx + 64 * ny];
		int nr = int (generator.getRandFloat() * 512.0f);

		if (nr < r)
		{
			float height = 
				context.landscapeMaps->
					getGroundMaps().getInterpHeight(lx, ly);

			if (height > minheight + 0.5f)
			{
				objectCount ++;
				Position position;
				position.position[0] = lx;
				position.position[1] = ly;
				position.position[2] = height;
				if (checkCloseness(position.position, context, 
					returnPositions, mincloseness))
				{
					returnPositions.push_back(position);
				}
			}
		}
	}
}

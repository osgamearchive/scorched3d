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

#include <landscape/LandscapeObjectsPlacement.h>
#include <landscape/LandscapeObjectsEntryTree.h>
#include <landscape/LandscapeObjectsEntryModel.h>
#include <landscape/LandscapeObjects.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/LandscapeTex.h>
#include <landscape/Landscape.h>
#include <client/ScorchedClient.h>
#include <GLEXT/GLBitmapModifier.h>
#include <3dsparse/ModelStore.h>

void LandscapeObjectPlacementTrees::generateObjects(
	RandomGenerator &generator, 
	LandscapeTexObjectsPlacementTree &placement,
	ProgressCounter *counter)
{
	// Generate a map of where the trees should go
	unsigned char objectMap[64 * 64];
	memset(objectMap, 0, sizeof(unsigned char) * 64 * 64);

	// A few points where trees will be clustered around
	for (int i=0; i<placement.numclusters; i++)
	{
		// Get a random point
		int x = int(generator.getRandFloat() * 64.0f);
		int y = int(generator.getRandFloat() * 64.0f);

		// Check point is in the correct height band
		float height = 
			ScorchedClient::instance()->getLandscapeMaps().
				getHMap().getHeight(x * 4, y * 4);
		Vector &normal =
			ScorchedClient::instance()->getLandscapeMaps().
				getHMap().getNormal(x * 4, y * 4);
		if (height > placement.minheight && 
			height < placement.maxheight && 
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
						ScorchedClient::instance()->getLandscapeMaps().
						getHMap().getNormal(newX * 4, newY * 4);
					height = 
						ScorchedClient::instance()->getLandscapeMaps().
						getHMap().getHeight(newX * 4, newY *4);
					if (height > placement.minheight && 
						height < placement.maxheight && 
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

	bool pine = true;
	float snowHeight = 20.0f;
	GLVertexSet *model = 0;
	GLVertexSet *modelburnt = 0;
	if (0 == strcmp(placement.objecttype.c_str(), "tree"))
	{
		LandscapeTexObjectsTree *treeObjects = 
			(LandscapeTexObjectsTree *) placement.object;
		pine = (0 == strcmp(treeObjects->tree.c_str(), "pine"));
		snowHeight = treeObjects->snow;
	}
	else if (0 == strcmp(placement.objecttype.c_str(), "model"))
	{
		LandscapeTexObjectsModel *modelObjects = 
			(LandscapeTexObjectsModel *) placement.object;
		
		model = ModelStore::instance()->loadOrGetArray(
			modelObjects->model, true, true);
		modelburnt = ModelStore::instance()->loadOrGetArray(
			modelObjects->modelburnt, true, true);
		if (!model || !modelburnt)
		{
			dialogExit("LandscapeObjectPlacementTrees",
				"Failed to find models");
		}
	}
	else
	{
		dialogExit("LandscapeObjectPlacementTrees",
			"Error: Unknown model type \"%s\"",
			placement.objecttype.c_str());
	}

	// Add lots of trees, more chance of adding a tree where
	// the map is stongest
	const int NoIterations = placement.numobjects;
	for (int i=0; i<NoIterations; i++)
	{
		if (i % 1000 == 0) if (counter) 
			counter->setNewPercentage(float(i)/float(NoIterations)*100.0f);

		int x = int(RAND * 1023.0f);
		int y = int(RAND * 1023.0f);
		int nx = x / 16;
		int ny = y / 16;
		int r = objectMap[nx + 64 * ny];
		int nr = int (RAND * 512.0f);

		if (nr < r)
		{
			float lx = float(x) / 4.0f;
			float ly = float(y) / 4.0f;
			float height = 
				ScorchedClient::instance()->getLandscapeMaps().
					getHMap().getInterpHeight(lx, ly);

			if (height > placement.minheight + 0.5f)
			{
				float mult = (float) Landscape::instance()->getMainMap().getWidth() / 256.0f;
				GLBitmapModifier::addCircle(Landscape::instance()->getMainMap(),
					lx * mult, ly * mult, 2.5f * mult, 1.0f);

				LandscapeObjectsEntry *entry = 0;
				if (model)
				{
					entry = new LandscapeObjectsEntryModel;
					((LandscapeObjectsEntryModel *) entry)->model = model;
					((LandscapeObjectsEntryModel *) entry)->modelburnt = modelburnt;				
					entry->color = 1.0f;
					entry->size = 0.05f;
					entry->posX = lx;
					entry->posY = ly;
					entry->posZ = height;
					entry->rotation = RAND * 360.0f;
				}
				else
				{
					entry = new LandscapeObjectsEntryTree;
					((LandscapeObjectsEntryTree *) entry)->snow = (pine && 
						(height > snowHeight + (RAND * 10.0f) - 5.0f));
					((LandscapeObjectsEntryTree *) entry)->pine = pine;
					entry->color = RAND * 0.5f + 0.5f;
					entry->size =  RAND * 2.0f + 1.0f;
					entry->posX = lx;
					entry->posY = ly;
					entry->posZ = height;
					entry->rotation = RAND * 360.0f;
				}

				Landscape::instance()->getObjects().addObject(
					(unsigned int) lx,
					(unsigned int) ly,
					entry);
			}
		}
	}
}

void LandscapeObjectPlacementMask::generateObjects(
	RandomGenerator &generator, 
	LandscapeTexObjectsPlacementMask &placement,
	ProgressCounter *counter)
{
	bool pine = true;
	float snowHeight = 20.0f;
	GLVertexSet *model = 0;
	GLVertexSet *modelburnt = 0;
	if (0 == strcmp(placement.objecttype.c_str(), "tree"))
	{
		LandscapeTexObjectsTree *treeObjects = 
			(LandscapeTexObjectsTree *) placement.object;
		pine = (0 == strcmp(treeObjects->tree.c_str(), "pine"));
		snowHeight = treeObjects->snow;
	}
	else if (0 == strcmp(placement.objecttype.c_str(), "model"))
	{
		LandscapeTexObjectsModel *modelObjects = 
			(LandscapeTexObjectsModel *) placement.object;
		
		model = ModelStore::instance()->loadOrGetArray(
			modelObjects->model, true, true);
		modelburnt = ModelStore::instance()->loadOrGetArray(
			modelObjects->modelburnt, true, true);
		if (!model || !modelburnt)
		{
			dialogExit("LandscapeObjectPlacementMask",
				"Failed to find models");
		}
	}
	else
	{
		dialogExit("LandscapeObjectPlacementMask",
			"Error: Unknown model type \"%s\"",
			placement.objecttype.c_str());
	}

	GLBitmap map(getDataFile(placement.mask.c_str()));
	if (!map.getBits())
	{
		dialogExit("LandscapeObjectPlacementMask",
			"Error: failed to find mask \"%s\"",
			placement.mask.c_str());
	}

	const int NoIterations = placement.numobjects;
	for (int i=0; i<NoIterations; i++)
	{
		if (i % 1000 == 0) if (counter) 
			counter->setNewPercentage(float(i)/float(NoIterations)*100.0f);

		float lx = generator.getRandFloat() * 255.0f;
		float ly = generator.getRandFloat() * 255.0f;
		float lr = generator.getRandFloat() * 360.0f;
		
		if (placement.xsnap > 0.0f) 
		{
			lx = float(int(lx / placement.xsnap)) * placement.xsnap;
		}
		if (placement.ysnap > 0.0f)
		{
			ly = float(int(ly / placement.ysnap)) * placement.ysnap;
		}
		if (placement.angsnap > 0.0f)
		{
			lr = float(int(lr / placement.angsnap)) * placement.angsnap;
		}
		lx = MIN(MAX(0.0f, lx), 255.0f);
		ly = MIN(MAX(0.0f, ly), 255.0f);

		float height = 
			ScorchedClient::instance()->getLandscapeMaps().
				getHMap().getInterpHeight(lx, ly);
		Vector normal;
		ScorchedClient::instance()->getLandscapeMaps().
			getHMap().getInterpNormal(lx, ly, normal);
		if (height > placement.minheight && 
			height < placement.maxheight &&
			normal[2] > placement.minslope)
		{
				
			int mx = int(map.getWidth() * (lx / 255.0f));
			int my = int(map.getWidth() * (ly / 255.0f));
			GLubyte *bits = map.getBits() +
				mx * 3 + my * map.getWidth() * 3;
			if (bits[0] > 127)
			{
				bool closeness = true;
				if (placement.mincloseness > 0.0f)
				{
					float distsq = placement.mincloseness * placement.mincloseness;
					std::multimap<unsigned int, LandscapeObjectsEntry*> &entries =
						Landscape::instance()->getObjects().getEntries();
					std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator itor;
					for (itor = entries.begin();
						itor != entries.end();
						itor++)
					{
						LandscapeObjectsEntry *object = (*itor).second;
						float distx = object->posX - lx;
						float disty = object->posY - ly;
						if (distx * distx + disty *disty < distsq)
						{
							closeness = false;
							break;
						}
					}
				}
			
				if (closeness)
				{
					LandscapeObjectsEntry *entry = 0;
					if (model)
					{
						entry = new LandscapeObjectsEntryModel;
						((LandscapeObjectsEntryModel *) entry)->model = model;
						((LandscapeObjectsEntryModel *) entry)->modelburnt = modelburnt;				
						entry->color = 1.0f;
						entry->size = 0.05f;
						entry->posX = lx;
						entry->posY = ly;
						entry->posZ = height;
						entry->rotation = lr;
					}
					else
					{
						entry = new LandscapeObjectsEntryTree;
						((LandscapeObjectsEntryTree *) entry)->snow = (pine && 
							(height > snowHeight + (RAND * 10.0f) - 5.0f));
						((LandscapeObjectsEntryTree *) entry)->pine = pine;
						entry->color = RAND * 0.5f + 0.5f;
						entry->size =  RAND * 2.0f + 1.0f;
						entry->posX = lx;
						entry->posY = ly;
						entry->posZ = height;
						entry->rotation = lr;
					}

					Landscape::instance()->getObjects().addObject(
						(unsigned int) lx,
						(unsigned int) ly,
						entry);
				}
			}
		}
	}
}


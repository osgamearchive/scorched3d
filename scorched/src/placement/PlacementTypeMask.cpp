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

#include <placement/PlacementTypeMask.h>
#include <landscape/LandscapeMaps.h>
#include <engine/ScorchedContext.h>
#include <common/ProgressCounter.h>
#include <common/RandomGenerator.h>
#include <common/Defines.h>
#include <GLEXT/GLBitmap.h>
#include <XML/XMLParser.h>

PlacementTypeMask::PlacementTypeMask()
{
}

PlacementTypeMask::~PlacementTypeMask()
{
}

bool PlacementTypeMask::readXML(XMLNode *node)
{
	if (!node->getNamedChild("numobjects", numobjects)) return false;
	if (!node->getNamedChild("mask", mask)) return false;
	if (!node->getNamedChild("minheight", minheight)) return false;
	if (!node->getNamedChild("maxheight", maxheight)) return false;
	if (!node->getNamedChild("mincloseness", mincloseness)) return false;
	if (!node->getNamedChild("minslope", minslope)) return false;
	if (!node->getNamedChild("xsnap", xsnap)) return false;
	if (!node->getNamedChild("ysnap", ysnap)) return false;
	if (!node->getNamedChild("angsnap", angsnap)) return false;
	return PlacementType::readXML(node);
}

void PlacementTypeMask::getPositions(ScorchedContext &context,
	RandomGenerator &generator,
	std::list<Position> &returnPositions,
	ProgressCounter *counter)
{
	int groundMapWidth = context.landscapeMaps->getGroundMaps().getMapWidth();
	int groundMapHeight = context.landscapeMaps->getGroundMaps().getMapHeight();

	GLBitmap map(getDataFile(mask.c_str()));
	if (!map.getBits())
	{
		dialogExit("PlacementTypeMask",
			formatString("Error: failed to find mask \"%s\"",
			mask.c_str()));
	}

	const int NoIterations = numobjects;
	int objectCount = 0;
	for (int i=0; i<NoIterations; i++)
	{
		if (i % 1000 == 0) if (counter) 
			counter->setNewPercentage(float(i)/float(NoIterations)*100.0f);

		float lx = generator.getRandFloat() * float(groundMapWidth);
		float ly = generator.getRandFloat() * float(groundMapHeight);
		float lr = generator.getRandFloat() * 360.0f;
		
		if (xsnap > 0.0f) 
		{
			lx = float(int(lx / xsnap)) * xsnap;
		}
		if (ysnap > 0.0f)
		{
			ly = float(int(ly / ysnap)) * ysnap;
		}
		if (angsnap > 0.0f)
		{
			lr = float(int(lr / angsnap)) * angsnap;
		}
		lx = MIN(MAX(0.0f, lx), float(groundMapWidth));
		ly = MIN(MAX(0.0f, ly), float(groundMapHeight));

		float height = 
			context.landscapeMaps->
				getGroundMaps().getInterpHeight(lx, ly);
		Vector normal;
		context.landscapeMaps->
			getGroundMaps().getInterpNormal(lx, ly, normal);
		if (height > minheight && 
			height < maxheight &&
			normal[2] > minslope)
		{
				
			int mx = int(map.getWidth() * (lx / float(groundMapWidth)));
			int my = int(map.getWidth() * (ly / float(groundMapHeight)));
			GLubyte *bits = map.getBits() +
				mx * 3 + my * map.getWidth() * 3;
			if (bits[0] > 127)
			{
				bool closeness = true;
				if (mincloseness > 0.0f)
				{
					float distsq = mincloseness * mincloseness;
					std::multimap<unsigned int, LandscapeObjectsEntry*> &entries =
						context.landscapeMaps->getGroundMaps().getObjects().getEntries();
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
					Position position;
					position.position[0] = lx;
					position.position[1] = ly;
					position.position[2] = height;
					position.size = 0.05f;
					position.rotation = lr;
					returnPositions.push_back(position);
				}
			}
		}
	}
}
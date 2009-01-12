////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2009
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
#include <landscapemap/LandscapeMaps.h>
#include <engine/ScorchedContext.h>
#include <common/ProgressCounter.h>
#include <common/RandomGenerator.h>
#include <common/Defines.h>
#include <image/ImageFactory.h>
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
	return PlacementType::readXML(node);
}

void PlacementTypeMask::getPositions(ScorchedContext &context,
	RandomGenerator &generator,
	std::list<Position> &returnPositions,
	ProgressCounter *counter)
{
	int groundMapWidth = context.getLandscapeMaps().getGroundMaps().getLandscapeWidth();
	int groundMapHeight = context.getLandscapeMaps().getGroundMaps().getLandscapeHeight();

	ImageHandle map = ImageFactory::loadImageHandle(S3D::getDataFile(mask.c_str()));
	if (!map.getBits())
	{
		S3D::dialogExit("PlacementTypeMask",
			S3D::formatStringBuffer("Error: failed to find mask \"%s\"",
			mask.c_str()));
	}

	const int NoIterations = numobjects;
	int objectCount = 0;
	for (int i=0; i<NoIterations; i++)
	{
		if (i % 1000 == 0) if (counter) 
			counter->setNewPercentage(float(i)/float(NoIterations)*100.0f);

		fixed lx = generator.getRandFixed() * fixed(groundMapWidth);
		fixed ly = generator.getRandFixed() * fixed(groundMapHeight);
		
		if (xsnap > 0) 
		{
			lx = fixed((lx / xsnap).asInt()) * xsnap;
		}
		if (ysnap > 0)
		{
			ly = fixed((ly / ysnap).asInt()) * ysnap;
		}
		lx = MIN(MAX(fixed(0), lx), fixed(groundMapWidth));
		ly = MIN(MAX(fixed(0), ly), fixed(groundMapHeight));

		fixed height = 
			context.getLandscapeMaps().getGroundMaps().getInterpHeight(lx, ly);
		FixedVector normal;
		context.getLandscapeMaps().
			getGroundMaps().getInterpNormal(lx, ly, normal);
		if (height > minheight && 
			height < maxheight &&
			normal[2] > minslope)
		{
			int mx = (map.getWidth() * lx.asInt()) / groundMapWidth;
			int my = (map.getHeight() * ly.asInt()) / groundMapHeight;
			unsigned char *bits = map.getBits() +
				mx * 3 + my * map.getWidth() * 3;
			if (bits[0] > 127)
			{	
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
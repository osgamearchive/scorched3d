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

#include <landscape/MovementMap.h>
#include <landscape/LandscapeTex.h>
#include <landscape/LandscapeDefn.h>
#include <landscape/LandscapeMaps.h>
#include <engine/ScorchedContext.h>
#include <common/OptionsGame.h>
#include <tank/Tank.h>
#include <memory.h>

MovementMap::MovementMap(int width, int height) :
	width_(width), height_(height)
{
	entries_ = new MovementMapEntry[(width + 1) * (height + 1)];
	memset(entries_, 0, sizeof(MovementMapEntry) * (width + 1) * (height + 1));
}

MovementMap::~MovementMap()
{
	delete [] entries_;
	entries_ = 0;
}

unsigned int MovementMap::POINT_TO_UINT(unsigned int x, unsigned int y)
{
	return (x << 16) | (y & 0xffff);
}

MovementMap::MovementMapEntry &MovementMap::getEntry(int w, int h)
{ 
	if (w >= 0 && h >= 0 && w<=width_ && h<=width_)
	{
		return entries_[(width_+1) * h + w];
	}
	static MovementMapEntry entry(MovementMap::eNoMovement, 1000.0f, 0, 0);
	return entry;
}

void MovementMap::addPoint(unsigned int x, unsigned int y, 
					 float height, float dist,
					 std::list<unsigned int> &edgeList,
					 unsigned int sourcePt,
					 ScorchedContext &context,
					 float minHeight,
					 unsigned int epoc)
{
	// Check that we are not going outside the arena
	if (x < 0 || y < 0 ||
		x > (unsigned int) width_ ||
		y > (unsigned int) height_) return;

	// Check if we can already reach this point
	// Through a shorted already visited path
	// That is not a current edge point
	MovementMapEntry &priorEntry = getEntry(x, y);
	if (priorEntry.type == eMovement &&
		priorEntry.epoc < epoc)
	{
		return;
	}

	// Find how much the tank has to climb to reach this new point
	// check that this is acceptable
	float newHeight = context.landscapeMaps->getGroundMaps().getHeight(
		(int) x, (int) y);

	// Check water height 
	if (newHeight < minHeight) return; 

	// Check climing height
	float MaxTankClimbHeight = float(context.optionsGame->
		getMaxClimbingDistance()) / 10.0f;
	if (newHeight - height > MaxTankClimbHeight) return;

	// Check if we can also reach this point from another edge point
	if (priorEntry.epoc == epoc)
	{
		// Check if this prior edge is further than the current
		if (dist < priorEntry.dist)
		{
			// If so set the distance etc
			priorEntry = MovementMap::MovementMapEntry(
				MovementMap::eMovement,
				dist,
				sourcePt,
				epoc);
		}
	}
	else
	{
		// Add this new edge to the list of edges
		unsigned int pt = POINT_TO_UINT(x, y);

		// Set the distance etc
		edgeList.push_back(pt);
		priorEntry = MovementMap::MovementMapEntry(
			MovementMap::eMovement,
			dist,
			sourcePt,
			epoc);
	}
}

void MovementMap::calculateForTank(Tank *tank, ScorchedContext &context, bool maxFuel)
{
	// Check if the tank is buried and cannot move
	float landscapeHeight = context.landscapeMaps->getGroundMaps().getInterpHeight(
		tank->getPosition().getTankPosition()[0],
		tank->getPosition().getTankPosition()[1]);
	float tankHeight = 
		tank->getPosition().getTankPosition()[2];
	float MaxTankClimbHeight = float(context.optionsGame->
		getMaxClimbingDistance()) / 10.0f;
	if (landscapeHeight > tankHeight + MaxTankClimbHeight)
	{
		return;
	}

	// Calculate the water height
	float waterHeight = -10.0f;
	if (context.optionsGame->getMovementRestriction() ==
		OptionsGame::MovementRestrictionLand ||
		context.optionsGame->getMovementRestriction() ==
		OptionsGame::MovementRestrictionLandOrAbove)
	{
		LandscapeTex &tex = *context.landscapeMaps->getDefinitions().getTex();
		if (tex.border->getType() == LandscapeTexType::eWater)
		{
			LandscapeTexBorderWater *water = 
				(LandscapeTexBorderWater *) tex.border;

			waterHeight = water->height;
		}
	}

	if (context.optionsGame->getMovementRestriction() ==
		OptionsGame::MovementRestrictionLandOrAbove)
	{
		if (waterHeight > tank->getPosition().getTankPosition()[2] - 0.1f)
		{
			waterHeight = tank->getPosition().getTankPosition()[2] - 0.1f;
		}
	}

	// Setup movement variables
	unsigned int posX = (unsigned int) 
		tank->getPosition().getTankPosition()[0];
	unsigned int posY = (unsigned int) 
		tank->getPosition().getTankPosition()[1];
	float fuel = (float) tank->getAccessories().getFuel().getNoFuel();
	if (fuel == -1.0f) fuel = 40.0f;
	if (maxFuel) fuel = 90.0f;

	// Add this point to the movement map
	unsigned int epoc = 0;
	getEntry(posX, posY) = 
		MovementMap::MovementMapEntry(
			MovementMap::eMovement,
			0.0f,
			0,
			epoc);

	// And add it to the list of next edge points
	std::list<unsigned int> edgeList;
	unsigned int pt = POINT_TO_UINT(posX, posY);
	edgeList.push_back(pt);

	// Find all the edges for the current edges and so on
	while (!edgeList.empty())
	{
		epoc++;

		std::list<unsigned int> tmpEdgeList = edgeList;
		edgeList.clear();

		std::list<unsigned int>::iterator edgeItor;
		for (edgeItor = tmpEdgeList.begin();
			edgeItor != tmpEdgeList.end();
			edgeItor++)
		{
			unsigned int pt = (*edgeItor);
			unsigned int x = pt >> 16;
			unsigned int y = pt & 0xffff;

			MovementMapEntry &priorEntry = getEntry(x, y);
			float dist = priorEntry.dist;
			if (dist <= fuel)
			{
				float height = 
					context.landscapeMaps->getGroundMaps().getHeight(
					(int) x, (int) y);

				addPoint(x+1, y, height, dist + 1, edgeList, pt, context, waterHeight, epoc);
				addPoint(x, y+1, height, dist + 1, edgeList, pt, context, waterHeight, epoc);
				addPoint(x-1, y, height, dist + 1, edgeList, pt, context, waterHeight, epoc);
				addPoint(x, y-1, height, dist + 1, edgeList, pt, context, waterHeight, epoc);
				addPoint(x+1, y+1, height, dist + 1.4f, edgeList, pt, context, waterHeight, epoc);
				addPoint(x-1, y+1, height, dist + 1.4f, edgeList, pt, context, waterHeight, epoc);
				addPoint(x-1, y-1, height, dist + 1.4f, edgeList, pt, context, waterHeight, epoc);
				addPoint(x+1, y-1, height, dist + 1.4f, edgeList, pt, context, waterHeight, epoc);
			}
		}
	}
}

#include <landscape/Landscape.h>
#include <client/ScorchedClient.h>

void MovementMap::movementTexture()
{
	GLBitmap newMap(
		Landscape::instance()->getMainMap().getWidth(),
		Landscape::instance()->getMainMap().getHeight());

	float width = (float)
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapWidth();
	float height = (float)
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapHeight();

	GLubyte *src = Landscape::instance()->getMainMap().getBits();
	GLubyte *dest = newMap.getBits();
	for (int y=0; y<newMap.getHeight(); y++)
	{
		int posY = int(float(y) / float(newMap.getHeight()) * height);
		for (int x=0; x<newMap.getWidth(); x++)
		{
			int posX = int(float(x) / float(newMap.getWidth()) * width);

			if (getEntry(posX, posY).type == eMovement)
			{
				dest[0] = src[0];
				dest[1] = src[1];
				dest[2] = src[2];
			}
			else
			{
				dest[0] = src[0] / 4;
				dest[1] = src[1] / 4;
				dest[2] = src[2] / 4;
			}

			src+=3;
			dest+=3;
		}
	}

	Landscape::instance()->getMainTexture().replace(newMap, GL_RGB, false);
	Landscape::instance()->setTextureType(Landscape::eMovement);
}

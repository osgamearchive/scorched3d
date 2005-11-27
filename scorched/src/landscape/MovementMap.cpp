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
	static MovementMapEntry entry(MovementMap::eNoMovement, 1000.0f, 0);
	return entry;
}

void MovementMap::addPoint(unsigned int x, unsigned int y, 
					 float height, float dist,
					 std::map<unsigned int, MovementMap::MovementMapEntry> &edgeMap,
					 std::map<unsigned int, MovementMap::MovementMapEntry> &pointsMap,
					 unsigned int sourcePt,
					 ScorchedContext &context,
					 float minHeight)
{
	// Check that we are not going outside the arena
	if (x < 0 || y < 0 ||
		x > (unsigned int) width_ ||
		y > (unsigned int) height_) return;

	// Form this point
	unsigned int pt = POINT_TO_UINT(x, y);

	// Find how much the tank has to climb to reach this new point
	// check that this is acceptable
	float newHeight = context.landscapeMaps->getGroundMaps().getHeight(
		(int) x, (int) y);

	float MaxTankClimbHeight = float(context.optionsGame->
		getMaxClimbingDistance()) / 10.0f;
	if (newHeight - height > MaxTankClimbHeight) return;
	if (newHeight < minHeight) return; // Water height 

	// Check if we can already reach this point
	// Through a shorted already visited path
	std::map<unsigned int, MovementMap::MovementMapEntry>::iterator ptFind;
	ptFind = pointsMap.find(pt);
	if (ptFind != pointsMap.end()) return;

	// Check if we can also reach this point from another edge point
	std::map<unsigned int, MovementMap::MovementMapEntry>::iterator edgeFind;
	edgeFind = edgeMap.find(pt);
	if (edgeFind != edgeMap.end())
	{
		// Ensure that the sortest edge route is kept
		if (dist < (*edgeFind).second.dist)
		{
			// Update the newer shorter route
			edgeMap[pt] = MovementMap::MovementMapEntry(
				MovementMap::eMovement,
				dist,
				sourcePt);
		}
	}
	else
	{
		// Ensure this is the first route here add it
		edgeMap[pt] = MovementMap::MovementMapEntry(
				MovementMap::eMovement,
				dist,
				sourcePt);
	}
}

void MovementMap::calculateForTank(Tank *tank, ScorchedContext &context, bool maxFuel)
{
	// Check if the tank is buried and cannot move
	float landscapeHeight = context.landscapeMaps->getGroundMaps().getInterpHeight(
		tank->getPhysics().getTankPosition()[0],
		tank->getPhysics().getTankPosition()[1]);
	float tankHeight = 
		tank->getPhysics().getTankPosition()[2];
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
		if (waterHeight > tank->getPhysics().getTankPosition()[2] - 0.1f)
		{
			waterHeight = tank->getPhysics().getTankPosition()[2] - 0.1f;
		}
	}

	// Move
	unsigned int posX = (unsigned int) 
		tank->getPhysics().getTankPosition()[0];
	unsigned int posY = (unsigned int) 
		tank->getPhysics().getTankPosition()[1];
	float fuel = (float) tank->getAccessories().getFuel().getNoFuel();
	if (maxFuel) fuel = 90.0f;

	std::map<unsigned int, MovementMapEntry> edgeMap;
	std::map<unsigned int, MovementMapEntry> pointsMap;

	unsigned int pt = POINT_TO_UINT(posX, posY);
	edgeMap[pt] = MovementMap::MovementMapEntry(
				MovementMap::eMovement,
				0.0f,
				0);

	std::map<unsigned int, float> newEdges;
	while (!edgeMap.empty())
	{
		pointsMap.insert(edgeMap.begin(), edgeMap.end());
		std::map<unsigned int, MovementMapEntry> tmpEdgeMap = edgeMap;
		edgeMap.clear();

		std::map<unsigned int, MovementMapEntry>::iterator edgeItor;
		for (edgeItor = tmpEdgeMap.begin();
			edgeItor != tmpEdgeMap.end();
			edgeItor++)
		{
			float dist = (*edgeItor).second.dist;
			if (dist <= fuel)
			{
				unsigned int pt = (*edgeItor).first;
				unsigned int x = pt >> 16;
				unsigned int y = pt & 0xffff;

				float height = 
					context.landscapeMaps->getGroundMaps().getHeight(
					(int) x, (int) y);

				addPoint(x+1, y, height, dist + 1, edgeMap, pointsMap, pt, context, waterHeight);
				addPoint(x, y+1, height, dist + 1, edgeMap, pointsMap, pt, context, waterHeight);
				addPoint(x-1, y, height, dist + 1, edgeMap, pointsMap, pt, context, waterHeight);
				addPoint(x, y-1, height, dist + 1, edgeMap, pointsMap, pt, context, waterHeight);
				addPoint(x+1, y+1, height, dist + 1.4f, edgeMap, pointsMap, pt, context, waterHeight);
				addPoint(x-1, y+1, height, dist + 1.4f, edgeMap, pointsMap, pt, context, waterHeight);
				addPoint(x-1, y-1, height, dist + 1.4f, edgeMap, pointsMap, pt, context, waterHeight);
				addPoint(x+1, y-1, height, dist + 1.4f, edgeMap, pointsMap, pt, context, waterHeight);
			}
		}
	}

	std::map<unsigned int, MovementMapEntry>::iterator finalItor;
	for (finalItor = pointsMap.begin();
		finalItor != pointsMap.end();
		finalItor++)
	{
		unsigned int pt = (*finalItor).first;
		unsigned int x = pt >> 16;
		unsigned int y = pt & 0xffff;

		getEntry(x, y) = (*finalItor).second;
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

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

#include <movement/TargetMovementEntryShips.h>
#include <common/Defines.h>
#include <common/RandomGenerator.h>
#include <graph/OptionsDisplay.h>
#include <engine/ScorchedContext.h>
#include <target/Target.h>
#include <target/TargetLife.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeMovement.h>

TargetMovementEntryShips::TargetMovementEntryShips()
{
}

TargetMovementEntryShips::~TargetMovementEntryShips()
{
}

void TargetMovementEntryShips::generate(ScorchedContext &context, 
	RandomGenerator &random, LandscapeMovementType *movementType)
{
	int mapWidth = context.landscapeMaps->getGroundMaps().getMapWidth();
	int mapHeight = context.landscapeMaps->getGroundMaps().getMapHeight();
	LandscapeTex &tex = *context.landscapeMaps->getDefinitions().getTex();

	// Get the water height (if water is on)
	float waterHeight = 0.0f;
	if (tex.border->getType() == LandscapeTexType::eWater)
	{
		LandscapeTexBorderWater *water = 
			(LandscapeTexBorderWater *) tex.border;
       	waterHeight = water->height;
	}

	// Create the spline path the ships will move on
	// Do this from the set of control points specified in the xml file
	LandscapeMovementTypeShips *shipGroup = 
		(LandscapeMovementTypeShips *) movementType;
	std::vector<Vector> controlPoints;
	controlPoints.push_back(Vector::nullVector);
	float diff = 360.0f / float(shipGroup->controlpoints);
	for (float i=0.0f; i<360.0f; i+=diff)
	{
		float distWidth = random.getRandFloat() * 
			shipGroup->controlpointsrand + shipGroup->controlpointswidth;
		float distHeight = random.getRandFloat() * 
			shipGroup->controlpointsrand + shipGroup->controlpointsheight;
		float x = getFastSin(i / 180.0f * PI) * distWidth + float(mapWidth) / 2.0f;
		float y = getFastCos(i / 180.0f * PI) * distHeight + float(mapHeight) / 2.0f;

		Vector pt(x,y,waterHeight);
		controlPoints.push_back(pt);
	}

	// Add a control point at the end to join the loop
	Vector midPt = (controlPoints[1] + controlPoints.back()) / 2.0;
	controlPoints.push_back(midPt);
	controlPoints.front() = midPt;

	// Generate the spline path
	path_.generate(controlPoints, 200, 3, shipGroup->speed);
	path_.simulate(shipGroup->starttime);

	// Find the group to move the objects in
	groupEntry_ = context.landscapeMaps->getGroundMaps().getGroups().
		getGroup(shipGroup->groupname.c_str());
	if (!groupEntry_)
	{
		dialogExit("TargetMovementEntryShips", 
			formatString("Group entry %s has no objects defined for it", 
			shipGroup->groupname.c_str()));
	}

	// Generate the list of offsets for all of the targets in the group
	std::map<unsigned int, TargetGroupEntry *> &objects = groupEntry_->getObjects();
	std::map<unsigned int, TargetGroupEntry *>::iterator itor;
	for (itor = objects.begin();
		itor != objects.end();
		itor++)
	{
		unsigned int playerId = (*itor).first;
		TargetGroupEntry *entry = (*itor).second;

		if (!entry->getTarget()->isTarget())
		{
			dialogMessage("TargetMovementEntryShips",
				"Movement can be assigned to targets only (no tanks)");
		}

		float offX = random.getRandFloat() * 200.0f;
		float offY = random.getRandFloat() * 200.0f;
		Vector offset(offX, offY - 100.0f);
		offsets_[playerId] = offset;
	}
}

void TargetMovementEntryShips::simulate(float frameTime)
{
	// Update the position of all of the ships along the path
	path_.simulate(frameTime);

	// Get the position and direction along the current ship path
	Vector position;
	Vector direction;
	path_.getPathAttrs(position, direction);
	Vector directionPerp = direction.get2DPerp();

	// For each target set position and rotation based on its offset
	std::map<unsigned int, TargetGroupEntry *> &objects = groupEntry_->getObjects();
	std::map<unsigned int, TargetGroupEntry *>::iterator itor;
	for (itor = objects.begin();
		itor != objects.end();
		itor++)
	{
		unsigned int playerId = (*itor).first;
		TargetGroupEntry *groupEntry = (*itor).second;
		
		// Find the offset for this player
		std::map<unsigned int, Vector>::iterator findItor =
			offsets_.find(playerId);
		if (findItor != offsets_.end())
		{
			// Calculate position
			Vector &offset = (*findItor).second;
			Vector shipPosition = position;
			shipPosition += directionPerp * -offset[0];
			shipPosition += direction * offset[1];
			shipPosition[2] -= 1.0f;
			float angle = atan2f(direction[1], direction[0]);
			float angleDegs = (angle / 3.14f) * 180.0f - 90.0f;

			// Update target
			groupEntry->getTarget()->getLife().setTargetPosition(shipPosition);
			groupEntry->getTarget()->getLife().setRotation(angleDegs);
		}
	}
}

bool TargetMovementEntryShips::writeMessage(NetBuffer &buffer)
{
	float pathTime = path_.getPathTime();
	buffer.addToBuffer(pathTime);
	return true;
}

bool TargetMovementEntryShips::readMessage(NetBufferReader &reader)
{
	float pathTime = 0.0f;
	if (!reader.getFromBuffer(pathTime)) return false;
	path_.setPathTime(pathTime);
	return true;
}

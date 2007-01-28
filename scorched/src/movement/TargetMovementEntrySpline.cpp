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

#include <movement/TargetMovementEntrySpline.h>
#include <common/Defines.h>
#include <common/RandomGenerator.h>
#include <graph/OptionsDisplay.h>
#include <engine/ScorchedContext.h>
#include <target/Target.h>
#include <target/TargetLife.h>
#include <target/TargetState.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeMovement.h>

TargetMovementEntrySpline::TargetMovementEntrySpline() : context_(0)
{
}

TargetMovementEntrySpline::~TargetMovementEntrySpline()
{
}

void TargetMovementEntrySpline::generate(ScorchedContext &context, 
	RandomGenerator &random, LandscapeMovementType *movementType)
{
	// Create the spline path the target will move on
	// Do this from the set of control points specified in the xml file
	LandscapeMovementTypeSpline *splineGroup = 
		(LandscapeMovementTypeSpline *) movementType;
	context_ = &context;
	groundOnly_ = splineGroup->groundonly;

	// Create the control points from those specified
	std::vector<Vector> controlPoints;
	controlPoints.push_back(Vector::nullVector);
	controlPoints.insert(controlPoints.end(), splineGroup->points.begin(), splineGroup->points.end());

	// Add a control point at the end to join the loop
	Vector midPt = (controlPoints[1] + controlPoints.back()) / 2.0;
	controlPoints.push_back(midPt);
	controlPoints.front() = midPt;

	// This is done mainly so they are draw correctly for debug
	if (groundOnly_) 
	{
		std::vector<Vector>::iterator itor;
		for (itor = controlPoints.begin();
			itor != controlPoints.end();
			itor++)
		{
			Vector &point = (*itor);
			point[2] = context.landscapeMaps->getGroundMaps().getInterpHeight(
				point[0], point[1]);
		}
	}

	// Generate the spline path
	path_.generate(controlPoints, 200, 3, splineGroup->speed);
	path_.simulate(splineGroup->starttime);

	// Find the group to move the objects in
	groupEntry_ = context.landscapeMaps->getGroundMaps().getGroups().
		getGroup(splineGroup->groupname.c_str());
	if (!groupEntry_)
	{
		dialogExit("TargetMovementEntrySpline", 
			formatString("Group entry %s has no objects defined for it", 
			splineGroup->groupname.c_str()));
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
			dialogExit("TargetMovementEntrySpline",
				"Movement can be assigned to targets only (no tanks)");
		}

		// Set this target as moving
		entry->getTarget()->getTargetState().setMovement(true);
	}
}

void TargetMovementEntrySpline::simulate(float frameTime)
{
	// Update the position of all of the targets along the path
	path_.simulate(frameTime);

	// Get the position and direction along the current path
	Vector position;
	Vector direction;
	path_.getPathAttrs(position, direction);
	Vector directionPerp = direction.get2DPerp();

	// Move the position to the ground if set
	if (groundOnly_)
	{
		position[2] = context_->landscapeMaps->getGroundMaps().getInterpHeight(
			position[0], position[1]);
	}

	// For each target set position and rotation based on its offset
	std::map<unsigned int, TargetGroupEntry *> &objects = groupEntry_->getObjects();
	std::map<unsigned int, TargetGroupEntry *>::iterator itor;
	for (itor = objects.begin();
		itor != objects.end();
		itor++)
	{
		unsigned int playerId = (*itor).first;
		TargetGroupEntry *groupEntry = (*itor).second;
		
		float angle = atan2f(direction[1], direction[0]);
		float angleDegs = (angle / 3.14f) * 180.0f - 90.0f;

		// Update target
		groupEntry->getTarget()->getLife().setTargetPosition(position);
		groupEntry->getTarget()->getLife().setRotation(angleDegs);
	}
}

bool TargetMovementEntrySpline::writeMessage(NetBuffer &buffer)
{
	float pathTime = path_.getPathTime();
	buffer.addToBuffer(pathTime);
	return true;
}

bool TargetMovementEntrySpline::readMessage(NetBufferReader &reader)
{
	float pathTime = 0.0f;
	if (!reader.getFromBuffer(pathTime)) return false;
	path_.setPathTime(pathTime);
	return true;
}

void TargetMovementEntrySpline::draw()
{
#ifndef S3D_SERVER
	path_.draw();
#endif
}

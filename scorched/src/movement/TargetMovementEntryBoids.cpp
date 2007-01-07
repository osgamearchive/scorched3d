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

#include <movement/TargetMovementEntryBoids.h>
#include <movement/BoidsObstacle.h>
#include <movement/Boid.h>
#include <common/Defines.h>
#include <common/RandomGenerator.h>
#include <graph/OptionsDisplay.h>
#include <engine/ScorchedContext.h>
#include <target/Target.h>
#include <target/TargetLife.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeMovement.h>

TargetMovementEntryBoids::TargetMovementEntryBoids() :
	visibilityMatrix_(0)
{
}

TargetMovementEntryBoids::~TargetMovementEntryBoids()
{
	{
		for (unsigned int i=0; i<boids_.size(); i++) 
		{
			delete [] visibilityMatrix_[i];
			delete boids_[i];
		}
		delete [] visibilityMatrix_;
		visibilityMatrix_ = 0;
		boids_.clear();
		boidsMap_.clear();
	}
	{
		while (!obstacles_.empty())
		{
			Obstacle *obstacle = obstacles_.back();
			obstacles_.pop_back();
			delete obstacle;
		}
	}
}

void TargetMovementEntryBoids::generate(ScorchedContext &context, 
	RandomGenerator &random, LandscapeMovementType *movementType)
{
	LandscapeMovementTypeBoids *boids = 
		(LandscapeMovementTypeBoids *) movementType;

	// Set the boid parameters
	cruiseDistance_ = 0.75f * boids->cruisedistance;
	maxVelocity_ = 1.0f * boids->maxvelocity;
	maxAcceleration_ = 0.15f * boids->maxacceleration;

	// Find the group to move the objects in
	groupEntry_ = context.landscapeMaps->getGroundMaps().getGroups().
		getGroup(boids->groupname.c_str());
	if (!groupEntry_)
	{
		dialogExit("TargetMovementEntryBoids", 
			formatString("Group entry %s has no objects defined for it", 
			boids->groupname.c_str()));
	}

	// Create boids
	makeBoids(context, random, boids->maxbounds, boids->minbounds);
	makeObstacles(context, random, boids->maxbounds, boids->minbounds);

	// Allocate a new visibility matrix
	visibilityMatrix_ = new int*[boids_.size()];
	for (unsigned int i=0; i<boids_.size(); i++)
	{
		visibilityMatrix_[i] = new int[boids_.size()];
		for (unsigned int j=0; j<boids_.size(); j++)
		{
			visibilityMatrix_[i][j] = -1;
		}
	}
}

void TargetMovementEntryBoids::makeBoids(ScorchedContext &context, 
	RandomGenerator &random, Vector &maxBounds, Vector &minBounds)
{
	BoidVector p;
	BoidVector attitude;      // roll, pitch, yaw
	BoidVector v;             // velocity vector

	// Generate the list of offsets for all of the targets in the group
	int i = 0;
	std::map<unsigned int, TargetGroupEntry *> &objects = groupEntry_->getObjects();
	std::map<unsigned int, TargetGroupEntry *>::iterator itor;
	for (i=0, itor = objects.begin();
		itor != objects.end();
		itor++, i++)
	{
		unsigned int playerId = (*itor).first;
		TargetGroupEntry *groupEntry = (*itor).second;

		if (!groupEntry->getTarget()->isTarget())
		{
			dialogMessage("TargetMovementEntryBoids",
				"Movement can be assigned to targets only (no tanks)");
		}

		// Add to world
		Boid *boid = new Boid(groupEntry->getTarget(), this, i + 1);
		boids_.push_back(boid);
		boidsMap_[playerId] = boid;
	}
}

void TargetMovementEntryBoids::makeObstacles(ScorchedContext &context, 
	RandomGenerator &random, Vector &maxBounds, Vector &minBounds) 
{
	BoidsObstacle *o = 
		new BoidsObstacle(context, maxBounds, minBounds);
	getObstacles().push_back(o);
}

void TargetMovementEntryBoids::simulate(float frameTime)
{
	// For each target set position and rotation based on its offset
	std::map<unsigned int, TargetGroupEntry *> &objects = groupEntry_->getObjects();
	std::map<unsigned int, TargetGroupEntry *>::iterator itor;
	for (itor = objects.begin();
		itor != objects.end();
		itor++)
	{
		unsigned int playerId = (*itor).first;
		TargetGroupEntry *groupEntry = (*itor).second;
		
		// Find the boid for this target
		std::map<unsigned int, Boid *>::iterator findItor = boidsMap_.find(playerId);
		if (findItor != boidsMap_.end())
		{
			// Update boid and target
			Boid *boid = (*findItor).second;
			boid->update((double) frameTime);
		}
	}
}

bool TargetMovementEntryBoids::writeMessage(NetBuffer &buffer)
{
	// No serialization needed as the boid movement code 
	// contains no random factors!
	return true;
}

bool TargetMovementEntryBoids::readMessage(NetBufferReader &reader)
{
	// No serialization needed as the boid movement code 
	// contains no random factors!
	return true;
}

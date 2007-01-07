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

#if !defined(__INCLUDE_TargetMovementEntryBoidsh_INCLUDE__)
#define __INCLUDE_TargetMovementEntryBoidsh_INCLUDE__

#include <common/SplinePath.h>
#include <movement/TargetMovementEntry.h>
#include <map>
#include <vector>

class Obstacle;
class Boid;
class TargetGroupsGroupEntry;
class TargetMovementEntryBoids : public TargetMovementEntry
{
public:
	TargetMovementEntryBoids();
	virtual ~TargetMovementEntryBoids();

	// Overridden from TargetMovementEntry
	virtual void generate(ScorchedContext &context, 
		RandomGenerator &random, 
		LandscapeMovementType *movementType);
	virtual void simulate(float frameTime);
	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);

	// Accessors for boids
	float getCruiseDistance() { return cruiseDistance_; }
	float getMaxVelocity() { return maxVelocity_; }
	float getMaxAcceleration() { return maxAcceleration_; }
	std::vector<Boid *> &getBoids() { return boids_; }
	std::vector<Obstacle *> &getObstacles() { return obstacles_; }
	int getBoidCount() { return (int) boids_.size(); } 
	int **getVisibilityMatrix() { return visibilityMatrix_; }

protected:
	TargetGroupsGroupEntry *groupEntry_;

	// All boids have access to this list, and use it to 
	// determine where all the other boids are. 
	std::vector<Boid *> boids_;
	std::map<unsigned int, Boid *> boidsMap_;

	// Linked list of obstacles that every boid will try to avoid. By default
	// there are no obstacles.
	std::vector<Obstacle *> obstacles_;

	// See description in CalculateVisibilityMatrix() and the comments for the
	// visMatrix macro for more info.
	int **visibilityMatrix_;

	// Boid settings
	float cruiseDistance_;
	float maxVelocity_;
	float maxAcceleration_;

	void makeBoids(ScorchedContext &context, RandomGenerator &random,
		Vector &maxBounds, Vector &minBounds);
	void makeObstacles(ScorchedContext &context, RandomGenerator &random,
		Vector &maxBounds, Vector &minBounds);
};

#endif // __INCLUDE_TargetMovementEntryBoidsh_INCLUDE__

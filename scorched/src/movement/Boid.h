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

/*
 * Copyright (C) 1996, Christopher John Kline
 * Electronic mail: ckline@acm.org
 *
 * This software may be freely copied, modified, and redistributed
 * for academic purposes and by not-for-profit organizations, provided that
 * this copyright notice is preserved on all copies, and that the source
 * code is included or notice is given informing the end-user that the source
 * code is publicly available under the terms described here.
 *
 * Persons or organizations wishing to use this code or any modified version
 * of this code in a commercial and/or for-profit manner must contact the
 * author via electronic mail (preferred) or other method to arrange the terms
 * of usage. These terms may be as simple as giving the author visible credit
 * in the final product.
 *
 * There is no warranty or other guarantee of fitness for this software,
 * it is provided solely "as is". Bug reports or fixes may be sent
 * to the author, who may or may not act on them as he desires.
 *
 * If you use this software the author politely requests that you inform him via
 * electronic mail. 
 *
 */

 // Boid.h
//
// INTERFACE: Class for objects that move with A-Life boid-type behavior.
//
// (c) 1996 Christopher Kline <ckline@acm.org>
//
// $Id$

#ifndef __BOID_H
#define __BOID_H

#include <limits.h>
#include <float.h>
#include <movement/TargetMovementEntryBoids.h>
#include <movement/BoidsObstacle.h>
#include <target/Target.h>
#include <target/TargetLife.h>

class Boid {
  
public:
  					 
  Boid(Target *target, TargetMovementEntryBoids *world); 
  // Constructor. Creates a bird of type NORMAL with the given dimensions,
  // initial position, and velocity.
  // bDimensions.{x,y,z} = {length, width, height}
  ~Boid();

  bool update(const double &elapsedSeconds);  
  // Updates this object based on the amount of elapsed time since the last
  // update, and the previous acceleration and velocity.

  void clearTarget();
  // Called when the target is destroyed

protected:

  BoidVector getPosition();
  BoidVector getVelocity();

  double getGravAcceleration(void) const;		
  // Returns the magnitude of gravitational acceleration in the (0, -1, 0)
  // direction [m/(sec^2)]. 

  double accumulate(BoidVector &accumulator, BoidVector valueToAdd);
  // Given an accumulator and a value to add to the accumulator, this
  // method truncates the magnitude of valueToAdd so that, when added to the
  // accumulator, the magnitude of the accumulator is at most 1.0. It then
  // adds the truncated value to the accumulator. The value returned is the
  // magnitude of the accumulator after the addition. 

  float getProbeLength(void);
  // Returns how far in front of boid to probe for obstacles. By default,
  // the probe length scales linearly from 10 times bodylength to 50 times
  // bodylength as the boid accelerates from 0 m/s to maxVelocity.

  double desiredCruisingSpeed();
  // Returns the speed the boid would like to travel at when not under any
  // other influences (i.e., obstacles, flocking desires, etc). The default
  // value is 1/5 of maxVelocity.
  
  void calculateVisibilityMatrix();
  // Each boid helps maintain a visibility matrix, which is an NxN matrix,
  // where N is the current number of boids (it is dynamically expanded each
  // time a new boid is created). Each cell [A,B] represents whether boid A can
  // see boid B. The contents of the matrix are described further in the
  // visMatrix macro in boid.c++
  // The reason for this matrix is to drastically reduce the computational
  // complexity of determining which boids are visible to the others.
  
  int visibleToSelf(Boid *b);
  // Returns 1 if this boid can see boid b, 0 otherwise.
  
  void calculateRollPitchYaw(
					BoidVector appliedAcceleration,
				     BoidVector currentVelocity,
				     BoidVector currentPosition);
  // Calculate the roll, pitch, and yaw of this boid based on its
  // acceleration, velocity, and position. Though position isn't necessary
  // for most approximations of attitude, it may be useful in some
  // circumstances. 

  BoidVector levelFlight(BoidVector AccelSoFar);
  // Returns a BoidVector which indicates how the boid would like to accelerate
  // in order to fly level (i.e., with minimal pitch). 
  
  BoidVector wander();
  // Returns a BoidVector which indicates how the boid would like to accelerate
  // when not under any other influences. Related to desiredCruisingSpeed().

  BoidVector collisionAvoidance();
  // Returns a BoidVector which indicates how the boid would like to accelerate
  // in order to avoid collisions with non-boid obstacles.

  BoidVector resolveCollision(BoidVector pointOnObject, BoidVector normalToObject);
  // Called by CollisionAvoidance, this method attempts to avoid a collision
  // with a specific obstacle, and returns an acceleration BoidVector indicating
  // how the boid should accelerate to achieve this end.

  BoidVector maintainingCruisingDistance();
  // Returns a BoidVector which indicates how the boid would like to accelerate
  // in order to maintain a distance of cruiseDistance from the nearest
  // visible boid. 

  BoidVector velocityMatching();
  // Returns a BoidVector which indicates how the boid would like to accelerate
  // in order to fly at approximately the same speed and direction as the
  // nearby boids.

  BoidVector flockCentering();
  // Returns a BoidVector which indicates how the boid would like to accelerate
  // in order to be near the center of the flock.

  BoidVector navigator();
  // This method prioritizes and resolves the acceleration BoidVectors from
  // CollisionAvoidance(), FlockCentering(), MaintainingCruisingDistance(),
  // VelocityMatching(), Wander(), and LevelFlight(). It returns the actual
  // acceleration BoidVector that the boid will apply to its flight in the
  // current time step.
  
private:

  struct VisibilityEntry
  {
	  int visible;
	  Boid *boid;
  }
  *visibilityMatrix;
  // Returns the value in the visibility matrix which determines if this boid
  // can see the "otherBoid". A value of -1 means that otherBoid cannot be
  // seen; any other value indicates that otherBoid _can_ be seen, and the
  // value is equal to the otherBoid's boidType.

  TargetMovementEntryBoids *world;
  // All visible boids + obstacles

  Target *target;
  // The target to update for this boid
};

// ------------------------------------------------ inline methods ------------------------------------------------

inline double
Boid::getGravAcceleration(void) const
{
  return 9.806650;
}

inline int  
Boid::visibleToSelf(Boid *b)
{
  // find out if the boid b is within our field of view
  BoidVector BoidVectorToObject = b->getPosition() - getPosition();
  
  // This isn't perfectly accurate, since we're not always facing in
  // the direction of our velocity, but it's close enough.
  return(AngleBetween(getVelocity(), BoidVectorToObject) <= 1.0471967);  // pi/3 radians is our FOV
}

inline float
Boid::getProbeLength(void)
{

  float maxScale = 5;

  // When we're at maxVelocity, scalefactor = maxScale.
  // When our velocity is 0, scalefactor = 1.
  // Linearly scale in between.
  float scaleFactor = float(((maxScale-1)/world->getMaxVelocity()) * Magnitude(getVelocity()) + 1);

  return float(10.0f*scaleFactor);

}

inline BoidVector Boid::getPosition()
{
	BoidVector result;
	if (target)
	{
		result.x = target->getLife().getTargetPosition()[0];
		result.y = target->getLife().getTargetPosition()[2];
		result.z = target->getLife().getTargetPosition()[1];
	}
	return result;
}

inline BoidVector Boid::getVelocity()
{
	BoidVector result;
	if (target)
	{
		result.x = target->getLife().getVelocity()[0];
		result.y = target->getLife().getVelocity()[2];
		result.z = target->getLife().getVelocity()[1];
	}
	return result;
}

inline double
Boid::desiredCruisingSpeed(void) {

  return 0.2*world->getMaxVelocity();
}

inline BoidVector 
Boid::levelFlight(BoidVector AccelSoFar) {

  // Determine the vertical acceleration.
  BoidVector verticalAcc(0, AccelSoFar.y, 0);

  // Try to negate it.
  return -verticalAcc;
}

inline void 
Boid::clearTarget()
{
	target = 0;
}

#endif /* __BOID_H */

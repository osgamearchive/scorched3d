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
#include "BoidWorld.h"
#include "SimObject.h"
#include "Obstacle.h"

class Boid : public SimObject {
  
public:
  
  enum boidTypes {
    NORMAL = 0,
    PREDATOR,
    PREY
  };
							 
  Boid(BoidWorld *world, int boidNumber, 
	  BoidVector bPosition, BoidVector bVelocity, BoidVector bDimensions); 
  // Constructor. Creates a bird of type NORMAL with the given dimensions,
  // initial position, and velocity.
  // bDimensions.{x,y,z} = {length, width, height}

  virtual bool update(const double &elapsedSeconds);  
  // Updates this object based on the amount of elapsed time since the last
  // update, and the previous acceleration and velocity.

  void updateslow();
    
  double maxVelocity;
  // [m/sec] Maximum magnitude of velocity. Default value is 10

  double maxAcceleration;
  // [m/(sec^2)] Maximum magnitude of acceleration as a fraction of
  // maxVelocity. Default value is 0.5 
  
  double cruiseDistance;
  // [m] Desired distance from closest neighbor when flying. Default value
  // is twice the bodylength.

  double dampedroll;

  double roll;
  // [radians] Rotation around body-local z-axis (+roll =
  // counterclockwise). Default value is 0.

  double pitch;
  // [radians] Rotation around body-local x-axis (+pitch = nose tilting
  // upward). Default value is 0.

  double yaw;
  // [radians] Rotation around body-local y-axis (increasing
  // counterclockwise, 0 is along body-local +z). Default value is 0. 

protected:

  virtual double getGravAcceleration(void) const;		
  // Returns the magnitude of gravitational acceleration in the (0, -1, 0)
  // direction [m/(sec^2)]. 

  virtual double accumulate(BoidVector &accumulator, BoidVector valueToAdd);
  // Given an accumulator and a value to add to the accumulator, this
  // method truncates the magnitude of valueToAdd so that, when added to the
  // accumulator, the magnitude of the accumulator is at most 1.0. It then
  // adds the truncated value to the accumulator. The value returned is the
  // magnitude of the accumulator after the addition. 

  virtual float getProbeLength(void);
  // Returns how far in front of boid to probe for obstacles. By default,
  // the probe length scales linearly from 10 times bodylength to 50 times
  // bodylength as the boid accelerates from 0 m/s to maxVelocity.

  virtual double desiredCruisingSpeed(void);
  // Returns the speed the boid would like to travel at when not under any
  // other influences (i.e., obstacles, flocking desires, etc). The default
  // value is 1/5 of maxVelocity.
  
  virtual void calculateVisibilityMatrix(void);
  // Each boid helps maintain a visibility matrix, which is an NxN matrix,
  // where N is the current number of boids (it is dynamically expanded each
  // time a new boid is created). Each cell [A,B] represents whether boid A can
  // see boid B. The contents of the matrix are described further in the
  // visMatrix macro in boid.c++
  // The reason for this matrix is to drastically reduce the computational
  // complexity of determining which boids are visible to the others.
  
  virtual int visibleToSelf(Boid *b);
  // Returns 1 if this boid can see boid b, 0 otherwise.
  
  virtual void calculateRollPitchYaw(BoidVector appliedAcceleration,
				     BoidVector currentVelocity,
				     BoidVector currentPosition);
  // Calculate the roll, pitch, and yaw of this boid based on its
  // acceleration, velocity, and position. Though position isn't necessary
  // for most approximations of attitude, it may be useful in some
  // circumstances. 

  virtual BoidVector levelFlight(BoidVector AccelSoFar);
  // Returns a BoidVector which indicates how the boid would like to accelerate
  // in order to fly level (i.e., with minimal pitch). 
  
  virtual BoidVector wander(void);
  // Returns a BoidVector which indicates how the boid would like to accelerate
  // when not under any other influences. Related to desiredCruisingSpeed().

  virtual BoidVector collisionAvoidance(void);
  // Returns a BoidVector which indicates how the boid would like to accelerate
  // in order to avoid collisions with non-boid obstacles.

  virtual BoidVector resolveCollision(BoidVector pointOnObject, BoidVector normalToObject);
  // Called by CollisionAvoidance, this method attempts to avoid a collision
  // with a specific obstacle, and returns an acceleration BoidVector indicating
  // how the boid should accelerate to achieve this end.

  virtual BoidVector maintainingCruisingDistance(void);
  // Returns a BoidVector which indicates how the boid would like to accelerate
  // in order to maintain a distance of cruiseDistance from the nearest
  // visible boid. 

  virtual BoidVector velocityMatching(void);
  // Returns a BoidVector which indicates how the boid would like to accelerate
  // in order to fly at approximately the same speed and direction as the
  // nearby boids.

  virtual BoidVector flockCentering(void);
  // Returns a BoidVector which indicates how the boid would like to accelerate
  // in order to be near the center of the flock.

  virtual BoidVector navigator(void);
  // This method prioritizes and resolves the acceleration BoidVectors from
  // CollisionAvoidance(), FlockCentering(), MaintainingCruisingDistance(),
  // VelocityMatching(), Wander(), and LevelFlight(). It returns the actual
  // acceleration BoidVector that the boid will apply to its flight in the
  // current time step.
  
  virtual int getBoidType(void) const;
  // Returns the type of this boid. 
  
  double bodyLength;
  // [m] Length of the boid. By default this value is equal to the z
  // component of the bDimensions passed to the constructor.

  double time;
  // [sec] current time				

  int boidNumber;
  // Unique integer identifying the number of this boid. The first boid
  // created is given boidNumber 1, and the values increase sequentially.

  char bName[50];
  // Name of self. This is used by the ObjectList data structure. The
  // constructor sets this equal to "boidX" where X is the boidNumber. 

  bool flockSelectively;
  // Should this boid flock only with boids of the same boidType, or with
  // all boids? The default value is FALSE, meaning that this boid will
  // flock with all boids regardless of type.
  // Basically, should boids of a feather stick together? :)
  
  int boidType;
  // Identifies the type of boid for selective flocking
  
private:

  BoidWorld *world;
  // All visible boids + obstacles

  BoidVector oldVelocity;
  // [m/sec] velocity at last update.

  BoidVector acceleration;
  // [m/(sec^2)] acceleration requested at last update.
					
  bool flightflag;
  // Has the boid been updated at least once?

};

// ------------------------------------------------ inline methods ------------------------------------------------

inline double
Boid::getGravAcceleration(void) const
{
  return 9.806650;
}

inline int
Boid::getBoidType(void) const
{
    return boidType;
}

inline int  
Boid::visibleToSelf(Boid *b)
{
  // find out if the boid b is within our field of view
  BoidVector BoidVectorToObject = b->position - position;
  
  // This isn't perfectly accurate, since we're not always facing in
  // the direction of our velocity, but it's close enough.
  return(AngleBetween(velocity, BoidVectorToObject) <= 1.0471967);  // pi/3 radians is our FOV
}

inline float
Boid::getProbeLength(void)
{

  float maxScale = 5;

  // When we're at maxVelocity, scalefactor = maxScale.
  // When our velocity is 0, scalefactor = 1.
  // Linearly scale in between.
  float scaleFactor = float(((maxScale-1)/maxVelocity) * Magnitude(velocity) + 1);

  return float(10.0f*bodyLength*scaleFactor);

}

inline double
Boid::desiredCruisingSpeed(void) {

  return 0.2*maxVelocity;
}

inline BoidVector 
Boid::levelFlight(BoidVector AccelSoFar) {

  // Determine the vertical acceleration.
  BoidVector verticalAcc(0, AccelSoFar.y, 0);

  // Try to negate it.
  return -verticalAcc;
}

#endif /* __BOID_H */
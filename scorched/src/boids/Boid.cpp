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

// Boid.c++
//
// CODE: Class for objects that move with A-Life boid-type behavior.
//
// (c) 1996 Christopher Kline <ckline@acm.org>
//


#ifndef __BOID_C
#define __BOID_C

#include "Boid.h"
#include <stdio.h>

// Returns the value in the visibility matrix which determines if this boid
// can see the "otherBoid". A value of -1 means that otherBoid cannot be
// seen; any other value indicates that otherBoid _can_ be seen, and the
// value is equal to the otherBoid's boidType.
#define visMatrix(otherBoid) \
     (world->getVisibilityMatrix()[boidNumber-1][(otherBoid)->boidNumber-1])

void 
Boid::calculateVisibilityMatrix(void) {

  
  int val;
  
  // Foreach boid, if it's not visible to us, put a a -1 in the
  // matrix. Otherwise put the boidType in the matrix.

  for (int i=0; i<world->getBoidCount(); i++) {
	  Boid *n = world->getBoids()[i];

    // Can't see self!
    if (n == this) {
      val = -1;
    }
    else {
      val = (visibleToSelf(n) ? n->boidType : -1);
    }

    visMatrix(n) = val;
  }
  
}

double
Boid::accumulate(BoidVector &accumulator, BoidVector valueToAdd) {

  double magnitudeRemaining = 1.0 - Magnitude(accumulator);
  double vMag = Magnitude(valueToAdd);
  double newMag = vMag < magnitudeRemaining ? vMag : magnitudeRemaining;

  accumulator += (Direction(valueToAdd) * newMag); 
  
  return newMag;
}

BoidVector 
Boid::navigator(void) {

  // Calculate the visibility matrix so that visibility computations are
  // much more efficient.
  calculateVisibilityMatrix();

  BoidVector vacc(0, 0, 0);  // BoidVector accumulator

  if (accumulate(vacc, collisionAvoidance()) >= 1.0)           goto MAXACCEL_ATTAINED;
  if (world->getBoidCount() > 1)
  {
	if (accumulate(vacc, flockCentering()) >= 1.0)               goto MAXACCEL_ATTAINED;
	if (accumulate(vacc, maintainingCruisingDistance()) >= 1.0)  goto MAXACCEL_ATTAINED;
	if (accumulate(vacc, velocityMatching()) >= 1.0)             goto MAXACCEL_ATTAINED;
  }
  if (accumulate(vacc, wander()) >= 1.0)                       goto MAXACCEL_ATTAINED;
  if (accumulate(vacc, levelFlight(vacc)) >= 1.0)              goto MAXACCEL_ATTAINED;
  
MAXACCEL_ATTAINED:  /* label */
  
  
  // IMPORTANT:
  // Since the FlockCentering, CollisionAvoidance, and VelocityMatching modules
  // return a BoidVector whose magnitude is a percentage of the maximum acceleration,
  // we need to convert this to a real magnitude before we hand it off to the Flight
  // module. 
  // Remember, maxAcceleration is in terms of a fraction (0.0 to 1.0) of maxVelocity

  vacc *= world->getMaxAcceleration() * world->getMaxVelocity(); 
  return(vacc);
}

void
Boid::calculateRollPitchYaw(BoidVector appliedAcceleration,
			    BoidVector currentVelocity,
			    BoidVector /*currentPosition*/) {
  // NOTES:
  // 
  // 1) currentPosition (the third argument) is unused, though in a more
  // advanced flight model it could make a difference (some flight dynamics
  // change depending on altitude, for example)
  //
  // 2) Dr. Craig Wanke of the Mitre Corporation in McLean, Virginia helped me
  // tremendously when working out these equations. Thanks, Craig!
  
  // In our simple flight model, roll only a function of lateral
  // (centripedal) acceleration and gravitational acceleration. We assume
  // that gravitational acceleration will NOT be zero. 

  // Determine the direction of the lateral acceleration.
  BoidVector lateralDir = Direction((currentVelocity % appliedAcceleration) % currentVelocity);
  
  // Set the lateral acceleration's magnitude. The magnitude is the BoidVector
  // projection of the appliedAcceleration BoidVector onto the direction of the
  // lateral acceleration).
  double lateralMag = appliedAcceleration * lateralDir;
  
  if (lateralMag == 0)
    roll = 0;  
  else
    roll = -atan2(getGravAcceleration(), lateralMag) + 3.14/2;

  pitch = -atan(currentVelocity.y / 
		sqrt(currentVelocity.z*currentVelocity.z
		     + currentVelocity.x*currentVelocity.x));

  

  yaw = atan2(currentVelocity.x, currentVelocity.z);

}

void Boid::updateslow()
{
	dampedroll = (dampedroll + roll) / 2.0;
}

BoidVector 
Boid::wander(void) {

  double distanceFromCruiseSpeed =
    (Magnitude(velocity) - desiredCruisingSpeed()) / world->getMaxVelocity();

  double urgency = fabs(distanceFromCruiseSpeed);
  if (urgency > 0.25)
    urgency = 0.25;
  
  return Direction(velocity) * urgency * (distanceFromCruiseSpeed > 0 ? -1 : 1);

}

BoidVector 
Boid::collisionAvoidance(void) {
    
    ISectData d;
    BoidVector normalToObject(0, 0, 0);
    int objectSeen = 0;
    BoidVector pointOnObject;

    // Ignore obstacles that are out of the range of our probe.
    double distanceToObject = getProbeLength();

    // Find closest imminent collision with non-boid object 
	for (unsigned int o=0; o<world->getObstacles().size(); o++) {

		Obstacle *obs = world->getObstacles()[o];
      
      d = obs->DoesRayIntersect(Direction(velocity), position);

      if (d.intersectionflag == 1) {      

	// Velocity BoidVector intersects an obstacle
	
	if (Magnitude(d.point-position) <= distanceToObject) {
	  // found a closer object
	  objectSeen = 1; 
	  distanceToObject = Magnitude(d.point-position);
	  normalToObject = d.normal;
	  pointOnObject = d.point;
	}
	
      }

    }
    
    if (!objectSeen) {
      return BoidVector(0,0,0);
    }

    return resolveCollision(pointOnObject, normalToObject); 
}
  
BoidVector 
Boid::resolveCollision(BoidVector pointOnObject, BoidVector normalToObject) {

  double distanceToObject = Magnitude(pointOnObject - position);
  
  // Make sure the object's normal is pointing towards the boid.
  // The boid wants to head in the direction of the normal, which
  // should push it away from the obstacle if the normal is pointing
  // towards the boid.
  if (AngleBetween(velocity, normalToObject) < 3.14/2) 
    normalToObject = - normalToObject;

  double mag = 1 - distanceToObject/getProbeLength();

  // Ignore objects that are farther away than our probe.
  if (mag < 0) mag = 0;

  return Direction(normalToObject) * mag;
}

BoidVector 
Boid::maintainingCruisingDistance(void) {

  double distanceToClosestNeighbor = DBL_MAX; // DBL_MAX defined in <limits.h>
  int foundClosestNeighbor = 0;
  Boid *n, *closestNeighbor;   
  double tempDistance;
  
  for (int i=0; i<world->getBoidCount(); i++) {
   
	  n = world->getBoids()[i];

    // Skip boids that we don't need to consider
    if (visMatrix(n) == -1) continue; 
    if (visMatrix(n) != boidType && flockSelectively) continue; 
    
    // Find distance from the current boid to self
    tempDistance = Magnitude(n->position - position);
    
    // remember distance to closest boid
    if (tempDistance < distanceToClosestNeighbor) {
      distanceToClosestNeighbor = tempDistance;
      foundClosestNeighbor = 1;
      closestNeighbor = n;
    }
    
  }
  
  BoidVector speedAdjustmentBoidVector(0, 0, 0);  

  if (foundClosestNeighbor) {

    // Have the boid try to remain at least cruiseDistance away from its
    // nearest neighbor at all times in all directions (i.e., don't violate
    // your neighbor's "personal space" bounding sphere of radius
    // cruiseDistance, but stay as close to the neighbor as possible). 

    BoidVector separationBoidVector = closestNeighbor->getPosition() - position;

     float separateFactor = 0.09f;
     float approachFactor = 0.05f;

	float cruiseDistance = world->getCruiseDistance();
    if (separationBoidVector.y < cruiseDistance) {
      speedAdjustmentBoidVector.y -= separateFactor;
    }
    else if (separationBoidVector.y > cruiseDistance) {
      speedAdjustmentBoidVector.y += approachFactor;
    }

    if (separationBoidVector.x < cruiseDistance) {
      speedAdjustmentBoidVector.x -= separateFactor;
    }
    else if (separationBoidVector.x > cruiseDistance) {
      speedAdjustmentBoidVector.x += approachFactor;
    }
    
    if (separationBoidVector.z < cruiseDistance) {
      speedAdjustmentBoidVector.z -= separateFactor;
    }
    else if (separationBoidVector.z > cruiseDistance) {
      speedAdjustmentBoidVector.z += approachFactor;
    }
    
  }
  // -- Otherwise, if we couldn't find a closest boid, speedAdjustmentBoidVector 
  //    will have a magnitude of 0 and thus have no effect on navigation.

  return(speedAdjustmentBoidVector);
}


BoidVector 
Boid::velocityMatching(void) {

  BoidVector velocityOfClosestNeighbor(0,0,0);
  double tempDistance;
  double distanceToClosestNeighbor = DBL_MAX;
  Boid *n;   
  
  for (int i=0; i<world->getBoidCount(); i++) {
   
	  n = world->getBoids()[i];
    
    // Skip boids that we don't need to consider
    if (visMatrix(n) == -1) continue; 
    if (visMatrix(n) != boidType && flockSelectively) continue; 
    
    // Find distance from the current boid to self
    tempDistance = Magnitude(n->position - position);
    
    // remember velocity BoidVector of closest boid
    if (tempDistance < distanceToClosestNeighbor) {
      distanceToClosestNeighbor = tempDistance;
      velocityOfClosestNeighbor = n->velocity;
    }
    
  }
  
  // If we found a close boid, set the percentage of our acceleration that
  // we want to use in order to begin flying parallel to its velocity BoidVector.
  // -- Otherwise, if we couldn't find a closest boid, velocityOfClosestNeighbor 
  //    will have a magnitude of 0 and thus have no effect on navigation.
  if (distanceToClosestNeighbor != DBL_MAX) {
    // return velocity BoidVector of closest boid so we can try to match it
    velocityOfClosestNeighbor.SetMagnitude(0.05);
  }
  
  return(velocityOfClosestNeighbor);
}

BoidVector 
Boid::flockCentering(void) {

  BoidVector t;
  double boids_observed = 0;   // number of boids that were checked 
  BoidVector flockcenter(0,0,0);   // approximate center of flock
  Boid *n;   
  
  // Calculate approximate center of flock by averaging the positions of all
  // visible boids that we are flocking with.
  for (int i=0; i<world->getBoidCount(); i++) {
   
	  n = world->getBoids()[i];

    if (visMatrix(n) == -1) continue; 
    if (visMatrix(n) != boidType && flockSelectively) continue; 
   
    flockcenter += n->position;
    boids_observed++;
  }
  
  if (boids_observed != 0) { 
    flockcenter /= boids_observed;
    
    // now calculate a BoidVector to head towards center of flock
    t = flockcenter - position;
    
    // and the percentage of maximum acceleration (in decimal) to use when yaw toward center
    t.SetMagnitude(0.1); 
    
  }
  else {
    // Don't see any other birds. 
    t.SetMagnitude(0);
  }
  
  return(t);
}

bool
Boid::update(const double &elapsedSeconds) {

  frame = (float) (elapsedSeconds * 20.0) + (float) frameOffset;

  bool ok =  true;
  
  if (flightflag == false) {
    flightflag = true;
  }
  else {
    double dt = elapsedSeconds - lastUpdate;
    
    // Step 1: Calculate new position and velocity by integrating
    //         the acceleration BoidVector.
    
    // Update position based on where we wanted to go, and how long it has 
    // been since we made the decision to go there
    position += (oldVelocity*dt + 0.5*acceleration*dt*dt);
    
    // Set new velocity, which will be in the direction boid is traveling.
    velocity += (acceleration*dt);
    
    // Cap off velocity at maximum allowed value
    if (Magnitude(velocity) > world->getMaxVelocity()) {
      velocity.SetMagnitude(world->getMaxVelocity());
    }
    
    // Step 2: Calculate new roll, pitch, and yaw of the boid
    //         which correspond to the changes in position and velocity.
    //         Remember: the boid isn't necessarily oriented in the
    //         direction of the velocity!
    // (assuming +y is up, +z is through the nose, +x is through the left wing)
    calculateRollPitchYaw(acceleration, oldVelocity, position);
  }    
  
  // remember current velocity 
  oldVelocity = velocity; 

  // remember desired acceleration (the acceleration BoidVector that the
  // Navigator() module specified
  acceleration = navigator(); 
  
  ok &= SimObject::update(elapsedSeconds); // Do generic object  updating

  return ok;
}


Boid::Boid(BoidWorld *w, int boidNum, 
	BoidVector bPosition, BoidVector bVelocity) {
  
  flightflag = false;	      // haven't flown yet
  world = w;

  position = bPosition;
  velocity = bVelocity;
  mass = 9.07; // 9.07 kg is approx 20 lbs.

  roll = pitch = yaw = dampedroll = 0;
  boidType = NORMAL;
  flockSelectively = false; 

  boidNumber = boidNum;

  frameOffset = (float) rand();
}		
			
#endif /* __BOID_C */

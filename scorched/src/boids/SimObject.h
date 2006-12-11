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

// SimObject.h
//
// INTERFACE: Base class for all object components of the VRRR project
//
// (c) 1996 Christopher Kline <ckline@acm.org>

// Forward declare SimObject so that the classes that are #included below will know about it.
class SimObject;

#ifndef __SIMOBJECT_H
#define __SIMOBJECT_H

#include "BoidVector.h"

class SimObject {

public:

  // -- Query Methods -- 
  float getFrameOffset(void) const;					// Returns the graphics frame of the object

  virtual double getMass(void) const;		      // Returns the mass of the object in kilograms. 
  
  virtual const BoidVector &getVelocity(void) const;      // Returns the current velocity of this object's center of mass
						      // (in world coordinates) in m/s.
  
  virtual const BoidVector &getPosition(void) const;      // Returns the current position of this object's center of mass
						      // (in world coordinates) in m.

  
  // -- Modification Methods

  virtual bool update(const double &elapsedSeconds);  // The simulation will call this method, which updates the object
						      // based on its current state, the current elapsed time in
						      // seconds, and the external and internal forces acting upon it.
						      // A return value of FALSE indicates that some sort of error has
						      // occured during the update.
  
  SimObject(void);				      // Default constructor
  
  virtual ~SimObject(void);			      // Destructor

protected:
  
  BoidVector velocity;				      // Current velocity of this object's center of mass (in 3-space)
						      // in m/s. 

  BoidVector position;				      // Current position of this object's center of mass (in 3-space)
						      // in m/s. 

  double mass;				              // Mass of this object in kilograms.

  double lastUpdate;				      // Last time this object was updated via update().

  float frameOffset;
  
private:
  
};

// ------------------------------------------------ inline methods ------------------------------------------------

inline const BoidVector &
SimObject::getVelocity(void) const {
  
  return velocity;

}

inline const BoidVector &
SimObject::getPosition(void) const {
  
  return position;

}

inline double
SimObject::getMass(void) const {

  return mass;
  
}

inline float 
SimObject::getFrameOffset(void) const {

  return frameOffset;
  
}

inline bool
SimObject::update(const double &elapsedSeconds) {

  lastUpdate = elapsedSeconds;		

  return true;
  
}

inline
SimObject::SimObject(void) {
  
  position = velocity = BoidVector(0, 0, 0);
  lastUpdate = 0;
  
}

inline   
SimObject::~SimObject(void) {
  
}

#endif /* __SIMOBJECT_H */



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

#ifndef _OBSTACLE_H
#define _OBSTACLE_H

#include "BoidVector.h"


/* information about a possible intersection of 
   a ray with a BoidPolygonal obstacle */

struct ISectData {
  int intersectionflag; // 1 = intersection did occur, 0 = didn't
  BoidVector normal;         // normal at intersection
  BoidVector point;          // point of intersection
};

class Obstacle;
class BoidPolygon;
class Box;
class Sphere;

//---------------- CLASS OBSTACLE ------------------------------

// A generic object class 

class Obstacle {

public:

  Obstacle(void) { _id = 0; }
  
  virtual ~Obstacle(void) { }

  virtual Obstacle *Clone(void) const = 0;
  // Interface to copy constructors for all derived classes. Returns a deep
  // copy of this obstacle.
  
  ISectData DoesRayIntersect(BoidVector raydirection, BoidVector rayorigin);
  // Does a ray intersect this obstacle if it projects from rayorigin in the
  // direction specified by raydirection? raydirection MUST NOT be (0,0,0) !
  
  virtual int getId(void) const;
  
  virtual void setId(int id);
  
protected:

  virtual ISectData IntersectionWithRay(const BoidVector & raydirection,
					const BoidVector &rayorigin) const = 0;

  int _id; // do anything you want with this (perhaps have "types" of objects)
};

//---------------- CLASS BoidPolygon ------------------------------

// A convex BoidPolygon object

class BoidPolygon : public Obstacle {

public:
  
  BoidPolygon(int numverts,
	  const BoidVector v0, const BoidVector v1, const BoidVector v2, const
	  BoidVector v3);
  // Constructor. Vertices must be specified in clockwise or
  // counter-clockwise order.
  
  BoidPolygon(const BoidPolygon &p); 
  // Copy constructor.
    
  ~BoidPolygon(void);
  
  virtual Obstacle *Clone(void) const;  

  friend int operator==(const BoidPolygon &a, const BoidPolygon &b);  
  
protected:
  virtual ISectData IntersectionWithRay(const BoidVector &raydirection,
					const BoidVector &rayorigin) const;

private:
  BoidPolygon(void) { };
  // Default constructor is private to forced users to specify vertices when
  // constructing.

  float d;         // plane of this BoidPolygon is: (normal dot v[0]) + d = 0 
  short i1, i2;    // indices of axis of closest major plane to BoidPolygon 
  BoidVector normal;   // normal to this BoidPolygon
  int numvertices; // size of array of vertices 
  BoidVector *vertex;  // array of vertices

};

//---------------- CLASS BOX ------------------------------

// A box-shaped object

class Box : public Obstacle {

public: 
  Box(const BoidVector &topLeftBackCorner, const BoidVector &bottomRightFrontCorner);
  // Constructor.
  // When constructing the box, imagine that it is originally positioned
  // with the bottomRightFrontCorner at (0, 0, 0) and the topLeftBackCorner
  // at (width, height, length). The top and bottom sides are parallel to
  // the x-z plane; the left and right are parallel to the y-z plane, and
  // the back and front are parallel to the x-y plane.
  // Then it is translated by the amount (x, y, z) where x, y, and z are the
  // respective components of the bottomRightFrontCorner.
  
  ~Box(void);

  virtual Obstacle *Clone(void) const;  

  Box(const Box &b);
  // copy constructor
  
  friend int operator==(const Box &a, const Box &b);  

protected:
  virtual ISectData IntersectionWithRay(const BoidVector &raydirection,
					const BoidVector &rayorigin) const;

private:
  Box(void) { };
  // Default constructor is private to force users to specify vertices when
  // constructing.

  BoidVector tlb, brf;  // topLeftBack and bottomRightFront corners of box.

  BoidPolygon *side[6]; // 6 BoidPolygons comprising the box's sides

  Sphere *boundingSphere; // Sphere bounding the area of this box.
};

//---------------- CLASS SPHERE ------------------------------

// A spherical object

class Sphere : public Obstacle {

public:
  Sphere(const BoidVector &sphereOrigin, const double sphereRadius);    

  virtual Obstacle *Clone(void) const;
  
  friend int operator==(const Sphere &a, const Sphere &b);  
    
protected:
  virtual ISectData IntersectionWithRay(const BoidVector &raydirection,
					const BoidVector &rayorigin) const;
    
private:
  Sphere(void) { };
  // Default constructor is private to force users to specify radius when
  // constructing.

  double radius;  // radius of sphere

  BoidVector origin;  // center of sphere

};

// ----------------------------- -------------- --------------------------
// ----------------------------- inline methods --------------------------
// ----------------------------- -------------- --------------------------

// --- inline methods for Obstacle

inline ISectData
Obstacle::DoesRayIntersect(BoidVector raydirection, BoidVector rayorigin)
{
  return IntersectionWithRay(raydirection, rayorigin);
}

inline int
Obstacle::getId(void) const
{
  return _id;
}
  
inline void
Obstacle::setId(int id)
{
  _id = id;
}

// --- inline methods for BoidPolygon

inline
BoidPolygon::~BoidPolygon(void)
{
  delete[] vertex;
}

inline Obstacle *
BoidPolygon::Clone(void) const
{
  return new BoidPolygon(*this);
}

// --- inline methods for Box

inline
Box::~Box(void)
{
  for (int i = 0; i < 6; i++)
    delete side[i];

  delete boundingSphere;
}

inline Obstacle *
Box::Clone(void) const
{
  return new Box(*this);
}

inline int 
operator==(const Box &a, const Box &b) { 
  return (a.tlb == b.tlb && a.brf == b.brf);
}

// --- inline methods for Sphere

inline
Sphere::Sphere(const BoidVector &sphereOrigin, const double sphereRadius)
{
  radius = sphereRadius;
  origin = sphereOrigin;
}

inline int 
operator==(const Sphere &a, const Sphere &b) { 
  return (a.radius == b.radius && a.origin == b.origin);
}

inline Obstacle *
Sphere::Clone(void) const
{
  return new Sphere(*this);
}   

#endif	/* #ifndef _OBSTACLE_H */

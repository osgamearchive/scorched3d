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

#include <movement/BoidsObstacle.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <stdarg.h>

//---------------- CLASS OBSTACLE ------------------------------


//---------------- CLASS BoidPolygon ------------------------------

int 
operator==(const BoidPolygon &a, const BoidPolygon &b) { 
    int i, test;
  
    if (a.numvertices != b.numvertices) return 0; // definitely different

    // Check if all vertices are the same vertices.
    for (i=0, test=1; test && (i < a.numvertices); i++)       
	test = test && (a.vertex[i]==b.vertex[i]);

    return(test);
}

BoidPolygon::BoidPolygon(const BoidPolygon &p) {
  
    d = p.d;
    i1 = p.i1;
    i2 = p.i2;
    normal = p.normal;

    // copy all the vertices over
    numvertices = p.numvertices;
    vertex = new BoidVector[numvertices];
    for (int i = 0; i < numvertices; i++) 
	vertex[i] = p.vertex[i];
  
}

BoidPolygon::BoidPolygon(int numverts, const BoidVector v0, 
		 const BoidVector v1, const BoidVector v2, const BoidVector v3) {

  BoidVector p;
  int i;

	assert(numverts == 4);

  // Set the number of vertices in the BoidPolygon and allocate the array
  // of vertices
  numvertices = numverts;
  vertex = new BoidVector[numvertices];

  // set up the first 3 required vertices
  vertex[0] = v0; vertex[1] = v1; vertex[2] = v2;
  
  // cross product provides normal to poly
  normal = (vertex[1] - vertex[0]) % (vertex[2] - vertex[0]); 
  normal.Normalize();

  // plane of BoidPolygon is: (normal dot vertex[0]) + d = 0 
  d = float(-vertex[0] * normal);  
  
  // Find find dominant axis of normal BoidVector, and put the indices 
  // of the plane perpendicular to that axis into i1 and i2.
  // This tells us what the closest major plane to the BoidPolygon is.
  // (i.e., i1==0 and i2==2 means closest plane is xz-plane)  
  if (fabs(normal.x) > fabs(normal.y) && fabs(normal.x) > fabs(normal.z)) {
    i1 = 1; i2 = 2;
  }
  else if (fabs(normal.y) >= fabs(normal.x) && fabs(normal.y) >= fabs(normal.z)) {
    i1 = 0; i2 = 2;    
  }
  else {
    i1 = 0; i2 = 1;   
  }

  // set up the rest of them
  for (i=3; i<numverts; i++) {
    vertex[i] = v3;
  }

}

/*
 *  IntersectionWithRay():
 *  Determines if a given ray intersects the BoidPolygon. 
 *  
 *  Parameters:
 *   raydirection: direction of ray
 *   rayorigin:    origin of ray
 *   
 *  Returns:
 *   An ISectData structure, which describes whether or
 *   not an intersection actually occured, and if so, information
 *   about that intersection
*/

ISectData 
BoidPolygon::IntersectionWithRay(const BoidVector &raydirection,
			     const BoidVector &rayorigin) const {

    /* 
       Adapted by Chris Kline 10/12/95 from :
     
       An Efficient Ray/BoidPolygon Intersection
       by Didier Badouel
       from "Graphics Gems", Academic Press, 1990
       */

    ISectData data;
    data.intersectionflag = 0;  // no intersection as default

    BoidVector rdirection = Direction(raydirection);     // only want a unit BoidVector
    
    // If ray has no direction, then try to handle this 
    // gracefully by returning with no intersection.
    if (rdirection.x == 0 &&
	rdirection.y == 0 &&
	rdirection.z == 0)
      return data;
  
    
    // Compute t, where the parametric representation of the ray r(t) is:
    //   r(t) = rayorigin + rdirection * t  
    // Using r(t) and the representation of the BoidPolygon's plane:
    //   (normal dot vertex[0]) + d = 0
    // we can calculate the value of t

  
    double denom = normal * rdirection;

    // If BoidPolygon is parallel to the ray, or contains the ray,
    // there is no _unique_ intersection.
    if (denom == 0) return(data); // return data indicating no intersection
  
    double t = - (d + normal*rayorigin) / denom;

    // If intersection is behind the ray's origin, there is no intersection
    if (t < 0) return (data);
    
    // Calculate the point of intersection 
    data.point = rayorigin + rdirection*t;
  
    // Determine if the point of intersection is inside the BoidPolygon 
    double u0 = data.point[i1] - vertex[0][i1]; 
    double v0 = data.point[i2] - vertex[0][i2];
    double u1, u2, v1, v2, alpha, beta;
    char inside = 0;  // is the intersection point inside the BoidPolygon?
    int  i = 2;       // loop counter

    // break the BoidPolygon up into triangles and check each one to see if the
    // point of intersection is within it.
    do {
	// The BoidPolygon is viewed as (n-2) triangles. 
	u1 = vertex[i-1][i1] - vertex[0][i1]; 
	v1 = vertex[i-1][i2] - vertex[0][i2];
	u2 = vertex[i  ][i1] - vertex[0][i1]; 
	v2 = vertex[i  ][i2] - vertex[0][i2];
    
	if (u1 == 0) {
	    beta = u0/u2;
	    if ((beta >= 0.0) && (beta <= 1.0)) {
		alpha = (v0 - beta*v2)/v1;
		inside = ((alpha >= 0.0) && (alpha+beta <= 1.0));
	    }
	} 
	else {
	    beta = (v0*u1 - u0*v1)/(v2*u1 - u2*v1);
	    if ((beta >= 0.0)&&(beta <= 1.0)) {
		alpha = (u0 - beta*u2)/u1;
		inside = ((alpha >= 0.0)&&((alpha+beta) <= 1.0));
	    }
	}
	i++;
    } while ((inside == 0) && (i < numvertices));
  
    // Ok, so there was an intersection with the BoidPolygon's plane, 
    // but we only care if the intersection was inside the BoidPolygon 
    data.intersectionflag = inside; 

    // make sure to return the normal of the poly that there the ray
    // might be intersecting. 
    data.normal = normal;

    return(data);  
}

//---------------- CLASS BOX------------------------------

Box::Box(const Box &b) {
  
  for (int i= 0; i < 6; i++) {
    side[i] = new BoidPolygon(*(b.side[i]));
  }

  tlb = b.tlb;
  brf = b.brf;

  boundingSphere = new Sphere(*(b.boundingSphere));
}

Box::Box(const BoidVector &topLeftBackCorner, const BoidVector &bottomRightFrontCorner) {

  // When constructing the box, imagine that it is originally positioned
  // with the bottomRightFrontCorner at (0, 0, 0) and the topLeftBackCorner
  // at (width, height, length). The top and bottom sides are parallel to
  // the x-z plane; the left and right are parallel to the y-z plane, and
  // the back and front are parallel to the x-y plane.
  // Then it is translated by the amount (x, y, z) where x, y, and z are the
  // respective components of the bottomRightFrontCorner.

  tlb = topLeftBackCorner;
  brf = bottomRightFrontCorner;

  double width  = tlb.x - brf.x;
  double height = tlb.y - brf.y;
  double length = tlb.z - brf.z;

#ifdef DEBUG_OBSTACLES
  cerr << "Constructor for Box" << endl;
  cerr << "length = " << length << endl;
  cerr << "width = " << width << endl;
  cerr << "height = " << height << endl;
  cerr << "tlb = " << tlb << endl;
  cerr << "brf = " << brf << endl;
#endif
  
  // Top
  side[0] = new BoidPolygon(4,
			brf + BoidVector(0,     height, 0),
			brf + BoidVector(0,     height, length),
			brf + BoidVector(width, height, length),
			brf + BoidVector(width, height, 0));
  
  // Bottom
  side[1] = new BoidPolygon(4,
			brf,
			brf + BoidVector(0,     0, length),
			brf + BoidVector(width, 0, length),
			brf + BoidVector(width, 0, 0));
  
  // Left
  side[2] = new BoidPolygon(4,
			brf + BoidVector(width, 0,      0),
			brf + BoidVector(width, height, 0),
			tlb,
			brf + BoidVector(width, 0,      length));
  
  // Right
  side[3] = new BoidPolygon(4,
			brf + BoidVector(0,      0,      0),
			brf + BoidVector(0,      height, 0),
			tlb + BoidVector(-width, 0,      0),
			brf + BoidVector(0,      0,      length));

  // Front
  side[4] = new BoidPolygon(4,
			brf,
			brf + BoidVector(0,     height, 0),
			brf + BoidVector(width, height, 0),
			brf + BoidVector(width, 0,      0));
  

  
  // Back
  side[5] = new BoidPolygon(4,
			brf + BoidVector(0,     0,      length),
			brf + BoidVector(0,     height, length),
			brf + BoidVector(width, height, length),
			brf + BoidVector(width, 0,      length));
  
#ifdef DEBUG_OBSTACLES
  for (int i = 0; i < 6; i++)
    cerr << "side[" << i<< "] = " << *side[i] << endl;
#endif

  BoidVector center(brf.x + width/2,
		brf.y + height/2,
		brf.z + length/2);

  
  double radius = Magnitude(tlb - center);
  double temp = Magnitude(brf - center);
  if (temp > radius)
    radius = temp;
  
  boundingSphere = new Sphere(center, radius);

#ifdef DEBUG_OBSTACLES
  cerr << "Bounding sphere = " << *boundingSphere << endl;
#endif
  
}

ISectData 
Box::IntersectionWithRay(const BoidVector &raydirection,
			 const BoidVector &rayorigin) const {
        
  // Do a quick check to see if the ray intersected the bounding sphere of
  // this box. If not, then no need to check the sides of the box
  // individually. 
  ISectData data = boundingSphere->DoesRayIntersect(raydirection, rayorigin);
  if (data.intersectionflag == 0) {
#ifdef DEBUG_OBSTACLES
    cerr << "Missed Bounding Sphere\n";
#endif
    return data;
  }

  ISectData closestIntersect;
  double distToClosestIntersect =  DBL_MAX;

  closestIntersect.intersectionflag = 0;

  // Attempt to find the closest intersection with a side of the box, if
  // there actually is an intersection.
  for (int i = 0; i < 6; i++) {
#ifdef DEBUG_OBSTACLES
    cerr << "Checking Poly: " << *side[i] << endl;
#endif
    data = side[i]->DoesRayIntersect(raydirection, rayorigin);
    if (data.intersectionflag == 1) {
#ifdef DEBUG_OBSTACLES
      cerr << "Found Isect in box\n";
#endif
      // Find closest intersection, since the ray could intersect multiple
      // sides. 
      double temp = Magnitude(data.point - rayorigin);
      if (temp < distToClosestIntersect) {
	distToClosestIntersect = temp;
	closestIntersect = data;
#ifdef DEBUG_OBSTACLES
	cerr << "Closest intersect at " << closestIntersect.point
	     << " Normal " << closestIntersect.normal << "\n";
#endif
      }
    }
  }

  return closestIntersect;
}

//---------------- CLASS SPHERE------------------------------

ISectData 
Sphere::IntersectionWithRay(const BoidVector &raydirection,
			    const BoidVector &rayorigin) const {
    
    ISectData data;

    BoidVector rdirection = Direction(raydirection);     // only want a unit BoidVector   

    // If the ray starts at the sphere's origin, then calculations
    // are trivial
    if (rayorigin == origin) {
      data.intersectionflag = 1; // The ray MUST intersect the sphere!
      data.point = radius*rdirection; 
      data.normal = Direction(origin - data.point);
      return data; // bail out early and save time
    }

    data.intersectionflag = 0;  // no intersection as default

    // If ray has no direction, then try to handle this 
    // gracefully by returning with no intersection.
    // This situation shouldn't happen, though
    if (rdirection == BoidVector(0,0,0)) return data;
    
    //  The stuff below is Adapted by Chris Kline 11/28/95 from :
    //
    //  Intersection Of A Ray With A Sphere
    //  by James Hultquist
    //  from "Graphics Gems", Academic Press, 1990
    // 
    // Modified to handle special case when the rayorigin is
    // inside the sphere, which Hultquist didn't consider (because
    // he was only interested in raytracing the outside of spheres).
    
    // Find component of the ray from rayorigin to the
    // sphere's origin in the direction specified by 
    // rdirection
    double v = (origin - rayorigin) * rdirection;

    // Find the square of the distance from the ray-sphere 
    // intersection to the point on the ray closest to the 
    // sphere's origin
    double d = 
	pow(radius, 2) - 
	pow(Magnitude(origin - rayorigin), 2) +
	pow(v, 2);

    // If d < 0 then there is no intersection
    if (d < 0) return data;

    // Otherwise find the intersection point
    if (Magnitude(rayorigin - origin) < radius) 
	data.point = (v + sqrt(d)) * rdirection + rayorigin; /* inside sphere */
    else 	
	data.point = (v - sqrt(d)) * rdirection + rayorigin; /* outside sphere */
	
    // An intersection with the ray occured
    data.intersectionflag = 1; 

    // set the normal at the point of ray-sphere intersection
    // (the outward-pointing normal) 	
    data.normal = Direction(data.point - origin);	

    return data;

}


#include <common/Line.h>
#include <engine/ScorchedContext.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapemap/HeightMap.h>

BoidsObstacle::BoidsObstacle(ScorchedContext &context, Vector &maxBounds, Vector &minBounds) :
	context_(context), minBounds_(minBounds), maxBounds_(maxBounds)
{
}

BoidsObstacle::~BoidsObstacle()
{
}

ISectData BoidsObstacle::IntersectionWithRay(const BoidVector & raydirection,
		const BoidVector &rayorigin) const
{
	ISectData result;
	result.intersectionflag = 0;

	Vector direction(
		(float) raydirection.x, 
		(float) raydirection.z, 
		(float) raydirection.y);
	direction.StoreNormalize();
	direction *= 2.0f;
	Vector position(
		(float) rayorigin.x, 
		(float) rayorigin.z, 
		(float) rayorigin.y);
	result.normal.x = 0.0;
	result.normal.y = 0.0;
	result.normal.z = 0.0;

	for (int i = 0; i < 12; i++)
	{
		position += direction;
		if (position[0] < minBounds_[0])
		{
			result.intersectionflag = 1;
			result.point.x = position[0];
			result.point.y = position[2];
			result.point.z = position[1];
			result.normal.x = 1.0f;
		}
		else if (position[0] > maxBounds_[0])
		{
			result.intersectionflag = 1;
			result.point.x = position[0];
			result.point.y = position[2];
			result.point.z = position[1];
			result.normal.x = -1.0f;
		}
		if (position[1] < minBounds_[1])
		{
			result.intersectionflag = 1;
			result.point.x = position[0];
			result.point.y = position[2];
			result.point.z = position[1];
			result.normal.z = 1.0f;
		}
		else if (position[1] > maxBounds_[1])
		{
			result.intersectionflag = 1;
			result.point.x = position[0];
			result.point.y = position[2];
			result.point.z = position[1];
			result.normal.z = -1.0f;
		}
		if (position[2] < double(minBounds_[2]))
		{
			result.intersectionflag = 1;
			result.point.x = position[0];
			result.point.y = position[2];
			result.point.z = position[1];
			result.normal.y = 1.0f;
		}
		else if (position[2] > double(maxBounds_[2]))
		{
			result.intersectionflag = 1;
			result.point.x = position[0];
			result.point.y = position[2];
			result.point.z = position[1];
			result.normal.y = -1.0f;
		}
		if (result.intersectionflag == 1)
		{
			if (result.normal.x + result.normal.y + result.point.z > 1.0f)
			{
				result.normal.Normalize();
			}
			break;
		}

		if (context_.landscapeMaps->getGroundMaps().
			getHeight((int) position[0], (int) position[1]) >
			position[2] - 3.0f)
		{
			position -= direction;
			Vector &normal = context_.landscapeMaps->getGroundMaps().
				getNormal((int) position[0], (int) position[1]);

			result.intersectionflag = 1;
			result.point.x = position[0];
			result.point.y = position[2];
			result.point.z = position[1];
			result.normal.x = normal[0];
			result.normal.y = normal[2];
			result.normal.z = normal[1];
			break;
		}
	}

	return result;
}

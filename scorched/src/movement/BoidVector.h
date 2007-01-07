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

// BoidVector.h
//
// INTERFACE: Class for 3-BoidVectors with double precision
//
// (c) 1996 Christopher Kline <ckline@acm.org>


#ifndef __BoidVector_H
#define __BoidVector_H

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <common/DefinesAssert.h>

// ---------------- BoidVectorS CLASS ------------------
 
class BoidVector {

public:
  double x, y, z;                         // direction with magnitude
  
  BoidVector(void);                           // Default constructor
  BoidVector( double a, double b, double c);  // Constructor
  
  void Set(double a, double b, double c);  // Set each component of the BoidVector
  void SetDirection(const BoidVector &d);      // Set the direction of the BoidVector without modifying the length
  void CopyDirection(const BoidVector &d);     // Set the direction of this BoidVector to be the same as the direction of the argument
  void SetMagnitude(const double m);       // Set the magnitude of the BoidVector without modifying the direction
  void CopyMagnitude(const BoidVector &v);     // Set the magnitude of this BoidVector to be the same as the magnitude of the argument
  void Normalize(void);                    // Normalize the BoidVector to have a length of 1
  double Length(void);			   // Return the magnitude (length) of this BoidVector 
  
  BoidVector &operator=(const BoidVector &b);                                // ex: a = b
  double &operator[](const int index);                               // ex: a[1] (same as a.y)
  friend int operator!=(const BoidVector &a, const BoidVector &b);     // ex: a != b
  friend int operator==(const BoidVector &a, const BoidVector &b);     // ex: a == b
  friend BoidVector operator+(const BoidVector &a, const BoidVector &b);   // ex: a + b
  friend BoidVector operator-(const BoidVector &a, const BoidVector &b);   // ex: a - b
  friend BoidVector operator-(const BoidVector &a);                    // ex: -a
  friend BoidVector &operator+=(BoidVector &a, const BoidVector &b);             // ex: a += b
  friend BoidVector &operator-=(BoidVector &a, const BoidVector &b);             // ex: a -= b
  friend BoidVector operator%(const BoidVector &a, const BoidVector &b);   // ex: a % b (cross product)
  friend double operator*(const BoidVector &a, const BoidVector &b);   // ex: a * b (dot product)
  friend BoidVector operator*(const double &a, const BoidVector &b);   // ex: a * b (scalar multiplication)
  friend BoidVector operator*(const BoidVector &a, const double &b);   // ex: a * b (scalar multiplication)
  friend BoidVector &operator*=(BoidVector &a, const double &b);             // ex: a *= b (scalar multiplication + assignment)
  friend BoidVector operator/(const BoidVector &a, const double &b);   // ex: a / b (scalar divide)
  friend BoidVector &operator/=(BoidVector &a, const double &b);             // ex: a /= b (scalar divide + assignment)

  friend double Magnitude(const BoidVector &a);                    // Returns the length of the argument
  friend double AngleBetween(const BoidVector &a, const BoidVector &b);// Returns the angle (in radians!) between the two arguments
  
private:

};



// ------------ CALCULATIONS USING BoidVectorS --------------

BoidVector Direction(const BoidVector &a);
BoidVector Direction(const double &x, const double &y, const double &z);

//-----------------------------------------------------
// INLINE FUNCTIONS
//-----------------------------------------------------

#define VLENSQRD(a, b, c) ((a)*(a) + (b)*(b) + (c)*(c))
#define VLEN(a, b, c) sqrt(VLENSQRD(a, b, c))

inline void 
BoidVector::Set(double a, double b, double c) {

  x = a;
  y = b;
  z = c;

}

inline void 
BoidVector::Normalize(void) {

  double mag = VLEN(x, y, z);

  if (mag == 0)
    return;

  x /= mag;
  y /= mag;
  z/= mag;
  
}

inline BoidVector::BoidVector(void) {

  Set(0, 0, 0);
}         

inline BoidVector::BoidVector( double a, double b, double c) { 

  Set(a, b, c);  
} 

inline BoidVector &
BoidVector::operator=(const BoidVector &b) { // example: a = b

  x = b.x; 
  y = b.y; 
  z = b.z; 

  return(*this);
}

inline void 
BoidVector::SetMagnitude(const double m) {

  this->Normalize();
  *this *= m;

}

inline void 
BoidVector::CopyMagnitude(const BoidVector &v) {

 SetMagnitude(Magnitude(v));
}

inline void 
BoidVector::SetDirection(const BoidVector &d) {

  double m = Magnitude(*this);
  BoidVector v = d;
  
  v.Normalize();
  *this = v * m;
  
}

inline void 
BoidVector::CopyDirection(const BoidVector &d) {

  SetDirection(Direction(d));
}

inline double
BoidVector::Length(void) {

  return Magnitude(*this);
}

inline double &
BoidVector::operator[](const int index) {  // example: a[1] (same as a.y)

  if (index == 0) 
    return(x);
  else if (index == 1) 
    return(y);
  else if (index == 2) 
    return(z);
  else {
    exit(888);
    return(z); // this will never get called, but prevents compiler warnings...
  }
  
}

inline int 
operator!=(const BoidVector &a, const BoidVector &b) { // example: a 1= b

  return(a.x != b.x || a.y != b.y || a.z != b.z);
}

inline int 
operator==(const BoidVector &a, const BoidVector &b) { // example: a == b

  return(a.x == b.x && a.y == b.y && a.z == b.z);
}

inline BoidVector 
operator+(const BoidVector &a, const BoidVector &b) { // example: a + b

  BoidVector c(a.x+b.x, a.y+b.y, a.z+b.z);
  return(c);
}

inline BoidVector 
operator-(const BoidVector &a, const BoidVector &b) { // example: a - b

  BoidVector c(a.x-b.x, a.y-b.y, a.z-b.z);
  return(c);
}

inline BoidVector 
operator-(const BoidVector &a) { // example: -a

  BoidVector c(-a.x, -a.y, -a.z);

  return(c);
}

inline BoidVector &
operator+=(BoidVector &a, const BoidVector &b) { // example: a += b

  a = a + b;
  return(a);
}

inline BoidVector &
operator-=(BoidVector &a, const BoidVector &b) { // example: a -= b

  a = a - b;
  return(a);
}

inline BoidVector 
operator%(const BoidVector &a, const BoidVector &b) { // example: a % b (cross product)

  BoidVector c(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
  return(c);
}

inline double 
operator*(const BoidVector &a, const BoidVector &b) { // example: a * b (dot product)

  return(a.x*b.x + a.y*b.y + a.z*b.z);
}

inline BoidVector 
operator*(const double &a, const BoidVector &b) { // example: a * b (scalar multiplication)

  BoidVector c = b;
  c.x *= a;
  c.y *= a;
  c.z *= a;
  
  return(c);
}

inline BoidVector 
operator*(const BoidVector &a, const double &b) { // example: a * b (scalar multiplication)

  return(b * a);
}

inline BoidVector &
operator*=(BoidVector &a, const double &b) { // example: a *= b (scalar multiplication + assignment)

  a = a * b;
  return(a);
}

inline BoidVector 
operator/(const BoidVector &a, const double &b) { // example: a / b (scalar divide)

  if (b == 0) dialogExit("Boid", "WARNING: You're dividing a BoidVector by a zero-length scalar! NOT GOOD!\n");

  BoidVector c = a*(1/b);

  return(c);
}

inline BoidVector &
operator/=(BoidVector &a, const double &b) { // example: a / b (scalar divide + assignment)

  a = a/b;
  return(a);
}

inline BoidVector 
Direction(const BoidVector &a) {

  BoidVector u = a;

  u.Normalize();
  return(u);
}

inline BoidVector
Direction(const double &x, const double &y, const double &z) {

  return Direction(BoidVector(x, y, z));
}


inline double 
Magnitude(const BoidVector &a) {

  return VLEN(a.x, a.y, a.z);
}

inline double 
AngleBetween(const BoidVector &a, const BoidVector &b) {

  // returns angle between a and b in RADIANS
  return acos((a*b)/(Magnitude(a)*Magnitude(b)));
} 


#undef VLEN
 
#endif /* #ifndef __BoidVector_H */

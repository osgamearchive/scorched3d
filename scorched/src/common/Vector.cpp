////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
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

#include <math.h>
#include <common/Defines.h>
#include <common/Vector.h>

__inline float fast_sqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y  = number;
	i  = * (long *) &y;						// evil floating point bit level hacking
	i  = 0x5f3759df - (i >> 1);             // what the fuck?
	y  = * (float *) &i;
	y  = y * (threehalfs - (x2 * y * y));   // 1st iteration

	return y;
}

Vector Vector::nullVector;

Vector::Vector()
{
	V[0] = V[1] = V[2] = 0.0f;
}

Vector::Vector(const float ang, const double length )
{
	V[0] = (float) sin(ang / 180.0f * 3.14f) * float(length);
	V[1] = (float) cos(ang / 180.0f * 3.14f) * float(length);
	V[2] = 0.0f;
}

Vector::Vector(const Vector &v)
{
	V[0] = ((Vector &) v)[0];
	V[1] = ((Vector &) v)[1];
	V[2] = ((Vector &) v)[2];
}

Vector::Vector(const float ptA, const float ptB, const float ptC)
{
	V[0] = ptA;
	V[1] = ptB;
	V[2] = ptC;
}

Vector::Vector(const int ptA, const int ptB, const int ptC)
{
	V[0] = (float) ptA;
	V[1] = (float) ptB;
	V[2] = (float) ptC;
}

Vector::Vector(const float Pt[3])
{
	V[0] = Pt[0];
	V[1] = Pt[1];
	V[2] = Pt[2];
}

//////////////////////////////////////////////////////////////////////
// Ordinary Fns
//////////////////////////////////////////////////////////////////////

void Vector::initialise(const float a, const float b, const float c)
{
	V[0] = a;
	V[1] = b;
	V[2] = c;
}

Vector Vector::operator+(const Vector &Vin)
{
	Vector v(Vin.V[0] + V[0], Vin.V[1] + V[1], Vin.V[2] + V[2]);
	return v;
}

Vector Vector::operator-(const Vector &Vin)
{
	Vector v(V[0] - Vin.V[0], V[1] - Vin.V[1], V[2] - Vin.V[2]);
	return v;
}

Vector Vector::operator/(const float a)
{
	Vector v(V[0]/a, V[1]/a, V[2]/a);
	return v;
}

Vector Vector::operator/(const Vector &Vin)
{
	Vector v(V[0]/ ((Vector &) Vin)[0], 
			V[1]/ ((Vector &) Vin)[1], 
			V[2]/ ((Vector &) Vin)[2]);
	return v;
}

Vector Vector::operator*(const float a)
{
	Vector v(V[0]*a, V[1]*a, V[2]*a);
	return v;
}

Vector Vector::operator*(const Vector &Vin)
{
	Vector v(V[1] * ((Vector &)Vin)[2] - V[2] * ((Vector &)Vin)[1], 
			 V[2] * ((Vector &)Vin)[0] - V[0] * ((Vector &)Vin)[2],
			 V[0] * ((Vector &)Vin)[1] - V[1] * ((Vector &)Vin)[0]);

	return v;
}

void Vector::operator*=(const Vector &Vin)
{
	float a = V[1] * ((Vector &)Vin)[2] - V[2] * ((Vector &)Vin)[1];
	float b = V[2] * ((Vector &)Vin)[0] - V[0] * ((Vector &)Vin)[2];
	float c = V[0] * ((Vector &)Vin)[1] - V[1] * ((Vector &)Vin)[0];
	V[0] = a;
	V[1] = b;
	V[2] = c;
}

Vector Vector::operator-()
{
	Vector v(-V[0], -V[1], -V[2]);
	return v;
}

Vector Vector::Normalize()
{
	float mag = Magnitude();
	Vector v;
	if (mag != 0.0f)
	{
		v = (*this) / mag;
	}
	return v;
}

Vector Vector::Normalize2D()
{
	float mag = float(sqrt(V[0]*V[0] + V[1]*V[1]));
	Vector v;
	if (mag != 0.0f)
	{
		v = (*this) / mag;
	}
	return v;
}

float Vector::Magnitude()
{
	return float(sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]));
}

void Vector::StoreInvert()
{
	V[0] = -V[0];
	V[1] = -V[1];
	V[2] = -V[2];
}

void Vector::StoreNormalize()
{
	float mag = Magnitude();
	if (mag != 0.0f)
	{
		(*this) /= mag;
	}
}

float Vector::dotP(const Vector &Vin)
{
	Vector &V1 = (*this);
	Vector &V2 = (Vector &) Vin;
	return (V1.V[0] * V2.V[0]) + (V1.V[1] * V2.V[1]) + (V1.V[2] * V2.V[2]);
}

Vector Vector::operator+(const float m)
{
	Vector v(V[0]+m, V[1]+m, V[2]+m);
	return v;
}

Vector Vector::operator-(const float m)
{
	Vector v(V[0]-m, V[1]-m, V[2]-m);
	return v;
}

void Vector::operator-=(const Vector &Vin)
{
	V[0] -= Vin.V[0];
	V[1] -= Vin.V[1];
	V[2] -= Vin.V[2];
}

void Vector::operator+=(const Vector &Vin)
{
	V[0] += Vin.V[0];
	V[1] += Vin.V[1];
	V[2] += Vin.V[2];
}

void Vector::operator*=(const float a)
{
	V[0] *= a;
	V[1] *= a;
	V[2] *= a;
}

void Vector::operator/=(const float a)
{
	V[0] /= a;
	V[1] /= a;
	V[2] /= a;
}

void Vector::operator/=(const Vector &Vin)
{
	V[0] /= ((Vector &)Vin)[0];
	V[1] /= ((Vector &)Vin)[1];
	V[2] /= ((Vector &)Vin)[2];
}

void Vector::operator+=(const float a)
{
	V[0] += a;
	V[1] += a;
	V[2] += a;
}

void Vector::operator-=(const float a)
{
	(*this) += -a;
}

bool Vector::operator==(const Vector &Vin1)
{
	return (Vin1.V[0]==V[0] && Vin1.V[1]==V[1] && Vin1.V[2]==V[2]);
}

bool Vector::operator!=(const Vector &Vin1)
{
	return !((*this) == Vin1);
}

Vector Vector::get2DPerp()
{
	Vector v(V[1], -V[0], 0.0f);

	return v;
}

void Vector::zero()
{
	V[0] = V[1] = V[2] = 0.0f;
}

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


// Vector.h: interface for the Vector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR_H__AD959187_7A1C_11D2_957C_00A0C9A4CA3E__INCLUDED_)
#define AFX_VECTOR_H__AD959187_7A1C_11D2_957C_00A0C9A4CA3E__INCLUDED_

#pragma warning(disable: 4786)

class Vector  
{
	float V[3];
public:
	Vector();
	Vector(const Vector &v);
	Vector(const float Pt[3]);
	Vector(const float ang, const double length);
	Vector(const float ptA, const float ptB, const float ptC=0.0f);
	Vector(const int ptA, const int ptB, const int ptC=0);

	void initialise(const float a, const float b, const float c);

	Vector Normalize();
	Vector Normalize2D();
	float Magnitude();
	float dotP(const Vector &Vin);
	Vector get2DPerp();
	void StoreInvert();
	void StoreNormalize();
	void zero();

	Vector operator+(const float a);
	Vector operator+(const Vector &Vin);
	Vector operator-(const float a);
	Vector operator-(const Vector &Vin);
	Vector operator*(const float a);
	Vector operator*(const Vector &Vin);
	Vector operator/(const float a);
	Vector operator/(const Vector &Vin);
	Vector operator-();
	void operator*=(const float a);
	void operator*=(const Vector &Vin);
	void operator/=(const float a);
	void operator/=(const Vector &Vin);
	void operator+=(const float a);
	void operator+=(const Vector &Vin);
	void operator-=(const float a);
	void operator-=(const Vector &Vin);
	bool operator==(const Vector &Vin1);
	bool operator!=(const Vector &Vin1);

	float &operator[](const int m) { return V[m]; }
	float const &operator[](const int m) const { return V[m]; }

	operator float*() { return V; }

};

#endif // !defined(AFX_VECTOR_H__AD959187_7A1C_11D2_957C_00A0C9A4CA3E__INCLUDED_)

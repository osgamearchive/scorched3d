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

#if !defined(AFX_Vector4_H__AD959187_7A1C_11D2_957C_00A0C9A4CA3E__INCLUDED_)
#define AFX_Vector4_H__AD959187_7A1C_11D2_957C_00A0C9A4CA3E__INCLUDED_

#include <common/Vector.h>

class Vector4  
{
public:
	static Vector4 nullVector;

	Vector4()
	{
		V[0] = V[1] = V[2] = V[3] = 0.0f;
	}

	Vector4(const Vector &v)
	{
		V[0] = ((Vector &) v)[0];
		V[1] = ((Vector &) v)[1];
		V[2] = ((Vector &) v)[2];
		V[3] = 1.0f;
	}

	Vector4(const Vector4 &v)
	{
		V[0] = ((Vector4 &) v)[0];
		V[1] = ((Vector4 &) v)[1];
		V[2] = ((Vector4 &) v)[2];
		V[3] = ((Vector4 &) v)[3];
	}

	Vector4(const float Pt[4])
	{
		V[0] = Pt[0];
		V[1] = Pt[1];
		V[2] = Pt[2];
		V[3] = Pt[3];
	}

	Vector4(const float ptA, const float ptB, const float ptC, float ptD=0.0f)
	{
		V[0] = ptA;
		V[1] = ptB;
		V[2] = ptC;
		V[3] = ptD;
	}

	Vector4(const int ptA, const int ptB, const int ptC, const int ptD=0)
	{
		V[0] = (float) ptA;
		V[1] = (float) ptB;
		V[2] = (float) ptC;
		V[3] = (float) ptD;
	}

	void zero()
	{
		V[0] = V[1] = V[2] = V[3] = 0.0f;
	}

	bool operator==(const Vector4 &Vin1)
	{
		return (Vin1.V[0]==V[0] && Vin1.V[1]==V[1] && Vin1.V[2]==V[2] && Vin1.V[3]==V[3]);
	}

	bool operator!=(const Vector4 &Vin1)
	{
		return !((*this) == Vin1);
	}

	float &operator[](const int m) { DIALOG_ASSERT(m<=3); return V[m]; }
	float const &operator[](const int m) const { DIALOG_ASSERT(m<=3); return V[m]; }

	operator float*() { return V; }

protected:
	float V[4];

};

#endif // !defined(AFX_Vector4_H__AD959187_7A1C_11D2_957C_00A0C9A4CA3E__INCLUDED_)


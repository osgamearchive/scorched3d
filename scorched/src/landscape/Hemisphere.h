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

#if !defined(AFX_HEMISPHERE_H__3577D267_2B6C_4300_B0EE_61E1E50E57DD__INCLUDED_)
#define AFX_HEMISPHERE_H__3577D267_2B6C_4300_B0EE_61E1E50E57DD__INCLUDED_

#include <GLEXT/GLVertexArray.h>

class GLBitmap;
class Vector;
class Hemisphere  
{
public:
	static void draw(float radius, float radius2,
		int heightSlices = 10, int rotationSlices = 20,
		int startHeightSlice = 0, int startRotationSlice = 0,
		bool inverse = false);
	static GLVertexArray *createXY(float radius, float radius2,
		int heightSlices = 10, int rotationSlices = 20,
		int startHeightSlice = 0, int startRotationSlice = 0);
	static GLVertexArray *createColored(float radius, float radius2, 
		int heightSlices, int rotationSlices,
		GLBitmap &colors,
		Vector &sunDir,
		int colorIndex);

private:
	Hemisphere();
	virtual ~Hemisphere();
};

#endif // !defined(AFX_HEMISPHERE_H__3577D267_2B6C_4300_B0EE_61E1E50E57DD__INCLUDED_)

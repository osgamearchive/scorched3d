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


// HeightMapSurround.cpp: implementation of the HeightMapSurround class.
//
//////////////////////////////////////////////////////////////////////

#include <GLEXT/GLState.h>
#include <landscape/HeightMapSurround.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HeightMapSurround::HeightMapSurround(SurroundDefs &defs) :
	defs_(defs)
{

}

HeightMapSurround::~HeightMapSurround()
{

}

void HeightMapSurround::draw()
{
	static GLuint listNo = 0;
	if (listNo)
	{
		glCallList(listNo);
	}
	else
	{
		glNewList(listNo = glGenLists(1), GL_COMPILE);
			generateList();
		glEndList();
	}
}

void HeightMapSurround::generateList()
{
	const int dataOfs[8][4] = {
		{8,11,3,0},
		{1,2,10,9},
		{4,8,0,12},
		{11,7,15,3},
		{3,15,14,2},
		{2,14,6,10},
		{13,1,9,5},
		{12,0,1,13}
	};

	const float mapping[8][4][2] = { 
		{{0,0}, {1,0}, {1,1}, {0,1}},
		{{0,0}, {1,0}, {1,1}, {0,1}},
		{{0,0}, {1,0}, {1,1}, {0,1}},
		{{0,0}, {1,0}, {1,1}, {0,1}},
		{{0,0}, {1,0}, {1,1}, {0,1}},
		{{0,0}, {1,0}, {1,1}, {0,1}},
		{{0,0}, {1,0}, {1,1}, {0,1}},
		{{0,0}, {1,0}, {1,1}, {0,1}}
	};

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	for (int i=0; i<8; i++) 
	{
		for (int j=0; j<4; j++) 
		{
			float x = defs_.getHmapBoxVerts()[dataOfs[i][j]][0];
			float y = defs_.getHmapBoxVerts()[dataOfs[i][j]][1];

			x /= 64.0f;
			y /= 64.0f;

			glTexCoord2f(x, y);
			Vector pos = defs_.getHmapBoxVerts()[dataOfs[i][j]];
			if (pos.Magnitude()> 500) pos[2] -= 15.0f;
			glVertex3fv(pos);
		}
	}
	glEnd();
}

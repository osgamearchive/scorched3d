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


// SurroundDefs.cpp: implementation of the SurroundDefs class.
//
//////////////////////////////////////////////////////////////////////

#include <landscape/SurroundDefs.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SurroundDefs::SurroundDefs(HeightMap &map, int width, int height)
{
	Vector centre(map.getWidth() / 2, map.getWidth() / 2, height);
	Vector offset(width, width, height);

	skyBoxVerts_[0] = Vector(centre[0] - offset[0], centre[1] + offset[1], centre[2] + offset[2]);
	skyBoxVerts_[1] = Vector(centre[0] + offset[0], centre[1] + offset[1], centre[2] + offset[2]);
	skyBoxVerts_[2] = Vector(centre[0] + offset[0], centre[1] + offset[1], centre[2] - offset[2]);
	skyBoxVerts_[3] = Vector(centre[0] - offset[0], centre[1] + offset[1], centre[2] - offset[2]);
	skyBoxVerts_[4] = Vector(centre[0] - offset[0], centre[1] - offset[1], centre[2] + offset[2]);
	skyBoxVerts_[5] = Vector(centre[0] + offset[0], centre[1] - offset[1], centre[2] + offset[2]);
	skyBoxVerts_[6] = Vector(centre[0] + offset[0], centre[1] - offset[1], centre[2] - offset[2]);
	skyBoxVerts_[7] = Vector(centre[0] - offset[0], centre[1] - offset[1], centre[2] - offset[2]);

	Vector offset2(map.getWidth() / 2, map.getWidth() / 2, height);
	hMapBoxVerts_[0] = Vector(centre[0] - offset2[0], centre[1] - offset2[1], centre[2] - offset[2]);
	hMapBoxVerts_[1] = Vector(centre[0] - offset2[0], centre[1] + offset2[1], centre[2] - offset[2]);
	hMapBoxVerts_[2] = Vector(centre[0] + offset2[0], centre[1] + offset2[1], centre[2] - offset[2]);
	hMapBoxVerts_[3] = Vector(centre[0] + offset2[0], centre[1] - offset2[1], centre[2] - offset[2]);

	Vector offset3(width + map.getWidth() * 2, width + map.getWidth() * 2, height);
	hMapBoxVerts_[4] = Vector(centre[0] - offset3[0], centre[1] - offset3[1], centre[2] - offset[2]);
	hMapBoxVerts_[5] = Vector(centre[0] - offset3[0], centre[1] + offset3[1], centre[2] - offset[2]);
	hMapBoxVerts_[6] = Vector(centre[0] + offset3[0], centre[1] + offset3[1], centre[2] - offset[2]);
	hMapBoxVerts_[7] = Vector(centre[0] + offset3[0], centre[1] - offset3[1], centre[2] - offset[2]);

	hMapBoxVerts_[8] = Vector(centre[0] - offset2[0], centre[1] - offset3[1], centre[2] - offset[2]);
	hMapBoxVerts_[9] = Vector(centre[0] - offset2[0], centre[1] + offset3[1], centre[2] - offset[2]);
	hMapBoxVerts_[10] = Vector(centre[0] + offset2[0], centre[1] + offset3[1], centre[2] - offset[2]);
	hMapBoxVerts_[11] = Vector(centre[0] + offset2[0], centre[1] - offset3[1], centre[2] - offset[2]);

	hMapBoxVerts_[12] = Vector(centre[0] - offset3[0], centre[1] - offset2[1], centre[2] - offset[2]);
	hMapBoxVerts_[13] = Vector(centre[0] - offset3[0], centre[1] + offset2[1], centre[2] - offset[2]);
	hMapBoxVerts_[14] = Vector(centre[0] + offset3[0], centre[1] + offset2[1], centre[2] - offset[2]);
	hMapBoxVerts_[15] = Vector(centre[0] + offset3[0], centre[1] - offset2[1], centre[2] - offset[2]);
}

SurroundDefs::~SurroundDefs()
{

}

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


// SurroundDefs.h: interface for the SurroundDefs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SURROUNDDEFS_H__5D5E88CC_1BA0_4A4B_BF2C_E9B2675A7A8B__INCLUDED_)
#define AFX_SURROUNDDEFS_H__5D5E88CC_1BA0_4A4B_BF2C_E9B2675A7A8B__INCLUDED_


#include <landscape/HeightMap.h>

class SurroundDefs  
{
public:
	SurroundDefs(HeightMap &map, int width, int height);
	virtual ~SurroundDefs();

	Vector *getSkyBoxVerts() { return skyBoxVerts_; }
	Vector *getHmapBoxVerts() { return hMapBoxVerts_; }

protected:
	Vector skyBoxVerts_[8];
	Vector hMapBoxVerts_[16];

};

#endif // !defined(AFX_SURROUNDDEFS_H__5D5E88CC_1BA0_4A4B_BF2C_E9B2675A7A8B__INCLUDED_)

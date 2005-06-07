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


// GLState.h: interface for the GLState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLSTATE_H__32B0E2D0_566D_4438_94E4_B12FE82430B1__INCLUDED_)
#define AFX_GLSTATE_H__32B0E2D0_566D_4438_94E4_B12FE82430B1__INCLUDED_

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <SDL/SDL_opengl.h>
#include <common/Defines.h>

class GLState  
{
public:
	enum State
	{
		TEXTURE_ON = 0x1,
		TEXTURE_OFF = 0x2,
		BLEND_ON = 0x4,
		BLEND_OFF = 0x8,
		DEPTH_ON = 0x10,
		DEPTH_OFF = 0x20,
		CUBEMAP_ON = 0x40,
		CUBEMAP_OFF = 0x80
	};

	GLState(unsigned wantedState);
	virtual ~GLState();

	static void setBaseState(unsigned baseState);
	static char *getStateString();
	static unsigned int getState() { return currentState_; }

protected:
	static unsigned currentState_;
	unsigned returnState_;

	static void setState(unsigned wanted);

};

#endif // !defined(AFX_GLSTATE_H__32B0E2D0_566D_4438_94E4_B12FE82430B1__INCLUDED_)

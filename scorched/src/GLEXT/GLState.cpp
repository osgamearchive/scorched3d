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


// GLState.cpp: implementation of the GLState class.
//
//////////////////////////////////////////////////////////////////////

#include <GLEXT/GLState.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

unsigned GLState::currentState_ = 
	GLState::TEXTURE_OFF | GLState::BLEND_OFF | GLState::DEPTH_OFF | GLState::CUBEMAP_OFF;

GLState::GLState(unsigned wantedState)
{
	returnState_ = currentState_;
	setState(wantedState);
}

GLState::~GLState()
{
	setState(returnState_);
}

void GLState::setState(unsigned wanted)
{
	if ((wanted & TEXTURE_ON) && (currentState_ & TEXTURE_OFF))
	{
		currentState_ ^= TEXTURE_OFF;
		currentState_ |= TEXTURE_ON;
		glEnable(GL_TEXTURE_2D);
	}
	else if ((wanted & TEXTURE_OFF) && (currentState_ & TEXTURE_ON))
	{
		currentState_ ^= TEXTURE_ON;
		currentState_ |= TEXTURE_OFF;
		glDisable(GL_TEXTURE_2D);
	}

	if ((wanted & DEPTH_ON) && (currentState_ & DEPTH_OFF))
	{
		currentState_ ^= DEPTH_OFF;
		currentState_ |= DEPTH_ON;
		glEnable(GL_DEPTH_TEST);
	}
	else if ((wanted & DEPTH_OFF) && (currentState_ & DEPTH_ON))
	{
		currentState_ ^= DEPTH_ON;
		currentState_ |= DEPTH_OFF;
		glDisable(GL_DEPTH_TEST);
	}

	if ((wanted & BLEND_ON) && (currentState_ & BLEND_OFF))
	{
		currentState_ ^= BLEND_OFF;
		currentState_ |= BLEND_ON;
		glEnable(GL_BLEND);
	}
	else if ((wanted & BLEND_OFF) && (currentState_ & BLEND_ON))
	{
		currentState_ ^= BLEND_ON;
		currentState_ |= BLEND_OFF;
		glDisable(GL_BLEND);
	}

	if ((wanted & CUBEMAP_ON) && (currentState_ & CUBEMAP_OFF))
	{
		currentState_ ^= CUBEMAP_OFF;
		currentState_ |= CUBEMAP_ON;
		glEnable(GL_TEXTURE_CUBE_MAP_EXT);
	}
	else if ((wanted & CUBEMAP_OFF) && (currentState_ & CUBEMAP_ON))
	{
		currentState_ ^= CUBEMAP_ON;
		currentState_ |= CUBEMAP_OFF;
		glDisable(GL_TEXTURE_CUBE_MAP_EXT);
	}
}

char *GLState::getStateString()
{
	static char buffer[1024];
	buffer[0] = '\0';

	if (currentState_ & TEXTURE_ON) strcat(buffer, "TEXTURE_ON\n");
	if (currentState_ & TEXTURE_OFF) strcat(buffer, "TEXTURE_ON\n");
	if (currentState_ & DEPTH_ON) strcat(buffer, "DEPTH_ON\n");
	if (currentState_ & DEPTH_OFF) strcat(buffer, "DEPTH_OFF\n");
	if (currentState_ & BLEND_ON) strcat(buffer, "BLEND_ON\n");
	if (currentState_ & BLEND_OFF) strcat(buffer, "BLEND_OFF\n");
	if (currentState_ & CUBEMAP_ON) strcat(buffer, "CUBEMAP_ON\n");
	if (currentState_ & CUBEMAP_OFF) strcat(buffer, "CUBEMAP_OFF\n");

	return buffer;
}

void GLState::setBaseState(unsigned bs)
{
	setState(bs);
}

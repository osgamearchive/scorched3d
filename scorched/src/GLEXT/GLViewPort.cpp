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

#include <GLEXT/GLViewPort.h>
#include <GLEXT/GLState.h>

int GLViewPort::width_ = 0;
int GLViewPort::height_ = 0;
int GLViewPort::actualWidth_ = 0;
int GLViewPort::actualHeight_ = 0;
float GLViewPort::widthMult_ = 0.0f;
float GLViewPort::heightMult_ = 0.0f;

GLViewPort::GLViewPort()
{
}

GLViewPort::~GLViewPort()
{
}

void GLViewPort::draw()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	glViewport(0, 0, actualWidth_, actualHeight_);
	glOrtho(0.0, float(width_), 
			0.0, float(height_), 
			-4000.0, 4000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLViewPort::setWindowSize(int width, int height,
							   int awidth, int aheight)
{
	width_ = width;
	height_ = height;
	actualWidth_ = awidth;
	actualHeight_ = aheight;

	widthMult_ = (float) width_ / (float) actualWidth_;
	heightMult_ = (float) height_ / (float) actualHeight_;
}

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


// GLBitmapItterator.cpp: implementation of the GLBitmapItterator class.
//
//////////////////////////////////////////////////////////////////////

#include <GLEXT/GLBitmapItterator.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLBitmapItterator::GLBitmapItterator(GLBitmap &bitmap,
									int destX,
									int destY,
									Type type)
	: bitmap_(bitmap), type_(type)
{
	if (type == stretch)
	{
		dx_ = (GLfloat) bitmap.getWidth() / (GLfloat) destX;
		dy_ = (GLfloat) bitmap.getHeight() / (GLfloat) destY;
	}
	else
	{
		dx_ = 1;
		dy_ = 1;
	}
	width_ = 3 * bitmap.getWidth();
	width_   = (width_ + 3) & ~3;	

	reset();
}

GLBitmapItterator::~GLBitmapItterator()
{

}

void GLBitmapItterator::reset()
{
	posX_ = posY_ = 0.0f;
	pos_ = bitmap_.getBits();
}

void GLBitmapItterator::incX()
{
	posX_ += dx_;
	if (posX_ >= bitmap_.getWidth()) posX_ = 0.0f;
}

void GLBitmapItterator::incY()
{
	posX_ = 0.0f;
	posY_ += dy_;
	if (posY_ >= bitmap_.getHeight()) posY_ = 0.0f;
	pos_ = (GLubyte*) (bitmap_.getBits() + ((int) posY_  * width_));
}

GLubyte *GLBitmapItterator::getPos()
{
	return pos_ + (int) posX_ * 3;
}

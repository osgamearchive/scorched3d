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


// GLBitmapItterator.h: interface for the GLBitmapItterator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLBITMAPITTERATOR_H__F83D9F30_02DF_44EC_B2E7_1F4D87D6FBC8__INCLUDED_)
#define AFX_GLBITMAPITTERATOR_H__F83D9F30_02DF_44EC_B2E7_1F4D87D6FBC8__INCLUDED_


#include <GLEXT/GLBitmap.h>

class GLBitmapItterator  
{
public:
	enum Type
	{
		wrap,
		stretch
	};

	GLBitmapItterator(GLBitmap &bitmap, 
						int destX,
						int destY,
						Type type = stretch);
	virtual ~GLBitmapItterator();

	void reset();
	void incX();
	void incY();
	GLubyte *getPos();

protected:
	GLBitmap &bitmap_;
	GLfloat dx_, dy_;
	INT width_;
	Type type_;

	GLubyte *pos_;
	GLfloat posX_, posY_;
};

#endif // !defined(AFX_GLBITMAPITTERATOR_H__F83D9F30_02DF_44EC_B2E7_1F4D87D6FBC8__INCLUDED_)

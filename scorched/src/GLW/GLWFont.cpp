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


// GLWFont.cpp: implementation of the GLWFont class.
//
//////////////////////////////////////////////////////////////////////

#include <GLW/GLWFont.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Vector GLWFont::widgetFontColor = Vector(0.2f, 0.2f, 0.2f);

GLWFont *GLWFont::instance_ = 0;

GLWFont *GLWFont::instance()
{
	if (!instance_)
	{
		instance_ = new GLWFont;
	}

	return instance_;
}

GLWFont::GLWFont() : courierFont_(0)
{
	courierFont_ = new GLFont2d;
	courierFont_->createFont(
		PKGDIR "data/fonts/data.bmp", 
		PKGDIR "data/fonts/datainv.bmp");

	courierFontOverLap_= new GLFont2d;
	courierFontOverLap_->createFont(
		PKGDIR "data/fonts/data.bmp", 
		PKGDIR "data/fonts/datainv.bmp",
		0.65f * 16.0f / 20.0f);
}

GLWFont::~GLWFont()
{

}

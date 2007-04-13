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

#if !defined(AFX_GLImageFactory_H__0EBAA0E7_3103_43A4_90C0_5708ECE6DB43__INCLUDED_)
#define AFX_GLImageFactory_H__0EBAA0E7_3103_43A4_90C0_5708ECE6DB43__INCLUDED_

#include <GLEXT/GLImageHandle.h>

class GLImageFactory  
{
public:
	static GLImage *loadImage(
		const char *filename, 
		const char *alphafilename = 0, 
		bool invert = true);
	static GLImage *loadAlphaImage(
		const char *filename);

	static GLImageHandle loadImageHandle(
		const char *filename, 
		const char *alphafilename = 0, 
		bool invert = true);
	static GLImageHandle loadAlphaImageHandle(
		const char *filename);

private:
	GLImageFactory();
};

#endif // !defined(AFX_GLImageFactory_H__0EBAA0E7_3103_43A4_90C0_5708ECE6DB43__INCLUDED_)

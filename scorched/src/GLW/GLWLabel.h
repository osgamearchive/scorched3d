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


// GLWLabel.h: interface for the GLWLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLWLABEL_H__75483479_A6F8_45CC_8E83_B517E721211F__INCLUDED_)
#define AFX_GLWLABEL_H__75483479_A6F8_45CC_8E83_B517E721211F__INCLUDED_

#pragma warning(disable: 4786)


#include <string>
#include <GLW/GLWVisibleWidget.h>

class GLWLabel : public GLWVisibleWidget  
{
public:
	GLWLabel(float x, float y, char *buttonText = 0);
	virtual ~GLWLabel();

	virtual void draw();	
	const char *getText() { return buttonText_.c_str(); }
	void setText(const char *text);

METACLASSID

protected:
	std::string buttonText_;

};

#endif // !defined(AFX_GLWLABEL_H__75483479_A6F8_45CC_8E83_B517E721211F__INCLUDED_)

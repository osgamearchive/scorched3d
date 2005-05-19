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

#if !defined(AFX_GLWLABEL_H__75483479_A6F8_45CC_8E83_B517E721211F__INCLUDED_)
#define AFX_GLWLABEL_H__75483479_A6F8_45CC_8E83_B517E721211F__INCLUDED_

#include <string>
#include <GLW/GLWidget.h>
#include <common/Vector.h>

class GLWLabel : public GLWidget  
{
public:
	GLWLabel(float x = 0.0f, float y = 0.0f, 
		char *buttonText = 0,
		float size = 14.0f);
	virtual ~GLWLabel();

	virtual float getW() { calcWidth(); return GLWidget::getW(); }
	virtual void draw();

	const char *getText() { return buttonText_.c_str(); }
	void setText(const char *text);
	void setColor(Vector &color);
	void setSize(float size);
	void calcWidth();

	REGISTER_CLASS_HEADER(GLWLabel);

protected:
	std::string buttonText_;
	Vector color_;
	float size_;

};

#endif // !defined(AFX_GLWLABEL_H__75483479_A6F8_45CC_8E83_B517E721211F__INCLUDED_)

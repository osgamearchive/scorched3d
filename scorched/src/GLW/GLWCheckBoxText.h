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

#if !defined(__INCLUDE_GLWCheckBoxTexth_INCLUDE__)
#define __INCLUDE_GLWCheckBoxTexth_INCLUDE__

#include <GLW/GLWLabel.h>
#include <GLW/GLWCheckBox.h>

class GLWCheckBoxText : public GLWidget
{
public:
	GLWCheckBoxText(float x = 0.0f, float y = 0.0f, 
		const char *text = "",
		bool startState = true);
	virtual ~GLWCheckBoxText();

	virtual void draw();
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);

	virtual float getW();
	virtual void setX(float x);
	virtual void setY(float y);

	GLWCheckBox &getCheckBox() { return box_; }
	GLWLabel &getLabel() { return label_; }

	REGISTER_CLASS_HEADER(GLWCheckBoxText);

protected:
	GLWCheckBox box_;
	GLWLabel label_;
};

#endif

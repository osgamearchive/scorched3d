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

#ifndef _GLWSLIDER_H_
#define _GLWSLIDER_H_

#include <GLW/GLWidget.h>

class GLWSliderI
{
public:
	virtual void currentChanged(unsigned int id, float value) = 0;
};

class GLWSlider : public GLWidget
{
public:
	GLWSlider(float x = 0.0f, float y = 0.0f, float w = 0.0f, 
		float range = 0.0f);
	virtual ~GLWSlider();

	void setHandler(GLWSliderI *handler) { handler_ = handler; }

	float getCurrent() { return current_; }
	void setCurrent(float current) { current_ = current; }

	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);

	REGISTER_CLASS_HEADER(GLWSlider);

protected:
	GLWSliderI *handler_;
	bool dragging_;
	float current_;
	float range_;

};

class GLWTankSlider : public GLWSlider, 
	public GLWSliderI
{
public:
	GLWTankSlider();
	virtual ~GLWTankSlider();

	virtual void draw();
	virtual void currentChanged(unsigned int id, float value);

	REGISTER_CLASS_HEADER(GLWTankSlider);

};

#endif /* _GLWSLIDER_H_ */

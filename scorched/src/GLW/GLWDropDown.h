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


#if !defined(__INCLUDE_GLWDropDownh_INCLUDE__)
#define __INCLUDE_GLWDropDownh_INCLUDE__

#include <list>
#include <string>
#include <GLW/GLWPushButton.h>
#include <GLW/GLWVisibleWidget.h>

class GLWDropDownEntry
{
public:
	GLWDropDownEntry(const char *name = "",
		GLWTip *tip = 0) :
		text_(name), tip_(tip)
	{
	}

	std::string text_;
	GLWTip *tip_;
};

class GLWDropDownI
{
public:
	virtual ~GLWDropDownI();

	virtual void select(unsigned int id, const int pos, GLWDropDownEntry value) = 0;
};

class GLWDropDown : public GLWVisibleWidget
{
public:
	GLWDropDown(float x = 0.0f, float y = 0.0f, float w = 0.0f);
	virtual ~GLWDropDown();

	void setHandler(GLWDropDownI *handler);
	void addText(GLWDropDownEntry text);
	void setText(const char *text) { text_ = text; }
	const char *getText() { return text_.c_str(); }
	void setCurrentPosition(int pos);
	int getCurrentPosition();

	virtual void draw();
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);

	void clear();

	REGISTER_CLASS_HEADER(GLWDropDown);
protected:
	std::string text_;
	std::list<GLWDropDownEntry> texts_;
	GLWPushButton button_;
	GLWDropDownI *handler_;

};


#endif

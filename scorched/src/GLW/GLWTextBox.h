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

#ifndef _GLWTEXTBOX_H_
#define _GLWTEXTBOX_H_

#include <string>
#include <GLW/GLWVisibleWidget.h>

class GLWTextBox : public GLWVisibleWidget
{
public:
	GLWTextBox(float x, float y, float w, char *startText = 0);
	virtual ~GLWTextBox();

	virtual void draw();
	virtual void simulate(float frameTime);
	virtual void keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

	std::string &getText() { return text_; }
	void setText(const char *text) { text_ = text; }
	void setMaxTextLen(unsigned int maxLen) { maxTextLen_ = maxLen; }

METACLASSID

protected:
	float ctime_;
	bool cursor_;
	unsigned int maxTextLen_;
	std::string text_;

};

#endif /* _GLWTEXTBOX_H_ */

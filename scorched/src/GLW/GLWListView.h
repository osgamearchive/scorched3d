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


#if !defined(__INCLUDE_GLWListViewh_INCLUDE__)
#define __INCLUDE_GLWListViewh_INCLUDE__
#include <GLW/GLWVisibleWidget.h>
#include <GLW/GLWScrollW.h>
#include <common/FileLines.h>
#include <vector>
#include <string>

class GLWListView :
	public GLWVisibleWidget
{
public:
	GLWListView(float x, float y, float w, float h, int maxLen = -1);
	virtual ~GLWListView();

	void addLine(const char *fmt, ...);

	virtual void draw();
	virtual void simulate(float frameTime);
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);

	virtual void setW(float w) { w_ = w; scroll_.setX(x_ + w_ - 17); }
	virtual void setH(float h) { h_ = h; scroll_.setH(h_); }

METACLASSID

protected:
	int maxLen_;
	GLWScrollW scroll_;
	std::vector<std::string> lines_;
};


#endif

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

#if !defined(__INCLUDE_GLWSelectorParth_INCLUDE__)
#define __INCLUDE_GLWSelectorParth_INCLUDE__

#include <GLW/GLWSelector.h>

class GLWSelectorPart
{
public:
	GLWSelectorPart(GLWSelectorI *user,
		int basePosition,
		float x, float y,
		std::list<GLWSelectorEntry> &entries,
		bool transparent);
	virtual ~GLWSelectorPart();

	void draw();
	void mouseDown(float x, float y, bool &hit);
	
	float getSelectedHeight() { return selectedHeight_; }
	float getSelectedWidth() { return selectedWidth_; }

protected:
	GLWSelectorI *user_;
	std::list<GLWSelectorEntry> entries_;
	float selectedHeight_, selectedWidth_;
	float selectedX_, selectedY_;
	float selectedIndent_;
	int basePosition_;
	bool transparent_;
	bool selected_;

	void calculateDimensions(float x, float y);

};

#endif

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


// GLMenuEntry.h: interface for the GLMenuEntry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLMENUENTRY_H__5AE3319D_D83E_4392_BABB_7E34B2D69BD8__INCLUDED_)
#define AFX_GLMENUENTRY_H__5AE3319D_D83E_4392_BABB_7E34B2D69BD8__INCLUDED_

#pragma warning(disable: 4786)


#include <GLEXT/GLMenuI.h>
#include <GLEXT/GLFont2d.h>

class GLMenuEntry  
{
public:
	GLMenuEntry(char *name, float width, 
				GLMenuI *selectFn, GLMenuI *textFn, 
				GLMenuI *subMenuFn, GLMenuI *enableFn);
	virtual ~GLMenuEntry();

	bool click(float currentTop, int x, int y);
	bool getEnabled() { return (enabledFn_?
		enabledFn_->getEnabled(menuName_.c_str()):true); }
	void draw(GLFont2d &font, float currentTop, float currentLeft);

	void addMenuItem(const char *item);
	float getWidth() { return width_; }

protected:
	bool depressed_;
	float left_;
	float width_, height_;
	GLMenuI *selectFn_;
	GLMenuI *textFn_;
	GLMenuI *subMenuFn_;
	GLMenuI *enabledFn_;
	std::list<std::string> menuItems_;
	std::string menuName_;

	void collapse();
	void drawNonDepressed(float currentTop, float currentLeft);
	void drawDepressed(GLFont2d &font, float currentTop, float currentLeft);
	void drawBackdrop(float x, float y, float w, float w2, float h);
	bool clickTitle(float currentTop, int x);
	bool clickMenu(float currentTop, int x, int y);

};

#endif // !defined(AFX_GLMENUENTRY_H__5AE3319D_D83E_4392_BABB_7E34B2D69BD8__INCLUDED_)

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

#if !defined(__INCLUDE_GLWSelectorh_INCLUDE__)
#define __INCLUDE_GLWSelectorh_INCLUDE__

#include <GLW/GLWWindow.h>
#include <GLW/GLWToolTip.h>
#include <GLEXT/GLTexture.h>
#include <string>
#include <list>

class GLWSelectorI;
class GLWSelector : public GLWWindow
{
public:
	class Entry
	{
	public:
		Entry(const char *text = "", 
			GLWTip *tooltip = 0, 
			bool selected = false,
			GLTexture *icon = 0);

		const char *getText() { return text_.c_str(); }
		GLWTip *getToolTip() { return tip_; }
		GLTexture *getIcon() { return icon_; }
		bool getSelected() { return selected_; }

	protected:
		std::string text_;
		GLTexture *icon_;
		GLWTip *tip_;
		bool selected_;
	};

    static GLWSelector *instance();

	virtual void draw();
	void showSelector(GLWSelectorI *user,
		float x, float y,
		std::list<GLWSelector::Entry> &entries);

protected:
	static GLWSelector *instance_;

	GLWSelectorI *user_;
	std::list<GLWSelector::Entry> entries_;
	float drawX_, drawY_;
	bool visible_;

private:
	GLWSelector();
	virtual ~GLWSelector();

};

class GLWSelectorI
{
	virtual void itemSelected(GLWSelector::Entry *entry, int position) = 0;
};

#endif

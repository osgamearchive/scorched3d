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

#if !defined(__INCLUDE_GLWWindowSkinh_INCLUDE__)
#define __INCLUDE_GLWWindowSkinh_INCLUDE__

#include <GLW/GLWWindow.h>
#include <XML/XMLFile.h>

class GLWWindowSkin : public GLWWindow
{
public:
	GLWWindowSkin();
	virtual ~GLWWindowSkin();

	virtual bool initFromXML(XMLNode *node);

	const char *getStates() { return states_.c_str(); }
	const char *getKey() { return key_.c_str(); }
	bool getVisible() { return visible_; }
	void setVisible(bool visible) { visible_ = visible; }

protected:
	std::string states_;
	std::string key_;
	bool visible_;

};

#endif

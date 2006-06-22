////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#if !defined(__INCLUDE_MessageDialogh_INCLUDE__)
#define __INCLUDE_MessageDialogh_INCLUDE__

#include <GLW/GLWWindow.h>
#include <string>
#include <list>

class MessageDialog : public GLWWindow
{
public:
	static MessageDialog *instance();

	virtual void draw();
	virtual void simulate(float frameTime);

	void clear();
	void addMessage(const char *text);

protected:
	static MessageDialog *instance_;
	float showTime_;
	std::string currentText_;
	std::list<std::string> texts_;

private:
	MessageDialog();
	virtual ~MessageDialog();
};

#endif // __INCLUDE_MessageDialogh_INCLUDE__

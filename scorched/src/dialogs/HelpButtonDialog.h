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

#if !defined(__INCLUDE_HelpButtonDialogh_INCLUDE__)
#define __INCLUDE_HelpButtonDialogh_INCLUDE__

#include <GLW/GLWWindow.h>
#include <GLW/GLWSelector.h>
#include <GLEXT/GLTexture.h>

class HelpButtonDialog : public GLWWindow,
	public GLWSelectorI
{
public:
	static HelpButtonDialog *instance();

protected:
	static HelpButtonDialog *instance_;
	GLTexture helpTexture_;
	GLTexture soundTexture_;

	// GLWSelectorI
	virtual void itemSelected(GLWSelectorEntry *entry, int position);

	// GLWWindow
	virtual void draw();
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);
	virtual void keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

private:
	HelpButtonDialog();
	virtual ~HelpButtonDialog();
};

#endif

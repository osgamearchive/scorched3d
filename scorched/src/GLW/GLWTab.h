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


// GLWTab.h: interface for the GLWTab class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLWTAB_H__5A717DEA_0AE4_4341_9991_A4575E3FF041__INCLUDED_)
#define AFX_GLWTAB_H__5A717DEA_0AE4_4341_9991_A4575E3FF041__INCLUDED_


#include <GLW/GLWScrollPanel.h>
#include <GLW/GLWLabel.h>

class GLWTabI
{
public:
	virtual ~GLWTabI();

	virtual void tabDown(unsigned int id) = 0;
};

class GLWTab : public GLWScrollPanel
{
public:
	GLWTab(char *tabName, float index, float x, float y, float w, float h, float tw = 40.0f);
	virtual ~GLWTab();

	virtual void setParent(GLWPanel *parent);
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void draw();

	virtual void setH(float h);

	void setDepressed();
	void setHandler(GLWTabI *handler) { handler_ = handler; }

METACLASSID

protected:
	float tw_;
	float index_;
	bool depressed_;
	GLWLabel label_;
	GLWTabI *handler_;

	void drawSurround();
	void drawNonSurround();
};

#endif // !defined(AFX_GLWTAB_H__5A717DEA_0AE4_4341_9991_A4575E3FF041__INCLUDED_)

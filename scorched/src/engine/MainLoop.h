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


// MainLoop.h: interface for the MainLoop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MainLoop_H__6E7B84B0_E055_48B6_B992_4D4C3C7455E0__INCLUDED_)
#define AFX_MainLoop_H__6E7B84B0_E055_48B6_B992_4D4C3C7455E0__INCLUDED_

#pragma warning(disable: 4786)


#include <list>
#include <set>
#include <GLEXT/GLState.h>
#include <common/Timer.h>
#include <engine/MainLoopI.h>

class MainLoop
{
public:
	static MainLoop *instance();

	void addMainLoop(MainLoopI *MainLoop);

	bool mainLoop();
	void draw();

	void swapBuffers();
	Timer &getTimer() { return fTimer_; }

	void exitLoop() { exitLoop_ = true; }

protected:
	static MainLoop *instance_;
	std::list<MainLoopI *> newMainLoops_;
	std::set<MainLoopI *> mainLoops_;
	Timer fTimer_;
	bool exitLoop_;

	void simulate(float frameTime);
	void addNew();

private:
	MainLoop();
	virtual ~MainLoop();


};

#endif // !defined(AFX_MainLoop_H__6E7B84B0_E055_48B6_B992_4D4C3C7455E0__INCLUDED_)

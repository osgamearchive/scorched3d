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


#if !defined(__INCLUDE_WindowManagerh_INCLUDE__)
#define __INCLUDE_WindowManagerh_INCLUDE__

#include <map>
#include <deque>
#include <engine/GameStateI.h>
#include <GLW/GLWWindow.h>
#include <GLEXT/GLMenuI.h>

class WindowManager : 
	public GameStateI,
	public GLMenuI
{
public:
	static WindowManager *instance();

	void addWindow(const unsigned state, GLWWindow *window, 
		unsigned key = 0, bool visible = false);
	bool showWindow(unsigned id);
	bool hideWindow(unsigned id);
	bool windowVisible(unsigned id);
	bool moveToFront(unsigned id);

	// All inherited from GameStateI
	virtual void draw(const unsigned state);
	virtual void simulate(const unsigned state, float simTime);
	virtual void keyboardCheck(const unsigned state, float frameTime, 
							   char *buffer, int bufCount,
							   KeyboardHistory::HistoryElement *history, int hisCount, 
							   bool &skipRest);
	virtual void mouseDown(const unsigned state, GameState::MouseButton button, 
		int x, int y, bool &skipRest);
	virtual void mouseUp(const unsigned state, GameState::MouseButton button, 
		int x, int y, bool &skipRest);
	virtual void mouseDrag(const unsigned state, GameState::MouseButton button, 
		int mx, int my, int x, int y, bool &skipRest);

	// Inherited from GLMenuI
	virtual void getMenuItems(const char* menuName, std::list<std::string> &result);
	virtual void menuSelection(const char* menuName, 
		const int position, const char *menuItem);

protected:
	static WindowManager *instance_;

	// The collection that determines a windows visibility
	std::map<unsigned, bool> windowVisibility_;
	std::map<unsigned, GLWWindow *> idToWindow_;

	// The deque of windows in each state
	struct StateEntry
	{
		unsigned state_;
		std::map<unsigned, GLWWindow *> windowKeys_;
		std::deque<GLWWindow *> windows_;
	};
	// The collection of states
	std::map<unsigned, StateEntry> stateEntrys_;
	StateEntry *currentStateEntry_;

	void setCurrentEntry(const unsigned state);

private:
	WindowManager();
	virtual ~WindowManager();

};

#endif

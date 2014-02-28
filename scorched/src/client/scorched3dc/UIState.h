////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2013
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
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#if !defined(__INCLUDE_UIStateh_INCLUDE__)
#define __INCLUDE_UIStateh_INCLUDE__

#include <engine/ThreadCallback.h>

class UIStateI;
class UIStateMainMenu;
class UIStateProgress;
class UIStateJoining;
class UIStatePlaying;
class UIState
{
public:
	static UIState *instance();

	enum State
	{
		StateMainMenu,
		StateProgress,
		StateJoining,
		StatePlaying
	};

	void setState(State nextState);
	void setStateNonUIThread(State nextState);

	ThreadCallback &getUIThreadCallback() { return uiThreadCallback_; }

protected:
	static UIState *instance_;
	UIStateMainMenu *uiStateMenuMenu_;
	UIStateProgress *uiStateProgress_;
	UIStateJoining *uiStateJoining_;
	UIStatePlaying *uiStatePlaying_;
	UIStateI *currentState_;
	ThreadCallback uiThreadCallback_;

private:
	UIState();
	virtual ~UIState();
};

class UIStateThreadCallback : public ThreadCallbackI
{
public:
	UIStateThreadCallback(UIState::State state);
	virtual ~UIStateThreadCallback();

	// ThreadCallbackI
	virtual void callbackInvoked();

private:
	UIState::State state_;
};

#endif // __INCLUDE_UIStateh_INCLUDE__
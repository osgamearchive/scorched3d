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


#include <common/WindowManager.h>
#include <common/Keyboard.h>
#include <dialogs/MainMenuDialog.h>
#include <GLW/GLWWindow.h>

WindowManager *WindowManager::instance_ = 0;

WindowManager *WindowManager::instance()
{
	if (!instance_)
	{
		instance_ = new WindowManager;
	}

	return instance_;
}

WindowManager::WindowManager() : currentStateEntry_(0)
{
	setCurrentEntry(-1);
	MainMenuDialog::instance()->
		addMenu("Windows", 80.0f, this, 0, this, 0);
}

WindowManager::~WindowManager()
{

}

void WindowManager::setCurrentEntry(const unsigned state)
{
	static StateEntry defaultStateEntry;
	std::map<unsigned, StateEntry>::iterator itor =
		stateEntrys_.find(state);
	if (itor == stateEntrys_.end())
	{
		currentStateEntry_ = &defaultStateEntry;
		defaultStateEntry.state_ = state;
	}
	else
	{
		currentStateEntry_ = &(*itor).second;

		std::deque<GLWWindow *> windowsCopy = currentStateEntry_->windows_;
		std::deque<GLWWindow *>::iterator itor;
		for (itor = windowsCopy.begin();
			itor != windowsCopy.end();
			itor++)
		{
			(*itor)->windowInit(state);
			if (windowVisible((*itor)->getId()))
			{
				(*itor)->windowDisplay();
			}
		}
	}
}

void WindowManager::addWindow(const unsigned state, GLWWindow *window, KeyboardKey *key, bool visible)
{
	windowVisibility_[window->getId()] = visible;
	idToWindow_[window->getId()] = window;

	stateEntrys_[state].windows_.push_back(window);
	stateEntrys_[state].state_ = state;
	stateEntrys_[state].windowKeys_.push_back(
		std::pair<KeyboardKey *, GLWWindow *>(key, window));
}

bool WindowManager::showWindow(unsigned id)
{
	std::map<unsigned, bool>::iterator itor =
		windowVisibility_.find(id);
	if (itor != windowVisibility_.end())
	{
		if (!(*itor).second)
		{
			GLWWindow *window = idToWindow_[id];
			window->windowDisplay();
			(*itor).second = true;

			moveToFront(id);
		
			return true;
		}
	}

	return false;
}

bool WindowManager::moveToFront(unsigned id)
{
	GLWWindow *found = 0;
	std::deque<GLWWindow *> tmpList;

	while (!currentStateEntry_->windows_.empty())
	{
		GLWWindow *window = currentStateEntry_->windows_.front();
		currentStateEntry_->windows_.pop_front();
		if (window->getId() == id) found = window;
		else tmpList.push_back(window);
	}

	currentStateEntry_->windows_.swap(tmpList);
	if (found) 
	{
		currentStateEntry_->windows_.push_back(found);
	}

	return (found != 0);
}

bool WindowManager::windowVisible(unsigned id)
{
	std::map<unsigned, bool>::iterator itor =
		windowVisibility_.find(id);
	if (itor != windowVisibility_.end())
	{
		return ((*itor).second);
	}

	return false;
}

bool WindowManager::hideWindow(unsigned id)
{
	std::map<unsigned, bool>::iterator itor =
		windowVisibility_.find(id);
	if (itor != windowVisibility_.end())
	{
		if ((*itor).second)
		{
			(*itor).second = false;
			return true;
		}
	}

	return false;
}

bool WindowManager::windowInCurrentState(unsigned id)
{
	if (!currentStateEntry_) return false;

	std::deque<GLWWindow *>::iterator itor;
	for (itor = currentStateEntry_->windows_.begin();
		itor != currentStateEntry_->windows_.end();
		itor++)
	{
		GLWWindow *window = (*itor);
		if (window->getId() == id)
		{
			return true;
		}
	}
	return false;
}

void WindowManager::draw(const unsigned state)
{
	if (currentStateEntry_->state_ != state) setCurrentEntry(state);

	std::deque<GLWWindow *>::iterator itor;
	for (itor = currentStateEntry_->windows_.begin();
		itor != currentStateEntry_->windows_.end();
		itor++)
	{
		GLWWindow *window = (*itor);
		if (windowVisible(window->getId()))
		{
			window->draw();
		}
	}
}

void WindowManager::simulate(const unsigned state, float simTime)
{
	if (currentStateEntry_->state_ != state) setCurrentEntry(state);

	std::deque<GLWWindow *>::iterator itor;
	for (itor = currentStateEntry_->windows_.begin();
		itor != currentStateEntry_->windows_.end();
		itor++)
	{
		GLWWindow *window = (*itor);
		window->simulate(simTime);
	}	
}

void WindowManager::keyboardCheck(const unsigned state, float frameTime, 
							   char *buffer, unsigned int keyState,
							   KeyboardHistory::HistoryElement *history, int hisCount, 
							   bool &skipRest)
{
	std::deque<GLWWindow *>::reverse_iterator itor;
	for (itor = currentStateEntry_->windows_.rbegin();
		itor != currentStateEntry_->windows_.rend();
		itor++)
	{
		GLWWindow *window = (*itor);
		if (windowVisible(window->getId()))
		{
			window->keyDown(buffer, keyState,
								history, hisCount, 
								skipRest);
			if (skipRest) break;
		}
	}

	if (skipRest) return;

	for (int i=0; i<hisCount; i++)
	{
		char c = history[i].representedKey;
		DWORD dik = history[i].sdlKey;

		std::list<std::pair<KeyboardKey *, GLWWindow *> >::iterator keyItor;
		std::list<std::pair<KeyboardKey *, GLWWindow *> >::iterator endKeyItor = 
			currentStateEntry_->windowKeys_.end();
		for (keyItor = currentStateEntry_->windowKeys_.begin();
			keyItor != endKeyItor;
			keyItor++)
		{
			KeyboardKey *key = (*keyItor).first;
			if (key && key->keyDown(buffer, keyState))
			{
				GLWWindow *window = (*keyItor).second;
				if (windowVisible(window->getId()))
				{
					hideWindow(window->getId());
				}
				else
				{
					showWindow(window->getId());
				}
			}
		}
	}
}

void WindowManager::mouseDown(const unsigned state, GameState::MouseButton button, int x, int y, bool &skipRest)
{
	std::deque<GLWWindow *>::reverse_iterator itor;
	for (itor = currentStateEntry_->windows_.rbegin();
		itor != currentStateEntry_->windows_.rend();
		itor++)
	{
		GLWWindow *window = (*itor);
		if (windowVisible(window->getId()))
		{
			window->mouseDown((float) x, (float) y, skipRest);
			if (skipRest) break;
		}
	}
}

void WindowManager::mouseUp(const unsigned state, GameState::MouseButton button, int x, int y, bool &skipRest)
{
	std::deque<GLWWindow *>::reverse_iterator itor;
	for (itor = currentStateEntry_->windows_.rbegin();
		itor != currentStateEntry_->windows_.rend();
		itor++)
	{
		GLWWindow *window = (*itor);
		if (windowVisible(window->getId()))
		{
			window->mouseUp((float) x, (float) y, skipRest);
			if (skipRest) break;
		}
	}
}

void WindowManager::mouseDrag(const unsigned state, GameState::MouseButton button, 
							  int mx, int my, int x, int y, bool &skipRest)
{
	std::deque<GLWWindow *>::reverse_iterator itor;
	for (itor = currentStateEntry_->windows_.rbegin();
		itor != currentStateEntry_->windows_.rend();
		itor++)
	{
		GLWWindow *window = (*itor);
		if (windowVisible(window->getId()))
		{
			window->mouseDrag((float) mx, (float) my, (float) x, (float) y, skipRest);
			if (skipRest) break;
		}
	}
}

void WindowManager::getMenuItems(const char* menuName, 
								 std::list<GLMenuItem> &items)
{
	if (currentStateEntry_) 
	{
		std::deque<GLWWindow *>::reverse_iterator itor;
		for (itor = currentStateEntry_->windows_.rbegin();
			itor != currentStateEntry_->windows_.rend();
			itor++)
		{
			GLWWindow *window = (*itor);

			if (window->getName()[0] != '\0')
			{
				static char buffer[128];
				sprintf(buffer, "%c %s",
					(windowVisible(window->getId())?'x':' '),
					window->getName());
				items.push_back(buffer);
			}
		}
	}
}

void WindowManager::menuSelection(const char* menuName, const int position, const char *menuItem)
{
	std::list<std::string> items;
	if (currentStateEntry_) 
	{
		int pos = 0;
		std::deque<GLWWindow *>::reverse_iterator itor;
		for (itor = currentStateEntry_->windows_.rbegin();
			itor != currentStateEntry_->windows_.rend();
			itor++)
		{
			GLWWindow *window = (*itor);

			if (window->getName()[0] != '\0')
			{
				if (pos++ == position) 
				{
					if (windowVisible(window->getId()))
					{
						hideWindow(window->getId());
					}
					else
					{
						showWindow(window->getId());
					}
					moveToFront(
						MainMenuDialog::instance()->getId());
					return;
				}
			}
		}
	}
}

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

#include <GLW/GLWWindowManager.h>
#include <dialogs/MainMenuDialog.h>
#include <limits.h>
#include <set>

GLWWindowManager *GLWWindowManager::instance_ = 0;

GLWWindowManager *GLWWindowManager::instance()
{
	if (!instance_)
	{
		instance_ = new GLWWindowManager;
	}

	return instance_;
}

GLWWindowManager::GLWWindowManager() : currentStateEntry_(0)
{
	setCurrentEntry(UINT_MAX);
	MainMenuDialog::instance()->
		addMenu("Windows", 90.0f, 0, this, 0, this);
}

GLWWindowManager::~GLWWindowManager()
{

}

void GLWWindowManager::clear()
{
	currentStateEntry_ = 0;
	setCurrentEntry(UINT_MAX);
	stateEntrys_.clear();
	idToWindow_.clear();
	windowVisibility_.clear();
}

void GLWWindowManager::setCurrentEntry(const unsigned state)
{
	// Store the currently visible windows
	std::set<unsigned int> shownWindows;
    std::deque<GLWWindow *> windowsCopy;
	std::deque<GLWWindow *>::iterator qitor;

	if (currentStateEntry_)
	{
		windowsCopy = currentStateEntry_->windows_;
		for (qitor = windowsCopy.begin();
			qitor != windowsCopy.end();
			qitor++)
		{
			GLWWindow *window = *qitor;
			if (windowVisible(window->getId()))
			{
				shownWindows.insert(window->getId());
			}
		}
	}

	// Find the next state
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
	}

	// Tell the windows that they have been hidden or shown
	windowsCopy = currentStateEntry_->windows_;
	for (qitor = windowsCopy.begin();
		qitor != windowsCopy.end();
		qitor++)
	{
		GLWWindow *window = *qitor;
		window->windowInit(state);

		// This window should be visible
		if (windowVisible(window->getId()))
		{
			// Only call window display on windows that have not
			// been shown in the last state
			std::set<unsigned int>::iterator shownPrev = 
				shownWindows.find(window->getId());
			if (shownPrev == shownWindows.end())
			{
				window->windowDisplay();
			}
		}
		else
		{
			// Only call window hide on windows that have
			// been shown in the last state
			std::set<unsigned int>::iterator shownPrev = 
				shownWindows.find(window->getId());
			if (shownPrev != shownWindows.end())
			{
				window->windowHide();
			}
		}
	}

	sortWindowLevels();
}

void GLWWindowManager::addWindow(const unsigned state, GLWWindow *window, KeyboardKey *key, bool visible)
{
	windowVisibility_[window->getId()] = visible;
	idToWindow_[window->getId()] = window;

	stateEntrys_[state].windows_.push_back(window);
	stateEntrys_[state].state_ = state;
	stateEntrys_[state].windowKeys_.push_back(
		std::pair<KeyboardKey *, GLWWindow *>(key, window));
}

bool GLWWindowManager::showWindow(unsigned id)
{
	std::map<unsigned, bool>::iterator itor =
		windowVisibility_.find(id);
	if (itor != windowVisibility_.end())
	{
		if (!(*itor).second)
		{
			GLWWindow *window = idToWindow_[id];
			(*itor).second = true;
			window->windowDisplay();
			moveToFront(id);

			return true;
		}
	}

	return false;
}

bool GLWWindowManager::moveToFront(unsigned id)
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

	currentStateEntry_->windows_ = tmpList;
	if (found) 
	{
		currentStateEntry_->windows_.push_back(found);
		sortWindowLevels();
	}

	return (found != 0);
}

void GLWWindowManager::sortWindowLevels()
{
	if (!currentStateEntry_) return;

	std::deque<GLWWindow *> &windows = currentStateEntry_->windows_;

	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0; i<int(windows.size())-1; i++)
		{
			GLWWindow *first = windows[i];
			GLWWindow *second = windows[i+1];
			if (first->getWindowLevel() < second->getWindowLevel())
			{
				windows[i] = second;
				windows[i+1] = first;
				changed = true;
			}
		}
	}
}

bool GLWWindowManager::windowVisible(unsigned id)
{
	std::map<unsigned, bool>::iterator itor =
		windowVisibility_.find(id);
	if (itor != windowVisibility_.end())
	{
		return ((*itor).second);
	}

	return false;
}

bool GLWWindowManager::hideWindow(unsigned id)
{
	std::map<unsigned, bool>::iterator itor =
		windowVisibility_.find(id);
	if (itor != windowVisibility_.end())
	{
		if ((*itor).second)
		{
			GLWWindow *window = idToWindow_[id];
			window->windowHide();

			(*itor).second = false;
			return true;
		}
	}

	return false;
}

bool GLWWindowManager::windowInCurrentState(unsigned id)
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

void GLWWindowManager::draw(const unsigned state)
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

unsigned int GLWWindowManager::getFocus(int x, int y)
{
	std::deque<GLWWindow *>::reverse_iterator itor;
	for (itor = currentStateEntry_->windows_.rbegin();
		itor != currentStateEntry_->windows_.rend();
		itor++)
	{
		GLWWindow *window = (*itor);
		if (windowVisible(window->getId()))
		{
			if (GLWidget::inBox(
				(float) x, (float) y,
				window->getX(), window->getY(),
				window->getW(), window->getH()))
			{
				return window->getId();
			}
		}
	}
	return 0;
}

void GLWWindowManager::simulate(const unsigned state, float simTime)
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
			window->simulate(simTime);
		}
	}	
}

void GLWWindowManager::keyboardCheck(const unsigned state, float frameTime, 
							   char *buffer, unsigned int keyState,
							   KeyboardHistory::HistoryElement *history, int hisCount, 
							   bool &skipRest)
{
	if (currentStateEntry_->state_ != state) setCurrentEntry(state);

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

void GLWWindowManager::mouseDown(const unsigned state, GameState::MouseButton button, int x, int y, bool &skipRest)
{
	if (currentStateEntry_->state_ != state) setCurrentEntry(state);

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

void GLWWindowManager::mouseUp(const unsigned state, GameState::MouseButton button, int x, int y, bool &skipRest)
{
	if (currentStateEntry_->state_ != state) setCurrentEntry(state);

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

void GLWWindowManager::mouseDrag(const unsigned state, GameState::MouseButton button, 
							  int mx, int my, int x, int y, bool &skipRest)
{
	if (currentStateEntry_->state_ != state) setCurrentEntry(state);

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

void GLWWindowManager::getMenuItems(const char* menuName, 
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
				items.push_back(
					GLMenuItem(
						window->getName(), 
						&window->getToolTip(), 
						windowVisible(window->getId())));
			}
		}
	}
}

void GLWWindowManager::menuSelection(const char* menuName, const int position, const char *menuItem)
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
					return;
				}
			}
		}
	}
}

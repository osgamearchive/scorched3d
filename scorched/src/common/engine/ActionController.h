////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2009
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

#if !defined(AFX_ACTIONCONTROLLER_H__86995B4A_478E_4CFE_BD4C_79128DE51904__INCLUDED_)
#define AFX_ACTIONCONTROLLER_H__86995B4A_478E_4CFE_BD4C_79128DE51904__INCLUDED_

#include <set>
#include <list>
#include <map>
#include <vector>
#include <string.h>
#include <actions/Action.h>

class ScorchedContext;
class ActionController
{
public:
	ActionController();
	virtual ~ActionController();

	// Add an action to be simulated
	void addAction(Action *action);
	bool noReferencedActions();
	void clear(bool warn = false);

	// Turn on action tracing
	void startActionProfiling() { actionProfiling_ = true; }
	void stopActionProfiling();
	void logActions();

	// Set the simulation speed
	void setScorchedContext(ScorchedContext *context);

	// Inherited from GameStateI
	void simulate(fixed frameTime, fixed time);
	void draw();

protected:
	class ActionList
	{
	public:
		ActionList() : actionCount(0), maxActions(1000)
		{
			actions = new Action*[maxActions];
		}

		void push_back(Action *action)
		{
			actions[actionCount++] = action;
			if (actionCount == maxActions)
			{
				Action **newActions = new Action*[maxActions * 2];
				memcpy(newActions, actions, sizeof(Action *) * maxActions);
				delete [] actions;
				maxActions = maxActions * 2;
				actions = newActions;
			}
		}
		void clear()
		{
			actionCount = 0;
		}

		int actionCount;
		Action **actions;
	private:
		int maxActions;
	};

	ScorchedContext *context_;
	std::list<Action *> newActions_;
	ActionList actions_;
	std::map<std::string, int> actionProfile_;
	int referenceCount_;
	bool actionProfiling_;

	bool allEvents();
	void addNewActions(fixed time);

};

#endif // !defined(AFX_ACTIONCONTROLLER_H__86995B4A_478E_4CFE_BD4C_79128DE51904__INCLUDED_)

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

#include <engine/GameState.h>
#include <engine/GameStateI.h>
#include <engine/GameStateStimulusI.h>
#include <common/Keyboard.h>
#include <common/Defines.h>

GameState *GameState::instance_ = 0;

GameState *GameState::instance()
{
	if (!instance_)
	{
		instance_ = new GameState;
	}

	return instance_;
}

GameState::GameState() :
	fakeMiddleButton_(true),
	currentMouseState_(0),
	pendingStimulus_(-1),
	currentState_(-1),
	currentEntry_(0),
	currentMouseX_(0),
	currentMouseY_(0)
{
	
}

GameState::~GameState()
{

}

void GameState::setFakeMiddleButton(bool fake)
{
	fakeMiddleButton_ = fake;
}

void GameState::mouseWheel(short z)
{
	bool skipRest = false;
	if (currentEntry_)
	{
		GameStateEntry *thisEntry = currentEntry_;
		unsigned thisState = currentState_;

		StateIList *currentList = &currentEntry_->subMouseWheelList;
		if (!currentList->empty())
		{
			StateIList::iterator subItor;
			for (subItor = currentList->begin();
				subItor != currentList->end();
				subItor++)
			{
				(*subItor)->mouseWheel(thisState, z, skipRest);
				if (skipRest) break;
			}
		}
	}
}

void GameState::mouseMove(int x, int y)
{
	currentMouseX_ = x;
	currentMouseY_ = y;
	if (currentEntry_)
	{
		GameStateEntry *thisEntry = currentEntry_;
		unsigned thisState = currentState_;

		if (MouseButtonMiddle & currentMouseState_)
		{
			int diffX = x - mouseMDragX_; mouseMDragX_ = x;
			int diffY = y - mouseMDragY_; mouseMDragY_ = y;

			mouseMoveCall(thisState, MouseButtonMiddle, 
						  currentEntry_->subMouseDragMiddleList,
					x, y, diffX, diffY);
		}

		if (fakeMiddleButton_ && (MouseButtonLeft & currentMouseState_ && 
								  MouseButtonRight & currentMouseState_))
		{
			int diffX = x - mouseMDragX_; mouseMDragX_ = x;
			int diffY = y - mouseMDragY_; mouseMDragY_ = y;

			mouseMoveCall(thisState, MouseButtonMiddle, 
						  currentEntry_->subMouseDragMiddleList,
					x, y, diffX, diffY);
		}
		else
		{
			if (MouseButtonLeft & currentMouseState_)
			{
				int diffX = x - mouseLDragX_; mouseLDragX_ = x;
				int diffY = y - mouseLDragY_; mouseLDragY_ = y;

				mouseMoveCall(thisState, MouseButtonLeft, 
							  currentEntry_->subMouseDragLeftList,
						x, y, diffX, diffY);
			}
			if (MouseButtonRight & currentMouseState_)
			{
				int diffX = x - mouseRDragX_; mouseRDragX_ = x;
				int diffY = y - mouseRDragY_; mouseRDragY_ = y;

				mouseMoveCall(thisState, MouseButtonRight, 
							  currentEntry_->subMouseDragRightList,
						x, y, diffX, diffY);
			}
		}
	}
}

void GameState::mouseMoveCall(const unsigned state, MouseButton button, 
							  StateIList &currentList, 
							  int mx, int my,
							  int dx, int dy)
{
	if (!currentList.empty())
	{
		bool skipRest = false;
		StateIList::iterator subItor;
		for (subItor = currentList.begin();
			subItor != currentList.end();
			subItor++)
		{
			(*subItor)->mouseDrag(state, button, mx, my, dx, dy, skipRest);
			if (skipRest) break;
		}
	}
}

void GameState::mouseDown(MouseButton button, int x, int y)
{
	mouseUpDown(button, true, x, y);
}

void GameState::mouseUp(MouseButton button, int x, int y)
{
	mouseUpDown(button, false, x, y);
}

void GameState::mouseUpDown(MouseButton button, bool down, int x, int y)
{
	bool skipRest = false;
	if (currentEntry_)
	{
		GameStateEntry *thisEntry = currentEntry_;
		unsigned thisState = currentState_;

		StateIList *currentList = 0;
		if (down)
		{
			currentMouseState_ |= (unsigned) button;
			switch(button)
			{
			case MouseButtonRight:
				mouseRDragX_ = x; mouseRDragY_ = y;
				currentList = &currentEntry_->subMouseDownRightList;
				break;
			case MouseButtonMiddle:
				mouseMDragX_ = x; mouseMDragY_ = y;
				currentList = &currentEntry_->subMouseDownMiddleList;
				break;
			default:
				mouseLDragX_ = x; mouseLDragY_ = y;
				currentList = &currentEntry_->subMouseDownLeftList;
				break;
			}
		}
		else
		{
			currentMouseState_ ^= (unsigned) button;
			switch(button)
			{
			case MouseButtonRight:
				currentList = &currentEntry_->subMouseUpRightList;
				break;
			case MouseButtonMiddle:
				currentList = &currentEntry_->subMouseUpMiddleList;
				break;
			default:
				currentList = &currentEntry_->subMouseUpLeftList;
				break;
			}
		}

		if (!currentList->empty())
		{
			StateIList::iterator subItor;
			for (subItor = currentList->begin();
				subItor != currentList->end();
				subItor++)
			{
				if (down) 
				{
					(*subItor)->mouseDown(thisState, button, x, y, skipRest);
				}
				else 
				{
					(*subItor)->mouseUp(thisState, button, x, y, skipRest);
				}
				if (skipRest) break;
			}
		}
	}
}

void GameState::simulate(float simTime)
{
	if (checkStimulate()) return;
	if (currentEntry_)
	{
		GameStateEntry *thisEntry = currentEntry_;
		unsigned thisState = currentState_;

		std::list<GameStateSubEntry>::iterator itor;
		for (itor = thisEntry->loopList.begin();
			itor != thisEntry->loopList.end();
			itor++)
		{
			itor->current->simulate(thisState, simTime);
			if (checkStimulate()) return;

			StateIList::iterator subItor;
			for (subItor = itor->subLoopList.begin();
				subItor != itor->subLoopList.end();
				subItor++)
			{
				(*subItor)->simulate(thisState, simTime);
				if (checkStimulate()) return;
			}
		}

		if (!thisEntry->subKeyList.empty())
		{
			unsigned int historySize;
			unsigned int bufferSize;

			KeyboardHistory::HistoryElement *history = 
				Keyboard::instance()->getkeyboardhistory(historySize);
			char *buffer  = 
				Keyboard::instance()->getkeyboardbuffer(bufferSize);
			unsigned int keyState = 
				Keyboard::instance()->getKeyboardState();

			bool skipRest = false;
			StateIList::iterator subItor;
			for (subItor = thisEntry->subKeyList.begin();
				subItor != thisEntry->subKeyList.end();
				subItor++)
			{
				(*subItor)->keyboardCheck(thisState, simTime, buffer, keyState, 
					history, historySize, skipRest);
				if (checkStimulate()) return;
				if (skipRest) break;
			}
		}

		if (!thisEntry->condStimList.empty())
		{
			StiulusIList::iterator itor;
			for (itor = thisEntry->condStimList.begin();
				itor != thisEntry->condStimList.end();
				itor++)
			{
				SimulusIPair &p = *itor;
				if (p.first->acceptStateChange(thisState, p.second, simTime))
				{
					setState(itor->second);
					return;
				}
			}
		}
	}
}

void GameState::draw()
{
	if (currentEntry_)
	{
		GameStateEntry *thisEntry = currentEntry_;
		unsigned thisState = currentState_;

		std::list<GameStateSubEntry>::iterator itor;
		for (itor = thisEntry->loopList.begin();
			itor != thisEntry->loopList.end();
			itor++)
		{
			itor->current->draw(thisState);

			StateIList::iterator subItor;
			for (subItor = itor->subLoopList.begin();
				subItor != itor->subLoopList.end();
				subItor++)
			{
				(*subItor)->draw(thisState);
			}
		}
	}
}

void GameState::setState(const unsigned state)
{
	currentState_ = state;
	currentEntry_ = 0;
	std::map<unsigned, GameStateEntry>::iterator itor = stateList_.find(state);
	if (itor != stateList_.end())
	{
		currentEntry_ = &itor->second;

		GameStateEntry *thisEntry = currentEntry_;
		unsigned thisState = currentState_;

		if (!thisEntry->enterStateList.empty())
		{
			StateIList::iterator subItor;
			for (subItor = thisEntry->enterStateList.begin();
				subItor != thisEntry->enterStateList.end();
				subItor++)
			{
				GameStateI *s = (*subItor);
				s->enterState(thisState);
			}
		}

		if (checkStimulate()) return;
	}
	else
	{
		DIALOG_ASSERT(0);
	}
}

bool GameState::checkStimulate()
{
	if (pendingStimulus_ != ((unsigned int) -1))
	{
		std::map<unsigned, unsigned>::iterator itor = 
			currentEntry_->stimList.find(pendingStimulus_);
		if (itor != currentEntry_->stimList.end())
		{
			pendingStimulus_ = -1;
			setState(itor->second);
			return true;
		}
		else
		{
			dialogMessage("Scorched3D", 
				"Failed to find stimulus %i in state %i", 
				pendingStimulus_, currentState_);
			exit(1);
		}
	}

	return false;
}

void GameState::stimulate(const unsigned stimulus)
{
	pendingStimulus_ = stimulus;
}

GameState::GameStateEntry* GameState::getEntry(const unsigned state)
{
	GameStateEntry *foundEntry = 0;
	std::map<unsigned, GameStateEntry>::iterator itor = stateList_.find(state);
	if (itor == stateList_.end())
	{
		GameStateEntry newEntry;
		stateList_[state] = newEntry;

		foundEntry = &stateList_[state];

		if (currentState_ == state)
		{
			currentEntry_ = &stateList_[state];
		}
	}
	else
	{
		foundEntry = &itor->second;
	}

	return foundEntry;
}

GameState::GameStateSubEntry* GameState::getSubEntry(const unsigned state, 
													 GameStateI *entry)
{
	GameStateEntry *foundEntry = getEntry(state);

	GameStateSubEntry *foundSubEntry = 0;
	std::list<GameStateSubEntry>::iterator subItor;
	for (subItor = foundEntry->loopList.begin();
		subItor != foundEntry->loopList.end();
		subItor++)
	{
		if (subItor->current == entry)
		{
			foundSubEntry = &(*subItor);
		}
	}

	if (!foundSubEntry)
	{
		GameStateSubEntry newEntry;
		newEntry.current = entry;

		foundEntry->loopList.push_back(newEntry);
		foundSubEntry = &foundEntry->loopList.back();
	}

	return foundSubEntry;
}

void GameState::addStateLoop(const unsigned state, GameStateI *entry, 
							 GameStateI *subEntry)
{
	getSubEntry(state, entry)->subLoopList.push_back(subEntry);
}

void GameState::addStateEntry(const unsigned state, GameStateI *subEntry)
{
	getEntry(state)->enterStateList.push_back(subEntry);
}

void GameState::addStateKeyEntry(const unsigned state, GameStateI *subEntry)
{
	getEntry(state)->subKeyList.push_back(subEntry);
}

void GameState::addStateMouseDownEntry(const unsigned state, 
									   const unsigned buttons, 
									   GameStateI *subEntry)
{
	if (buttons & MouseButtonLeft)
	{
		getEntry(state)->subMouseDownLeftList.push_back(subEntry);
	}
	if (buttons & MouseButtonRight)
	{
		getEntry(state)->subMouseDownRightList.push_back(subEntry);
	}
	if (buttons & MouseButtonMiddle)
	{
		getEntry(state)->subMouseDownMiddleList.push_back(subEntry);
	}
}

void GameState::addStateMouseUpEntry(const unsigned state, 
									 const unsigned buttons, 
									 GameStateI *subEntry)
{
	if (buttons & MouseButtonLeft)
	{
		getEntry(state)->subMouseUpLeftList.push_back(subEntry);
	}
	if (buttons & MouseButtonRight)
	{
		getEntry(state)->subMouseUpRightList.push_back(subEntry);
	}
	if (buttons & MouseButtonMiddle)
	{
		getEntry(state)->subMouseUpMiddleList.push_back(subEntry);
	}
}

void GameState::addStateMouseDragEntry(const unsigned state, 
									   const unsigned buttons, 
									   GameStateI *subEntry)
{
	if (buttons & MouseButtonLeft)
	{
		getEntry(state)->subMouseDragLeftList.push_back(subEntry);
	}
	if (buttons & MouseButtonRight)
	{
		getEntry(state)->subMouseDragRightList.push_back(subEntry);
	}
	if (buttons & MouseButtonMiddle)
	{
		getEntry(state)->subMouseDragMiddleList.push_back(subEntry);
	}
}

void GameState::addStateMouseWheelEntry(const unsigned state, 
										GameStateI *subEntry)
{
	getEntry(state)->subMouseWheelList.push_back(subEntry);
}

void GameState::addStateStimulus(const unsigned state, 
								 const unsigned stim, 
								 const unsigned nexts)
{
	GameState::GameStateEntry *entry = getEntry(state);
	std::map<unsigned, unsigned>::iterator itor = entry->stimList.find(stim);
	if (itor == entry->stimList.end())
	{
		entry->stimList[stim] = nexts;
	}
	else
	{
		DIALOG_ASSERT(0);
	}
}

void GameState::addStateStimulus(const unsigned state, 
								 GameStateStimulusI *check, 
								 const unsigned nexts)
{
	GameState::GameStateEntry *entry = getEntry(state);
	SimulusIPair pair(check, nexts);
	entry->condStimList.push_back(pair);
}

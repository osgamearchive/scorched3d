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

#include <common/KeyboardKey.h>
#include <common/KeyTranslate.h>
#include <common/KeyStateTranslate.h>
#include <common/Defines.h>

KeyboardKey::KeyboardKey(const char *name,
						 const char *description) :
	name_(name), description_(description), 
	noKeys_(0), keys_(0), keyStates_(0), keyToogle_(false)
{

}

KeyboardKey::~KeyboardKey()
{
	delete [] keys_;
	delete [] keyStates_;
}

bool KeyboardKey::addKeys(std::list<std::string> &keyNames,
						  std::list<std::string> &keyStates)
{
	DIALOG_ASSERT(keyNames.size() == keyStates.size());

	keys_ = new unsigned int[keyNames.size()];
	keyStates_ = new unsigned int[keyStates.size()];

	int i=0;
	std::list<std::string>::iterator itor;
	std::list<std::string>::iterator itorState;
	for (itor = keyNames.begin(), itorState = keyStates.begin();
		 itor != keyNames.end() && itorState != keyStates.end();
		 itor++, i++, itorState++)
	{
		std::string &name = *itor;
		if (!translateKeyName(name.c_str(), keys_[i])) 
		{
			dialogMessage("KeyboardKey",
						  "Failed to find key names \"%s\" for \"%s\"",
						  name.c_str(),
						  name_.c_str());
			return false;
		}

		std::string &state = *itorState;
		if (!translateKeyState(state.c_str(), keyStates_[i])) 
		{
			dialogMessage("KeyboardKey",
						  "Failed to find key state \"%s\" for \"%s\"",
						  state.c_str(),
						  name_.c_str());
			return false;
		}
		noKeys_ ++;
	}

	return true;
}

bool KeyboardKey::translateKeyName(const char *name, unsigned int &key)
{
	for (int i=0; i<sizeof(KeyTranslationTable)/sizeof(KeyTranslation); i++)
	{
		if (strcmp(KeyTranslationTable[i].keyName, name)==0)
		{
			key = KeyTranslationTable[i].keySym;
			return true;
		}
	}

	return false;
}

bool KeyboardKey::translateKeyState(const char *name, unsigned int &state)
{
	for (int i=0; i<sizeof(KeyStateTranslationTable)/sizeof(KeyStateTranslation); i++)
	{
		if (strcmp(KeyStateTranslationTable[i].keyStateName, name)==0)
		{
			state = KeyStateTranslationTable[i].keyStateSym;
			return true;
		}
	}

	return false;
}

bool KeyboardKey::keyDown(char *buffer, unsigned int keyState, bool repeat)
{
	for (unsigned int i=0; i<noKeys_; i++)
	{
		if (keyState == keyStates_[i] && buffer[keys_[i]])
		{
			if (!repeat && keyToogle_) return false;

			keyToogle_ = true;
			return true;
		}
	}

	keyToogle_ = false;
	return false;
}

bool KeyboardKey::keyMatch(unsigned key)
{
	for (unsigned int i=0; i<noKeys_; i++)
	{
		if (keys_[i] == key) return true;
	}
	return false;
}

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

#include <common/Keyboard.h>
#include <common/Defines.h>
#include <XML/XMLFile.h>

bool Keyboard::dvorak_ = false;

Keyboard *Keyboard::instance_ = 0;

Keyboard *Keyboard::instance()
{
	if (!instance_)
	{
		instance_ = new Keyboard;
	}

	return instance_;
}

Keyboard::Keyboard() : keybHistCnt_(0)
{

}

Keyboard::~Keyboard()
{

}

bool Keyboard::init()
{
 	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(250, 100);
	
	return true;
}

unsigned int Keyboard::getKeyboardState()
{
	// Ingore capslock, numlock and scroll lock
	const unsigned int keymask = KMOD_LSHIFT | KMOD_RSHIFT | KMOD_LCTRL | KMOD_RCTRL |
		KMOD_LALT | KMOD_RALT | KMOD_LMETA | KMOD_RMETA;

	return SDL_GetModState() & keymask;
}

char *Keyboard::getkeyboardbuffer(unsigned int &bufCnt)
{
 	bufCnt = SDLK_LAST;
 	return (char *) SDL_GetKeyState(NULL);
}
 
KeyboardHistory::HistoryElement *Keyboard::getkeyboardhistory(unsigned int &histCnt)
{
 	histCnt = keybHistCnt_;
 	keybHistCnt_=0;

 	return keybHist_;
}
 
void Keyboard::processKeyboardEvent(SDL_Event &event)
{
	// Check we have enough space in the array
	if (keybHistCnt_ >= MAX_KEYBDHIST) return;

	// Check we are adding the correct key type
	if (event.type != SDL_KEYDOWN) return;

	// Check we don't have an international character
	if ( (event.key.keysym.unicode & 0xFF80) == 0 )
	{
		char ch = event.key.keysym.unicode & 0x7F;

		KeyboardHistory::HistoryElement newElement =
		{
			event.key.keysym.sym,
			ch
		};

		// Add characater and symbol to history
		keybHist_[keybHistCnt_++] = newElement;
	}
}

bool &Keyboard::getDvorak()
{
	return dvorak_;
}

void Keyboard::clear()
{
	std::map<std::string, KeyboardKey *, std::less<std::string> >::iterator itor;
	for (itor = keyMap_.begin();
		itor != keyMap_.end();
		itor++)
	{
		delete (*itor).second;
	}
	keyMap_.clear();
}

bool Keyboard::parseKeyFile(const char *fileName)
{
	clear();

	// Load key definition file
	XMLFile file;
    if (!file.readFile(fileName))
	{
		dialogMessage("Keyboard", 
					  "Failed to parse \"%s\"\n%s", 
					  fileName,
					  file.getParserError());
		return false;
	}

	// Check file exists
	if (!file.getRootNode())
	{
		dialogMessage("Keyboard",
					  "Failed to find key definition file \"%s\"",
					  fileName);
		return false;		
	}

	std::map<std::string, KeyboardKey *, std::less<std::string> > usedKeyMap_;

	// Itterate all of the keys in the file
    std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = file.getRootNode()->getChildren();
    for (childrenItor = children.begin();
		 childrenItor != children.end();
		 childrenItor++)
    {
		// Parse the tank entry
        XMLNode *currentNode = (*childrenItor);
		if (strcmp(currentNode->getName(), "keyentry"))
		{
			dialogMessage("Keyboard",
						  "Failed to find keyentry node");
			return false;
		}

		// Get the name of the key
		const char *keyName = 0;
		bool command = false;
		XMLNode *nameNode = currentNode->getNamedChild("name");
		XMLNode *commandNode = currentNode->getNamedChild("command");
		if (nameNode)
		{
			keyName = nameNode->getContent();
			command = false;
		}
		else if (commandNode)
		{
			keyName = commandNode->getContent();
			command = true;
		}
		else
		{
			dialogMessage("Keyboard",
						  "Failed to find name node");
			return false;
		}
		
		// Get the description for the key
		XMLNode *descNode = currentNode->getNamedChild("description");
		if (!descNode)
		{
			dialogMessage("Keyboard",
						  "Failed to find description node");
			return false;
		}
		const char *keyDesc = descNode->getContent();

		// Create the key
		KeyboardKey *newKey = new KeyboardKey(keyName, keyDesc, command);

		// Add all the key names
		std::list<std::string> keyNames, keyStateNames;
		std::list<XMLNode *>::iterator keyItor;
		std::list<XMLNode *> &keys = currentNode->getChildren();
		for (keyItor = keys.begin();
			 keyItor != keys.end();
			 keyItor++)
		{
			XMLNode *currentKey = (*keyItor);
			if (strcmp(currentKey->getName(), "key")==0)
			{
				const char *state = "NONE";
				XMLNode *stateNode = currentKey->getNamedParameter("state");
				if (stateNode) state = stateNode->getContent();

				// Add a key and state
				keyNames.push_back(currentKey->getContent());
				keyStateNames.push_back(state);

				// Check the key is not already in use
				std::string wholeKey;
				wholeKey += state;
				wholeKey += ":";
				wholeKey += currentKey->getContent();
				std::map<std::string, KeyboardKey *, std::less<std::string> >::iterator useditor =
					usedKeyMap_.find(wholeKey);
				if (useditor != usedKeyMap_.end())
				{
					KeyboardKey *usedKey = (*useditor).second;
					dialogMessage("Keyboard",
								  "Key \"%s\" and state \"%s\" defined for \"%s\" was already defined for \"%s\"",
								  currentKey->getContent(), state, keyName, usedKey->getName());
					return false;					
				}
				usedKeyMap_[wholeKey] = newKey;
			}
		}

		// Actually add the key
		if (!newKey->addKeys(keyNames, keyStateNames)) return false;

		
		if (command)
		{
			// Add to the list of commands
			commandKeys_.push_back(newKey);
		}
		else
		{
			// Add to the list of pre-defined keys
			keyMap_[keyName] = newKey;
		}
	}

	return true;
}

std::list<KeyboardKey *> &Keyboard::getCommandKeys()
{
	return commandKeys_;
}

KeyboardKey *Keyboard::getKey(const char *name)
{
	static KeyboardKey defaultKey("None", "None", false);

	std::map<std::string, KeyboardKey *, std::less<std::string> >::iterator itor =
		keyMap_.find(name);
	if (itor != keyMap_.end()) return (*itor).second;

	dialogMessage("Keyboard",
				  "Failed to find key for key name \"%s\"",
				  name);	
	return &defaultKey;
}

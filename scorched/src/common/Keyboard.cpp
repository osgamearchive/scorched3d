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
#include <wx/wx.h>
#include <wx/utils.h>

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
	std::map<std::string, KeyboardKey *>::iterator itor;
	for (itor = keyMap_.begin();
		itor != keyMap_.end();
		itor++)
	{
		delete (*itor).second;
	}
	keyMap_.clear();
	keyList_.clear();
	commandKeys_.clear();	
}

bool Keyboard::saveKeyFile()
{
	XMLNode keysNode("keys");
	std::list<KeyboardKey *>::iterator itor;
	for (itor = keyList_.begin();
		itor != keyList_.end();
		itor++)
	{
		KeyboardKey *key = (*itor);
		XMLNode *keyNode = new XMLNode("keyentry");
		if (key->getNameIsCommand())
			keyNode->addChild(new XMLNode("command", key->getName()));
		else keyNode->addChild(new XMLNode("name", key->getName()));
		keyNode->addChild(new XMLNode("title", key->getTitle()));
		keyNode->addChild(new XMLNode("description", key->getDescription()));
		keyNode->addChild(new XMLNode("group", formatString("%i", key->getGroup())));

		std::vector<KeyboardKey::KeyEntry> &keys = key->getKeys();
		std::vector<KeyboardKey::KeyEntry>::iterator subitor;
		for (subitor = keys.begin();
			subitor != keys.end();
			subitor++)
		{
			KeyboardKey::KeyEntry &subentry = (*subitor);
			const char *name = "";
			const char *state = "";
			KeyboardKey::translateKeyNameValue(subentry.key, name);
			KeyboardKey::translateKeyStateValue(subentry.state, state);
			XMLNode *subnode = new XMLNode("key", name);
			if (strcmp(state, "NONE") != 0) subnode->addParameter(
				new XMLNode("state", state, XMLNode::XMLParameterType));
			keyNode->addChild(subnode);
		}

		keysNode.addChild(keyNode);
	}

	const char *fileName = getSettingsFile("keys.xml");
	if (!keysNode.writeToFile(fileName)) return false;

	return true;
}

bool Keyboard::loadKeyFile(bool loadDefaults)
{
	clear();
	std::map<std::string, KeyboardKey *> usedKeyMap_;

	const char *fileName = getSettingsFile("keys.xml");
	if (s3d_fileExists(fileName) && !loadDefaults)
	{
		if (!loadKeyFile(fileName, usedKeyMap_)) return false;
	}
	if (!loadKeyFile(getDataFile("data/keys.xml"), usedKeyMap_)) return false;
	return true;
}

bool Keyboard::loadKeyFile(const char *fileName,
	std::map<std::string, KeyboardKey *> &usedKeyMap)
{
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
		XMLNode *nameNode;
		XMLNode *commandNode;
		if (currentNode->getNamedChild("name", nameNode, false))
		{
			keyName = nameNode->getContent();
			command = false;
		}
		else if (currentNode->getNamedChild("command", commandNode, false))
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

		// Check to see if we have already loaded this entry
		if (keyMap_.find(keyName) != keyMap_.end()) continue;
		
		// Get the description for the key
		XMLNode *descNode;
		if (!currentNode->getNamedChild("description", descNode)) return false;
		const char *keyDesc = descNode->getContent();

		// Get the group
		int group = 0;
		if (!currentNode->getNamedChild("group", group)) return false;

		// Get the title
		XMLNode *titleNode;
		if (!currentNode->getNamedChild("title", titleNode)) return false;
		const char *keyTitle = titleNode->getContent();

		// Create the key
		KeyboardKey *newKey = new KeyboardKey(keyName, keyTitle, keyDesc, group, command);

		// Add all the key names
		XMLNode *currentKey = 0;
		std::list<std::string> keyNames, keyStateNames;
		while (currentNode->getNamedChild("key", currentKey, false))
		{
				const char *state = "NONE";
				XMLNode *stateNode;
				if (currentKey->getNamedParameter("state", stateNode, false)) 
					state = stateNode->getContent();

				// Add a key and state
				keyNames.push_back(currentKey->getContent());
				keyStateNames.push_back(state);

				// Check the key is not already in use
				std::string wholeKey;
				wholeKey += state;
				wholeKey += ":";
				wholeKey += currentKey->getContent();
				std::map<std::string, KeyboardKey *>::iterator useditor =
					usedKeyMap.find(wholeKey);
				if (useditor != usedKeyMap.end())
				{
					KeyboardKey *usedKey = (*useditor).second;
					dialogMessage("Keyboard",
								  "Key \"%s\" and state \"%s\" defined for \"%s\" was already defined for \"%s\"",
								  currentKey->getContent(), state, keyName, usedKey->getName());
					return false;					
				}
				usedKeyMap[wholeKey] = newKey;
		}

		// Actually add the key
		if (!newKey->addKeys(keyNames, keyStateNames)) return false;
		
		// Add to the list of pre-defined keys
		keyMap_[keyName] = newKey;
		keyList_.push_back(newKey);
		if (command) commandKeys_.push_back(newKey);

		if (!currentNode->failChildren()) return false;
	}

	return true;
}

KeyboardKey *Keyboard::getKey(const char *name)
{
	static KeyboardKey defaultKey("None", "None", "None", 0, false);

	std::map<std::string, KeyboardKey *>::iterator itor =
		keyMap_.find(name);
	if (itor != keyMap_.end()) return (*itor).second;

	dialogMessage("Keyboard",
				  "Failed to find key for key name \"%s\"",
				  name);	
	return &defaultKey;
}


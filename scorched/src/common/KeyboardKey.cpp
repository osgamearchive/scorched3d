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
#include <common/Defines.h>

KeyboardKey::KeyboardKey(const char *name,
						 const char *description) :
	name_(name), description_(description), 
	noKeys_(0), keys_(0), keyToogle_(false)
{

}

KeyboardKey::~KeyboardKey()
{
	delete [] keys_;
}

bool KeyboardKey::addKeys(std::list<std::string> &keyNames)
{
	keys_ = new unsigned[keyNames.size()];

	int i=0;
	std::list<std::string>::iterator itor;
	for (itor = keyNames.begin();
		 itor != keyNames.end();
		 itor++, i++)
	{
		std::string name = *itor;
		unsigned key = translateKeyName(name.c_str());
		if (!key) 
		{
			dialogMessage("KeyboardKey",
						  "Failed to find key names \"%s\" for \"%s\"",
						  name.c_str(),
						  name_.c_str());
			return false;
		}

		keys_[i] = key;
		noKeys_ ++;
	}

	return true;
}

unsigned int KeyboardKey::translateKeyName(const char *name)
{
	for (int i=0; i<sizeof(KeyTranslationTable)/sizeof(KeyTranslation); i++)
	{
		if (strcmp(KeyTranslationTable[i].keyName, name)==0)
			return KeyTranslationTable[i].keySym;
	}

	return 0;
}

bool KeyboardKey::keyDown(char *buffer, bool repeat)
{
	for (unsigned int i=0; i<noKeys_; i++)
	{
		if (buffer[keys_[i]])
		{
			if (!repeat && keyToogle_) return false;

			keyToogle_ = true;
			return true;
		}
	}

	keyToogle_ = false;
	return false;
}

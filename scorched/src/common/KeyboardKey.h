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

#ifndef __keyboardKey_h__
#define __keyboardKey_h__

#include <string>
#include <list>

class KeyboardKey
{
public:
	KeyboardKey(const char *name,
				const char *description);
	virtual ~KeyboardKey();

	bool keyDown(char *buffer, unsigned int keyState, bool repeat = true);
	bool keyMatch(unsigned key);
	bool addKeys(std::list<std::string> &keyNames,
		std::list<std::string> &keyStates);

	const char *getName() { return name_.c_str(); }
	const char *getDescription() { return description_.c_str(); }

	static bool translateKeyName(const char *name, unsigned int &key);
	static bool translateKeyState(const char *name, unsigned int &state);

protected:
	std::string name_;
	std::string description_;
	bool keyToogle_;
	unsigned int noKeys_;
	unsigned int *keys_;
	unsigned int *keyStates_;

private:
	KeyboardKey(const KeyboardKey &);
	const KeyboardKey & operator=(const KeyboardKey &);

};

#endif // __keyboardKey_h__


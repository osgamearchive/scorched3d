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

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <common/KeyboardHistory.h>
#include <common/KeyboardKey.h>
#include <string>
#include <map>

#define MAX_KEYBDHIST 1024
#define KEYBOARDKEY(name, key) static KeyboardKey * key = Keyboard::instance()->getKey( name );

class Keyboard
{
public:
	static Keyboard *instance();

	bool init();

	char *getkeyboardbuffer(unsigned int &bufCnt);
	KeyboardHistory::HistoryElement *getkeyboardhistory(unsigned int &histCnt); 

	void processKeyboardEvent(SDL_Event &event);
	static bool &getDvorak();

	bool parseKeyFile(const char *fileName);
	KeyboardKey *getKey(const char *name);
			                       
protected:
	static Keyboard *instance_;
	static bool dvorak_; // TODO // FIX ME NOT WORKING
	KeyboardHistory::HistoryElement keybHist_[MAX_KEYBDHIST];
	int keybHistCnt_;

	std::map<std::string, KeyboardKey *, std::less<std::string> > keyMap_;
	std::map<std::string, KeyboardKey *, std::less<std::string> > usedKeyMap_;
                       
private:
	Keyboard();
	virtual ~Keyboard();

};

#endif /* _KEYBOARD_H_ */

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

// Keyboard.h: interface for the Keyboard class.
//
//////////////////////////////////////////////////////////////////////

#include <common/KeyboardHistory.h>

#define KEYDOWN(name,key) (name[key]) 
#define KEYPRESS_START(name, key) { static bool toggle = false; if (KEYDOWN(name, key)) { if (!toggle) { toggle = true; 
#define KEYPRESS_END  } } else { toggle = false; } }
#define MAX_KEYBDHIST 1024

class Keyboard
{
public:
	static Keyboard *instance();

	bool init();

	char *getkeyboardbuffer(unsigned int &bufCnt);
	KeyboardHistory::HistoryElement *getkeyboardhistory(unsigned int &histCnt); 

	void processKeyboardEvent(SDL_Event &event);
	static bool &getDvorak();
			                       
protected:
	static Keyboard *instance_;
	static bool dvorak_; // TODO // FIX ME NOT WORKING
	KeyboardHistory::HistoryElement keybHist_[MAX_KEYBDHIST];
	int keybHistCnt_;
                       
private:
	Keyboard();
	virtual ~Keyboard();

};

#endif /* _KEYBOARD_H_ */

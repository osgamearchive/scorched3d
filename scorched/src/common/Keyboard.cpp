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


// Keyboard.cpp: implementation of the Keyboard class.
//
//////////////////////////////////////////////////////////////////////

#include <common/Keyboard.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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
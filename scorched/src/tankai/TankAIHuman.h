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

#if !defined(AFX_TANKAIHUMAN_H__F71C91A5_B74D_4109_8F7B_ACD471025C28__INCLUDED_)
#define AFX_TANKAIHUMAN_H__F71C91A5_B74D_4109_8F7B_ACD471025C28__INCLUDED_

#include <tankai/TankAI.h>

class TankAIHuman : public TankAI
{
public:
	TankAIHuman(ScorchedContext *context, Tank *tank);
	virtual ~TankAIHuman();

	virtual bool isHuman() { return true; }

	// Inherited from TankAI
	virtual void playMove(const unsigned state, 
		float frameTime, char *buffer, unsigned int keyState);
	virtual void endPlayMove();
	virtual void newGame();
	virtual void nextShot();
	virtual void tankHurt(Weapon *weapon, unsigned int firer);
	virtual void shotLanded(Weapon *weapon, unsigned int firer, 
		Vector &position);

	// Tank move methods and tank defense methods
	//
	// The tankAI and its derived classes call these methods to perform
	// the actions associated with the method name.
	// Humans can only be on a client machine so we
	// don't need to check if we are on the server etc..
	virtual void fireShot();
	virtual void skipShot();
	virtual void resign();
	virtual void move(int x, int y);
	virtual void parachutesUpDown(bool on);
	virtual void shieldsUpDown(unsigned int shieldId=0);
	virtual void useBattery();

protected:
	void movePower(char *buffer, unsigned int keyState, float frameTime);
	void moveUpDown(char *buffer, unsigned int keyState, float frameTime);
	void moveLeftRight(char *buffer, unsigned int keyState, float frameTime);
	void leftRightHUD();
	void upDownHUD();
	void powerHUD();
	void autoAim();

};

#endif // !defined(AFX_TANKAIHUMAN_H__F71C91A5_B74D_4109_8F7B_ACD471025C28__INCLUDED_)

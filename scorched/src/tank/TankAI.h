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


#if !defined(__INCLUDE_TankAIh_INCLUDE__)
#define __INCLUDE_TankAIh_INCLUDE__

#include <weapons/Weapon.h>
#include <common/Vector.h>

class Tank;
class ComsDefenseMessage;
class ComsPlayedMoveMessage;

class TankAI
{
public:
	TankAI(Tank *tank);
	virtual ~TankAI();

	virtual bool isHuman() = 0;

	// State information about when the ai should act
	//
	// These are callback methods information the tank ai
	// when to play its move, or give information about the
	// game state
	virtual void playMove(const unsigned state, 
		float frameTime, char *buffer, unsigned int keyState) = 0;
	virtual void endPlayMove();
	virtual void newGame() = 0;
	virtual void nextRound() = 0;

	// Information about shots landing
	//
	// These are callback methods a tank ai can implement
	// to gain information about what shots they have fired
	// or who fired shots at them
	virtual void tankHurt(Weapon *weapon, unsigned int firer) = 0;
	virtual void shotLanded(Weapon *weapon, unsigned int firer, 
		Vector &position) = 0;

	// Tank move methods and tank defense methods
	//
	// The tankAI and its derived classes call these methods to perform
	// the actions associated with the method name.
	// Each method takes care of the fact if it is on the client or server.
	// If the method sends a message to the client or server these
	// messages are processed by the next set of methods
	virtual void fireShot();
	virtual void skipShot();
	virtual void resign();
	virtual void move(int x, int y);
	virtual void parachutesUpDown(bool on);
	virtual void shieldsUpDown(const char *name=0);
	virtual void useBattery();

protected:
	Tank *currentTank_;

};

#endif

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

#include <common/Vector.h>
#include <engine/ScorchedCollisionIds.h>
#include <GLW/GLWToolTip.h>

class Weapon;
class ScorchedContext;
class Tank;
class ComsDefenseMessage;
class ComsPlayedMoveMessage;

#define TANKAI_DEFINE(y) \
	virtual TankAI *getCopy(Tank *tank, ScorchedContext *context) { TankAI *comp = new y(*this); comp->setTank(tank); comp->setContext(context); return comp; }

class TankAI
{
public:
	TankAI();
	virtual ~TankAI();

	virtual bool isHuman() = 0;
	virtual const char *getName() = 0;
	virtual GLWTip &getDescription() { return description_; }
	virtual TankAI *getCopy(Tank *tank, ScorchedContext *context) = 0;
	virtual bool availableForRandom() { return false; }

	// State information about when the ai should act
	//
	// These are callback methods information the tank ai
	// when to play its move, or give information about the
	// game state
	virtual void playMove(const unsigned state, 
		float frameTime, char *buffer, unsigned int keyState) = 0;
	virtual void endPlayMove();
	virtual void newGame() = 0;
	
	// Set the tank and context this ai is for
	virtual void setTank(Tank *tank);
	virtual void setContext(ScorchedContext *context);

	// Information about shots landing
	//
	// These are callback methods a tank ai can implement
	// to gain information about what shots they have fired
	// or who fired shots at them
	virtual void tankHurt(Weapon *weapon, unsigned int firer) = 0;
	virtual void shotLanded(
		ParticleAction action,
		ScorchedCollisionInfo *collision,
		Weapon *weapon, unsigned int firer, 
		Vector &position) = 0;

protected:
	GLWTip description_;
	Tank *currentTank_;
	ScorchedContext *context_;

};

#endif

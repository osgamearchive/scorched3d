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

#if !defined(AFX_TANKAICOMPUTER_H__5F21C9C7_0F71_4CCC_ABB9_976CF0A5C5EC__INCLUDED_)
#define AFX_TANKAICOMPUTER_H__5F21C9C7_0F71_4CCC_ABB9_976CF0A5C5EC__INCLUDED_

#include <tank/Tank.h>
#include <tankai/TankAI.h>
#include <tankai/TankAIComputerBuyer.h>
#include <tankai/TankAIComputerTarget.h>
#include <tankai/TankAIComputerAim.h>

class XMLNode;
class TankAIComputer : public TankAI
{
public:
	TankAIComputer();
	virtual ~TankAIComputer();

	// Inherited from TankAI
	virtual void reset();
	virtual void newGame();
	virtual bool parseConfig(AccessoryStore &store, XMLNode *node);
	virtual void tankHurt(Weapon *weapon, unsigned int firer);
	virtual void shotLanded(ScorchedCollisionType action,
		ScorchedCollisionInfo *collision,
		Weapon *weapon, unsigned int firer, 
		Vector &position,
		unsigned int landedCounter);
	virtual void playMove(const unsigned state, float frameTime, 
		char *buffer, unsigned int keyState);

	virtual bool isHuman() { return false; }
	virtual void setTank(Tank *tank);

	// Computer only fns
	virtual void buyAccessories();
	virtual void autoDefense();

	// Indicates if this computer ai is available for choice by
	// the random tank ai type
	virtual bool availableForRandom() { return availableForRandom_; }

	// Tank move methods and tank defense methods
	//
	// The tankAI and its derived classes call these methods to perform
	// the actions associated with the method name.
	// Computer AIs can only run in the server context now
	// so no need to check if we are on the server etc.
	virtual void fireShot();
	virtual void skipShot();
	virtual void resign();
	virtual void move(int x, int y);
	virtual void parachutesUpDown(bool on);
	virtual void shieldsUpDown(unsigned int name=0);
	virtual void useBattery();
	void say(const char *text);

TANKAI_DEFINE(TankAIComputer);

protected:
	TankAIComputerTarget tankTarget_;
	TankAIComputerAim tankAim_;
	TankAIComputerBuyer *tankBuyer_;
	std::vector<TankAIComputerBuyer> tankBuyers_;
	bool primaryShot_;
	bool availableForRandom_;
	bool useParachutes_;
	bool useShields_;
	bool useBatteries_;

	void selectFirstShield();
	void raiseDefenses();

};

#endif // !defined(AFX_TANKAICOMPUTER_H__5F21C9C7_0F71_4CCC_ABB9_976CF0A5C5EC__INCLUDED_)

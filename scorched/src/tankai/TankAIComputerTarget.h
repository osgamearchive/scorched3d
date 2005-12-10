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

#if !defined(__INCLUDE_TankAIComputerTargeth_INCLUDE__)
#define __INCLUDE_TankAIComputerTargeth_INCLUDE__

#include <tank/Tank.h>
#include <XML/XMLParser.h>

class TankAIComputerTarget
{
public:
	TankAIComputerTarget();
	virtual ~TankAIComputerTarget();

	bool parseConfig(XMLNode *node);
	void setTank(Tank *tank);
	void shotLanded(
		ScorchedCollisionType action,
		ScorchedCollisionInfo *collision,
		Weapon *weapon, unsigned int firer, 
		Vector &position,
		unsigned int landedCounter);
	void newMatch();
	void tankHurt(Weapon *weapon, unsigned int firer);

	Tank *findTankToShootAt();

protected:
	Tank *currentTank_;
	std::map<unsigned int, int> hitlist_;
	std::string targetType_;

	Tank *findShotAtTankToShootAt();
	Tank *findNearTankToShootAt();
	void addTankToHitList(unsigned int firer, int count);
};

#endif

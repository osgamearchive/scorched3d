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

#if !defined(__INCLUDE_Rollerh_INCLUDE__)
#define __INCLUDE_Rollerh_INCLUDE__

#include <engine/ActionMeta.h>
#include <weapons/WeaponRoller.h>
#include <set>

class Roller : public ActionMeta
{
public:
	Roller();
	Roller(int x, int y, Weapon *weapon, unsigned int playerId);
	virtual ~Roller();

	virtual void init();
	virtual void simulate(float frameTime, bool &remove);
	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);

	unsigned int getPlayerId() { return playerId_; }
	int getX() { return x_; }
	int getY() { return y_; }

REGISTER_ACTION_HEADER(Roller);

protected:
	int x_, y_;
	unsigned int playerId_;
	WeaponRoller *weapon_;
	float totalTime_, rollerTime_;
	std::set<unsigned int> usedPoints_;

	float getHeight(int x, int y);
	bool simulateMoveStep();
	void finished();

};

#endif

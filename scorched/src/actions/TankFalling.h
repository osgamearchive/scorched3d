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


#if !defined(__INCLUDE_TankFallingh_INCLUDE__)
#define __INCLUDE_TankFallingh_INCLUDE__

#include <engine/PhysicsParticle.h>
#include <engine/ScorchedCollisionIds.h>
#include <weapons/Weapon.h>

#include <set>
#include <list>

class TankFalling;
class TankFallingParticle : public PhysicsParticle
{
public:
	TankFallingParticle(TankFalling *tell);

	virtual void init();
	virtual void collision(Vector &position);
	void hadCollision();

protected:
	TankFalling *tell_;
	ScorchedCollisionInfo collisionInfo_;
};

class TankFalling : public ActionMeta
{
public:
	static std::set<unsigned int> fallingTanks;

	TankFalling();
	TankFalling(Weapon *weapon, unsigned int fallingPlayerId,
			unsigned int firedPlayerId,
			unsigned int data);
	virtual ~TankFalling();

	virtual void init();
	virtual void simulate(float frameTime, bool &remove);
	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);

	void collision();

	REGISTER_ACTION_HEADER(TankFalling);

protected:
	std::list<TankFallingParticle *> particles_;
	Weapon *weapon_;
	unsigned int fallingPlayerId_;
	unsigned int firedPlayerId_;
	unsigned int data_;
	Vector tankStartPosition_;
	Vector lastPosition_;
	bool remove_;

	void getAllPositions(Vector &spherePositions);

};


#endif

////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#if !defined(__INCLUDE_TankFallingEndh_INCLUDE__)
#define __INCLUDE_TankFallingEndh_INCLUDE__

#include <engine/PhysicsParticle.h>
#include <engine/ScorchedCollisionIds.h>
#include <weapons/Weapon.h>

class TankFallingEnd : public ActionMeta
{
public:
	TankFallingEnd();
	TankFallingEnd(Weapon *weapon, 
		Vector &startPosition,
		Vector &endPosition,
		unsigned int fallingPlayerId,
		unsigned int firedPlayerId,
		unsigned int data);
	virtual ~TankFallingEnd();

	virtual void init();
	virtual void simulate(float frameTime, bool &remove);
	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);

	REGISTER_ACTION_HEADER(TankFallingEnd);

protected:
	Weapon *weapon_;
	unsigned int fallingPlayerId_;
	unsigned int firedPlayerId_;
	unsigned int data_;
	Vector startPosition_, endPosition_;
};

#endif // __INCLUDE_TankFallingEndh_INCLUDE__

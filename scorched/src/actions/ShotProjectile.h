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

#if !defined(__INCLUDE_ShotProjectileh_INCLUDE__)
#define __INCLUDE_ShotProjectileh_INCLUDE__

#include <engine/PhysicsParticle.h>
#include <engine/ScorchedCollisionIds.h>
#include <engine/ViewPoints.h>
#include <weapons/WeaponProjectile.h>
#include <list>

class ShotProjectile : 
	public PhysicsParticleMeta
{
public:
	ShotProjectile();
	ShotProjectile(
		Vector &startPosition, Vector &velocity,
		Weapon *weapon, unsigned int playerId,
		unsigned int flareType);
	virtual ~ShotProjectile();

	virtual void simulate(float frameTime, bool &remove);
	virtual void init();
	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);
	virtual void collision(Vector &position);

	REGISTER_ACTION_HEADER(ShotProjectile);

	void incLandedCounter() { landedCounter_++; }
	unsigned int getLandedCounter() { return landedCounter_; }
	unsigned int getPlayerId() { return playerId_; }
	WeaponProjectile *getWeapon() { return (WeaponProjectile *) weapon_; }
	std::list<Vector> &getPositions() { return positions_; }

protected:
	static Vector lookatPosition_;
	static unsigned int lookatCount_;
	ScorchedCollisionInfo collisionInfo_;
	Vector startPosition_, velocity_;
	Weapon *weapon_;
	ViewPoints::ViewPoint *vPoint_;
	unsigned int playerId_;
	unsigned int flareType_;
	bool up_;
	float snapTime_;
	std::list<Vector> positions_;
	unsigned int landedCounter_;

	void doCollision(Vector &position);

private:
	ShotProjectile(const ShotProjectile &);
	const ShotProjectile & operator=(const ShotProjectile &);

};

#endif

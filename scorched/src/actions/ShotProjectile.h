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
#include <tankgraph/RenderTracer.h>
#include <weapons/WeaponProjectile.h>
#include <list>

class ShotProjectile : 
	public PhysicsParticleReferenced
{
public:
	ShotProjectile(
		Vector &startPosition, Vector &velocity,
		WeaponProjectile *weapon, WeaponFireContext &weaponContext,
		unsigned int flareType, float spinSpeed);
	virtual ~ShotProjectile();

	virtual void simulate(float frameTime, bool &remove);
	virtual void init();
	virtual void collision(PhysicsParticleObject &position, 
		ScorchedCollisionId collisionId);

	unsigned int getPlayerId() { return weaponContext_.getPlayerId(); }
	WeaponProjectile *getWeapon() { return weapon_; }
	std::list<RenderTracer::TracerLinePoint> &getPositions() { return positions_; }

protected:
	Vector startPosition_, velocity_;
	WeaponProjectile *weapon_;
	ViewPoints::ViewPoint *vPoint_;
	WeaponFireContext weaponContext_;
	unsigned int flareType_;
	bool up_;
	float snapTime_;
	float totalTime_;
	// 	things like thrusttime etc should have their 
	// 	values set ONCE per projectile
	float thrustTime_;
	float thrustAmount_;
	float drag_;
	float timedCollision_;
	float spinSpeed_;
	std::list<RenderTracer::TracerLinePoint> positions_;

	void doCollision(Vector &position);

private:
	ShotProjectile(const ShotProjectile &);
	const ShotProjectile & operator=(const ShotProjectile &);

};

#endif

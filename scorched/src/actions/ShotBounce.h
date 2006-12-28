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

#if !defined(__INCLUDE_ShotBounceh_INCLUDE__)
#define __INCLUDE_ShotBounceh_INCLUDE__

#include <engine/PhysicsParticle.h>
#include <engine/ScorchedCollisionIds.h>
#include <engine/ViewPoints.h>
#include <weapons/WeaponRoller.h>

class ModelRendererSimulator;
class ShotBounce : 
	public PhysicsParticleMeta
{
public:
	ShotBounce();
	ShotBounce(
		Vector &startPosition, Vector &velocity,
		WeaponRoller *weapon, unsigned int playerId,
		unsigned int data);
	virtual ~ShotBounce();

	virtual void simulate(float frameTime, bool &remove);
	virtual void init();
	virtual void draw();
	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);
	virtual void collision(PhysicsParticleObject &position, 
		ScorchedCollisionId collisionId);

	unsigned int getPlayerId() { return playerId_; }
	WeaponRoller *getWeapon() { return weapon_; }

	REGISTER_ACTION_HEADER(ShotBounce);

protected:
	ViewPoints::ViewPoint *vPoint_;
	Vector startPosition_, velocity_;
	Vector lookFrom_;
	WeaponRoller *weapon_;
	unsigned int playerId_;
	unsigned int data_;
	float totalTime_;
	ModelRendererSimulator *model_;

	void doCollision();

private:
	ShotBounce(const ShotBounce &);
	const ShotBounce & operator=(const ShotBounce &);

};

#endif

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


#if !defined(__INCLUDE_PhysicsParticleObjecth_INCLUDE__)
#define __INCLUDE_PhysicsParticleObjecth_INCLUDE__

#include <common/Vector.h>
#include <common/Vector4.h>
#include <engine/ScorchedCollisionIds.h>

enum PhysicsParticleType
{
	ParticleTypeNone = 0,
	ParticleTypeShot,
	ParticleTypeBounce,
	ParticleTypeFalling
};

struct PhysicsParticleInfo
{
	PhysicsParticleInfo(
		PhysicsParticleType type,
		unsigned int playerId,
		void *data) :
		type_(type),
		playerId_(playerId),
		data_(data)
	{
	}

	PhysicsParticleType type_;
	unsigned int playerId_;
	void *data_;
};

class PhysicsParticleObject;
class PhysicsParticleObjectHandler
{
public:
	virtual void collision(PhysicsParticleObject &position, 
		ScorchedCollisionId collisionId) = 0;
};

class Target;
class ScorchedContext;
class PhysicsParticleObject
{
public:
	PhysicsParticleObject();
	virtual ~PhysicsParticleObject();

	void setPhysics(
		PhysicsParticleInfo info,
		ScorchedContext &context, 
		Vector &position, Vector &velocity,
		float sphereSize = 0.0f,
		float sphereDensity = 0.0f,
		float windFactor = 1.0f,
		bool underGroundCollision = false);

	void applyForce(Vector &force);
	void simulate(float frameTime);

	Vector &getPosition() { return position_; }
	Vector &getVelocity() { return velocity_; }
	Vector4 &getRotationQuat() { return rotation_; }

	void setHandler(PhysicsParticleObjectHandler *handler) { handler_ = handler; }
	void setPosition(Vector &position) { position_ = position; }

protected:
	PhysicsParticleInfo info_;
	ScorchedContext *context_;
	PhysicsParticleObjectHandler *handler_;
	bool underGroundCollision_;
	unsigned int iterations_;
	Vector position_;
	Vector velocity_;
	Vector windFactor_;
	Vector4 rotation_;

	enum CollisionAction
	{
		CollisionActionNone = 0,
		CollisionActionCollision,
		CollisionActionBounce
	};
	struct CollisionInfo
	{
		ScorchedCollisionId collisionId;
		float deflectFactor;
		Vector normal;
	};

	void checkCollision();
	CollisionAction checkShotCollision(CollisionInfo &collision, Target *target);
	CollisionAction checkBounceCollision(CollisionInfo &collision, Target *target);
	CollisionAction checkFallingCollision(CollisionInfo &collision, Target *target);

	bool getLandscapeCollision(CollisionInfo &collision);
	bool getRoofCollision(CollisionInfo &collision);
	bool getWallCollision(CollisionInfo &collision);
	bool getShieldCollision(CollisionInfo &collision, Target *target);
	bool getTargetCollision(CollisionInfo &collision, Target *target);
	bool getTargetBounceCollision(CollisionInfo &collision, Target *target);
};

#endif

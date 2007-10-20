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

#if !defined(__INCLUDE_TargetLifeh_INCLUDE__)
#define __INCLUDE_TargetLifeh_INCLUDE__

#include <engine/ScorchedCollisionIds.h>
#include <target/TargetSpaceContainment.h>
#include <net/NetBuffer.h>
#include <common/FixedVector4.h>

class ScorchedContext;
class Target;
class TargetLife
{
public:
	TargetLife(ScorchedContext &context, unsigned int playerId);
	virtual ~TargetLife();

	void newGame();

	// Position
	void setTargetPositionAndRotation(FixedVector &position, fixed rotation);
	void setTargetPosition(FixedVector &position);
	FixedVector &getTargetPosition() { return targetPosition_; }
	FixedVector &getCenterPosition();
	void setSize(FixedVector &size);
	FixedVector &getSize() { return size_; }
	FixedVector4 &getQuaternion() { return quaternion_; }
	FixedVector &getAabbSize() { return aabbSize_; }
	void setRotation(fixed rotation);
	FixedVector &getVelocity() { return velocity_; }
	void setVelocity(FixedVector &velocity) { velocity_ = velocity; }

	void setTarget(Target *target) { target_ = target; }
	void setBoundingSphere(bool sphereGeom);
	bool getBoundingSphere() { return sphereGeom_; }
	TargetSpaceContainment &getSpaceContainment() { return spaceContainment_; }

	// Client only performance functions
	Vector &getFloatPosition() { return floatPosition_; }
	Vector &getFloatAabbSize() { return floatAabbSize_; }
	float *getFloatRotMatrix() { return floatRotMatrix_; }
	Vector &getFloatCenterPosition();

	// Collision
	fixed collisionDistance(FixedVector &position);
	bool collision(FixedVector &position);

	// Tank Life / Health
	fixed getLife() { return life_; }
	void setLife(fixed life);
	fixed getMaxLife() { return maxLife_; }
	void setMaxLife(fixed life) { maxLife_ = life; }

	// Serialize the tank
	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);

protected:
	ScorchedContext &context_;
	TargetSpaceContainment spaceContainment_;
	Target *target_;
	FixedVector4 quaternion_;
	FixedVector targetPosition_;
	FixedVector velocity_;
	FixedVector aabbSize_;
	FixedVector size_;
	Vector floatPosition_;
	Vector floatAabbSize_;
	float floatRotMatrix_[16];
	fixed life_;
	fixed maxLife_;
	bool sphereGeom_;

	void updateSpace();
	void updateAABB();

};

#endif // __INCLUDE_TargetLifeh_INCLUDE__


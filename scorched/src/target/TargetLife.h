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
#include <common/Vector4.h>

class ScorchedContext;
class Target;
class TargetLife
{
public:
	TargetLife(ScorchedContext &context, unsigned int playerId);
	virtual ~TargetLife();

	void newGame();

	// Position
	void setPosition(Vector &pos);
	void setTarget(Target *target) { target_ = target; }
	void setSize(Vector &size);
	Vector &getSize() { return size_; }
	Vector4 getQuaternion() { return quaternion_; }
	Vector getAabbSize() { return aabbSize_; }
	float getRotation() { return rotation_; }
	void setRotation(float rotation);
	void setDriveOverToDestroy(bool d) { driveOverToDestroy_ = d; }
	bool getDriveOverToDestroy() { return driveOverToDestroy_; }
	void setBoundingSphere(bool sphereGeom);
	bool getBoundingSphere() { return sphereGeom_; }
	TargetSpaceContainment &getSpaceContainment() { return spaceContainment_; }

	float collisionDistance(Vector &position);
	bool collision(Vector &position);

	// Tank Life / Health
	float getLife() { return life_; }
	void setLife(float life);
	float getMaxLife() { return maxLife_; }
	void setMaxLife(float life) { maxLife_ = life; }

	// Serialize the tank
	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);

protected:
	ScorchedContext &context_;
	TargetSpaceContainment spaceContainment_;
	Target *target_;
	Vector4 quaternion_;
	Vector aabbSize_;
	Vector size_;
	float rotation_;
	float life_;
	float maxLife_;
	bool sphereGeom_;
	bool driveOverToDestroy_;

	void addToSpace();
	void removeFromSpace();
	void updateAABB();

};

#endif // __INCLUDE_TargetLifeh_INCLUDE__


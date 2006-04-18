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
#include <engine/PhysicsEngine.h>
#include <coms/NetBuffer.h>

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
	float getRotation() { return rotation_; }
	void setRotation(float rotation);
	void setDriveOverToDestroy(bool d) { driveOverToDestroy_ = d; }
	bool getDriveOverToDestroy() { return driveOverToDestroy_; }
	void setBoundingSphere(bool sphereGeom);
	bool getBoundingSphere() { return sphereGeom_; }

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

	Vector size_;
	float rotation_;
	float life_;
	float maxLife_;
	bool sphereGeom_;
	bool driveOverToDestroy_;

	// Physics engine stuff
	dGeomID targetGeom_;
	ScorchedCollisionInfo targetInfo_;
	Target *target_;

};

#endif // __INCLUDE_TargetLifeh_INCLUDE__


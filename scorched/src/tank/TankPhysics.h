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


#if !defined(__INCLUDE_TankPhysicsh_INCLUDE__)
#define __INCLUDE_TankPhysicsh_INCLUDE__

#include <engine/ScorchedCollisionIds.h>
#include <engine/PhysicsEngine.h>
#include <coms/NetBuffer.h>

class TankPhysics
{
public:
	TankPhysics(unsigned int playerId);
	virtual ~TankPhysics();

	// State change
	void newGame();
	void nextRound();

	// Rotation
	float rotateGunXY(float angle, bool diff=true);
	float rotateGunYZ(float angle, bool diff=true);
	void rotateTank(float a) { angle_ = a; }
	float getRotationGunXY() { return turretRotXY_; }
	float getRotationGunYZ() { return turretRotYZ_; }
	float getOldRotationGunXY() { return oldTurretRotXY_; }
	float getOldRotationGunYZ() { return oldTurretRotYZ_; }
	float getAngle() { return angle_; }

	// Position
	void setTankPosition(Vector &pos);
	Vector &getVelocityVector();
	Vector &getTankPosition(); // Position of center bottom of tank
	Vector &getTankTurretPosition(); // Position of center of turret
	Vector &getTankGunPosition(); // Position of end of gun

	// Serialize the tank
    bool writeMessage(NetBuffer &buffer);
    bool readMessage(NetBufferReader &reader);

protected:
	// Position
	Vector position_;

	// Turret angles
	float turretRotXY_, turretRotYZ_;
	float oldTurretRotXY_, oldTurretRotYZ_;
	float angle_;

	// Physics engine stuff
	dGeomID tankGeom_;
	dGeomID shieldSmallGeom_;
	dGeomID shieldLargeGeom_;
	ScorchedCollisionInfo tankInfo_;
	ScorchedCollisionInfo shieldSmallInfo_;
	ScorchedCollisionInfo shieldLargeInfo_;

};


#endif

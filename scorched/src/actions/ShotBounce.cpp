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

#include <actions/ShotBounce.h>
#include <actions/CameraPositionAction.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <weapons/WeaponRoller.h>
#include <weapons/AccessoryStore.h>
#include <GLEXT/GLState.h>
#include <3dsparse/ModelStore.h>
#include <ode/ode.h>
#include <string.h>

REGISTER_ACTION_SOURCE(ShotBounce);

ShotBounce::ShotBounce() : 
	collisionInfo_(CollisionIdBounce),
	totalTime_(0.0f), 
	vPoint_(0), model_(0), data_(0)
{
}

ShotBounce::ShotBounce(Vector &startPosition, Vector &velocity,
	WeaponRoller *weapon, unsigned int playerId,
	unsigned int data) : 
	collisionInfo_(CollisionIdBounce), startPosition_(startPosition),
	velocity_(velocity), weapon_(weapon), playerId_(playerId),
	totalTime_(0.0f), 
	vPoint_(0), model_(0), data_(data)
{
}

void ShotBounce::init()
{
	setPhysics(startPosition_, velocity_, 1.0f, 5.0f);
	collisionInfo_.data = this;
	collisionInfo_.collisionOnSurface = true;
	physicsObject_.setData(&collisionInfo_);

	//vPoint_ = context_->viewPoints.getNewViewPoint(playerId_);
}

ShotBounce::~ShotBounce()
{
	if (vPoint_) context_->viewPoints->releaseViewPoint(vPoint_);
}

void ShotBounce::collision(Vector &position)
{
	doCollision();
	PhysicsParticleMeta::collision(position);
}

void ShotBounce::simulate(float frameTime, bool &remove)
{
	totalTime_ += frameTime;
	if (totalTime_ > weapon_->getTime())
	{
		if (context_->serverMode) doCollision();
		remove = true;
	}

	PhysicsParticleMeta::simulate(frameTime, remove);
}

void ShotBounce::draw()
{
	if (!context_->serverMode) 
	{
		static double rotMatrix[16];
		static dReal quat[4];
		float *fquat = getRotationQuat();
		for (int i=0; i<4; i++)
			quat[i] = (dReal) fquat[i];

		// And I thought this would be easy!!
		dMatrix3 matrix;
		dQtoR(quat, matrix);
		rotMatrix[0] = matrix[0];
		rotMatrix[1] = matrix[4];
		rotMatrix[2] = matrix[8];
		rotMatrix[4] = matrix[1];
		rotMatrix[5] = matrix[5];
		rotMatrix[6] = matrix[9];
		rotMatrix[8] = matrix[2];
		rotMatrix[9] = matrix[6];
		rotMatrix[10] = matrix[10];
		rotMatrix[3] = rotMatrix[7] = rotMatrix[11] = 0.0;
		rotMatrix[15] = 1.0;
		rotMatrix[12] = rotMatrix[13] = rotMatrix[14] = 0.0;	

		if (!model_)
		{
			ModelID &id = ((WeaponRoller *) weapon_)->getRollerModelID();
			bool useTexture = (strcmp(id.getSkinName(), "none") != 0);
			model_ = ModelStore::instance()->loadOrGetArray(id, useTexture, false, false);
		}

		GLState state(GLState::TEXTURE_OFF);
		glPushMatrix();
			glTranslatef(
				getCurrentPosition()[0], 
				getCurrentPosition()[1], 
				getCurrentPosition()[2]);
			glMultMatrixd(rotMatrix);
			glScalef(0.08f, 0.08f, 0.08f);
			model_->draw();
		glPopMatrix();
	}
}

void ShotBounce::doCollision()
{
	WeaponRoller *proj = (WeaponRoller *) weapon_;
	proj->getCollisionAction()->fireWeapon(
		*context_,
		playerId_, getCurrentPosition(), getCurrentVelocity(),
		data_);
}

bool ShotBounce::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(data_);
	buffer.addToBuffer(startPosition_);
	buffer.addToBuffer(velocity_);
	context_->accessoryStore->writeWeapon(buffer, weapon_);
	return true;
}

bool ShotBounce::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(data_)) return false;
	if (!reader.getFromBuffer(startPosition_)) return false;
	if (!reader.getFromBuffer(velocity_)) return false;
	weapon_ = (WeaponRoller *) context_->accessoryStore->readWeapon(reader); if (!weapon_) return false;

	// Point the action camera at this event
	const float ShowTime = 5.0f;
	ActionMeta *pos = new CameraPositionAction(
		startPosition_, ShowTime,
		5);
	context_->actionController->getBuffer().clientAdd(-4.0f, pos);

	return true;
}

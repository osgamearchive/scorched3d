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
#include <GLEXT/GLState.h>
#include <3dsparse/ASEStore.h>

REGISTER_ACTION_SOURCE(ShotBounce);

ShotBounce::ShotBounce() : 
	collisionInfo_(CollisionIdBounce),
	totalTime_(0.0f), actionId_(0), actionVector_(0),
	snapshotTime_(0.0f), vPoint_(0), model_(0)
{
	
}

ShotBounce::ShotBounce(Vector &startPosition, Vector &velocity,
							   Weapon *weapon, unsigned int playerId) : 
	collisionInfo_(CollisionIdBounce), startPosition_(startPosition),
	velocity_(velocity), weapon_(weapon), playerId_(playerId),
	totalTime_(0.0f), actionId_(0), actionVector_(0),
	snapshotTime_(0.0f), vPoint_(0), model_(0)
{
	actionId_ = ActionVectorHolder::getNextActionId();
}

void ShotBounce::init()
{
	if (context_->serverMode)
	{
		setPhysics(startPosition_, velocity_, 1.0f, 5.0f);
		collisionInfo_.data = this;
		collisionInfo_.collisionOnSurface = true;
		physicsObject_.setData(&collisionInfo_);

		actionVector_ = new ActionVector(actionId_);	

		// Point the action camera at this event
		const float ShowTime = 5.0f;
		ActionMeta *pos = new CameraPositionAction(
			startPosition_, ShowTime,
			5);
		context_->actionController->getBuffer().serverAdd(
			context_->actionController->getActionTime() - 4.0f,
			pos);
		delete pos;
	}
	else
	{
		actionVector_ = ActionVectorHolder::getActionVector(actionId_);
	}
	//vPoint_ = context_->viewPoints.getNewViewPoint(playerId_);
}

ShotBounce::~ShotBounce()
{
	ActionVectorHolder::getActionVector(actionId_);
	if (actionVector_) actionVector_->remove();
	if (context_->serverMode) delete actionVector_;
	if (vPoint_) context_->viewPoints->releaseViewPoint(vPoint_);
}

void ShotBounce::collision(Vector &position)
{
	doCollision();
	PhysicsParticleMeta::collision(position);
}

void ShotBounce::simulate(float frameTime, bool &remove)
{
	const float timeStep = 0.1f;
	snapshotTime_ += frameTime;
	while (snapshotTime_ > timeStep)
	{
		if (context_->serverMode) 
		{
			actionVector_->addPoint((unsigned int) getCurrentPosition()[0]);
			actionVector_->addPoint((unsigned int) getCurrentPosition()[1]);
			actionVector_->addPoint((unsigned int) getCurrentPosition()[2]);
		}
		else
		{
			if (!actionVector_ || actionVector_->empty()) remove = true;
			else
			{
				unsigned int pointx = actionVector_->getPoint();
				unsigned int pointy = actionVector_->getPoint();
				unsigned int pointz = actionVector_->getPoint();
				startPosition_[0] = (float) pointx;
				startPosition_[1] = (float) pointy;
				startPosition_[2] = (float) pointz;

				if (vPoint_)
				{
					vPoint_->setPosition(startPosition_);
				}
			}
		}

		snapshotTime_ -= timeStep;
	}

	totalTime_ += frameTime;
	if (totalTime_ > 8.0f)
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
		if (!model_) model_ = ASEStore::instance()->
			loadOrGetArray(getDataFile("data/accessories/roller.ase"));

		GLState state(GLState::TEXTURE_OFF);
		glPushMatrix();
			glTranslatef(startPosition_[0], startPosition_[1], startPosition_[2]);
			glScalef(0.08f, 0.08f, 0.08f);
			model_->draw();
		glPopMatrix();
	}
}

void ShotBounce::doCollision()
{
	if (context_->serverMode)
	{
		context_->actionController->getBuffer().serverAdd(0.0f, actionVector_);
	}
	
	WeaponRoller *proj = (WeaponRoller *) weapon_;
	proj->getCollisionAction()->fireWeapon(
		*context_,
		playerId_, getCurrentPosition(), getCurrentVelocity());
}

bool ShotBounce::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(actionId_);
	return true;
}

bool ShotBounce::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(actionId_)) return false;
	return true;
}

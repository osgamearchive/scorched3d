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

#include <actions/TankFalling.h>
#include <tank/TankController.h>
#include <tank/TankContainer.h>
#include <common/OptionsGame.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <actions/TankMove.h>

REGISTER_ACTION_SOURCE(TankFalling);

std::set<unsigned int> TankFalling::fallingTanks;

TankFallingParticle::TankFallingParticle(TankFalling *tell) : 
	tell_(tell), collisionInfo_(CollisionIdFallingTank)
{

}

void TankFallingParticle::init()
{
	collisionInfo_.data = this;
	physicsObject_.setData(&collisionInfo_);
}

void TankFallingParticle::collision(Vector &position)
{
	if (tell_) tell_->collision();
}

void TankFallingParticle::hadCollision()
{
	collision_ = true;
	tell_ = 0;
}

TankFalling::TankFalling() : remove_(false)
{
}

TankFalling::TankFalling(Weapon *weapon, unsigned int fallingPlayerId,
				   unsigned int firedPlayerId) :
	weapon_(weapon), remove_(false),
	fallingPlayerId_(fallingPlayerId), firedPlayerId_(firedPlayerId)
{
}

TankFalling::~TankFalling()
{
	TankFalling::fallingTanks.erase(fallingPlayerId_);
}

void TankFalling::init()
{
	Tank *current = 
		context_->tankContainer->getTankById(fallingPlayerId_);
	if (current)
	{
		// Store the start positions
		tankStartPosition_ = current->getPhysics().getTankPosition();
		lastPosition_= tankStartPosition_;
		lastPosition_[2] += 0.25f;

		for (int i=0; i<4; i++)
		{
			TankFallingParticle *particle = new TankFallingParticle(this);
			particles_.push_back(particle);

			// The sphere is 0.25 radius
			Vector nullVelocity;
			Vector initPos = current->getPhysics().getTankPosition();	
			switch (i)
			{
			case 0:
				initPos[0] += 0.5f;
				initPos[1] += 0.5f;
				initPos[2] += 0.25;
				break;
			case 1:
				initPos[0] += 0.5f;
				initPos[1] -= 0.5f;
				initPos[2] += 0.25;
				break;
			case 2:
				initPos[0] -= 0.5f;
				initPos[1] -= 0.5f;
				initPos[2] += 0.25;
				break;
			case 3:
				initPos[0] -= 0.5f;
				initPos[1] += 0.5f;
				initPos[2] += 0.25;
				break;
			}

			particle->setScorchedContext(context_);
			particle->setPhysics(initPos, nullVelocity);
			particle->init();

			context_->actionController->addAction(particle);
		}
	}
}

void TankFalling::simulate(float frameTime, bool &remove)
{
	Vector spherePosition;
	getAllPositions(spherePosition);
	
	float distance = lastPosition_[2] - spherePosition[2];
	if (distance > 0.5f)
	{
		lastPosition_ = spherePosition;

		// Calcuate the action position
		Vector position(spherePosition[0], 
			spherePosition[1], 
			spherePosition[2] - 0.25f);

		// Move the tank to the new position
		context_->actionController->addAction(
			new TankMove(position, fallingPlayerId_, false));
	}

	remove = remove_;
	ActionMeta::simulate(frameTime, remove);
}

bool TankFalling::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(fallingPlayerId_);
	buffer.addToBuffer(firedPlayerId_);
	Weapon::write(buffer, weapon_);
	return true;
}

bool TankFalling::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(fallingPlayerId_)) return false;
	if (!reader.getFromBuffer(firedPlayerId_)) return false;
	weapon_ = Weapon::read(reader); if (!weapon_) return false;
	return true;
}

void TankFalling::getAllPositions(Vector &spherePosition)
{
	std::list<TankFallingParticle *>::iterator itor;
	for (itor = particles_.begin();
		itor != particles_.end();
		itor++)
	{
		TankFallingParticle *part = (*itor);
		spherePosition += part->getCurrentPosition();
	}
	spherePosition /= 4.0f;
}

void TankFalling::collision()
{
	// Remove this action and all
	// particle actions
	remove_ = true;
	std::list<TankFallingParticle *>::iterator itor;
	for (itor = particles_.begin();
		itor != particles_.end();
		itor++)
	{
		TankFallingParticle *part = (*itor);
		part->hadCollision();
	}

	Tank *current = 
		context_->tankContainer->getTankById(fallingPlayerId_);
	if (current && current->getState().getState() == TankState::sNormal)
	{
		// Calcuate the action position
		Vector spherePosition;
		getAllPositions(spherePosition);
		Vector position(spherePosition[0], 
			spherePosition[1], 
			spherePosition[2] - 0.25f);

		// Find how far we have falled to get the total damage
		float dist = (tankStartPosition_ - position).Magnitude();
		float damage = dist * 20.0f;

		// Check we need to cancel the damage
		bool useParachute = false;

		float minDist = float(context_->optionsGame->
			getMinFallingDistance()) / 10.0f;
		if (dist < minDist)
		{
			// No damage (or parachutes used for tiny falls)
			damage = 0.0f;
		}
		else
		if (current->getAccessories().getParachutes().parachutesEnabled())
		{
			if (dist >= current->getAccessories().getParachutes().getThreshold())
			{
				// No damage we were using parachutes
				damage = 0.0f;
				useParachute = true;
			}
		}

		// Move the tank to the final position
		context_->actionController->addAction(
			new TankMove(position, fallingPlayerId_, useParachute));

		// Add the damage to the tank
		TankController::damageTank(
			*context_,
			current, weapon_, firedPlayerId_, damage, false);
	}
}

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
#include <tank/TankContainer.h>
#include <tank/TankController.h>
#include <engine/ActionController.h>
#include <actions/TankMove.h>

REGISTER_ACTION_SOURCE(TankFalling);

std::set<unsigned int> TankFalling::fallingTanks;

TankFalling::TankFalling() : collisionInfo_(CollisionIdFallingTank)
{
}

TankFalling::TankFalling(Weapon *weapon, unsigned int fallingPlayerId,
				   unsigned int firedPlayerId) :
	weapon_(weapon),
	collisionInfo_(CollisionIdFallingTank),
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
		TankContainer::instance()->getTankById(fallingPlayerId_);
	if (current)
	{
		// Store the start positions
		tankStartPosition_ = current->getPhysics().getTankPosition();

		// The sphere is 1 radius
		Vector nullVelocity;
		Vector initPos = current->getPhysics().getTankPosition();
		initPos[2] += 1.0f;
		lastPosition_= initPos;
		setPhysics(initPos, nullVelocity);
	}

	collisionInfo_.data = this;
	physicsObject_.setData(&collisionInfo_);
}

void TankFalling::simulate(float frameTime, bool &remove)
{
	Vector &spherePosition = getCurrentPosition();
	float distance = lastPosition_[2] - spherePosition[2];
	if (distance > 0.5f)
	{
		lastPosition_ = spherePosition;

		// Calcuate the action position
		Vector position(spherePosition[0], 
			spherePosition[1], 
			spherePosition[2] - 1.0f);

		// Move the tank to the new position
		ActionController::instance()->addAction(
			new TankMove(position, fallingPlayerId_, false));
	}

	PhysicsParticleMeta::simulate(frameTime, remove);
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

void TankFalling::collision(Vector &spherePosition)
{
	Tank *current = 
		TankContainer::instance()->getTankById(fallingPlayerId_);
	if (current && current->getState().getState() == TankState::sNormal)
	{
		// Calcuate the action position
		Vector position(spherePosition[0], 
			spherePosition[1], 
			spherePosition[2] - 1.0f);

		// Find how far we have falled to get the total damage
		float dist = (tankStartPosition_ - position).Magnitude();
		float damage = dist * 20.0f;

		// Check we need to cancel the damage
		bool useParachute = false;
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
		ActionController::instance()->addAction(
			new TankMove(position, fallingPlayerId_, useParachute));

		// Add the damage to the tank
		TankController::damageTank(
			current, weapon_, firedPlayerId_, damage, false);
	}

	PhysicsParticleMeta::collision(spherePosition);
}

////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <actions/TankFallingEnd.h>
#include <actions/TankFalling.h>
#include <common/OptionsGame.h>
#include <weapons/AccessoryStore.h>
#include <weapons/Parachute.h>
#include <landscapemap/DeformLandscape.h>
#include <target/TargetContainer.h>
#include <target/TargetDamageCalc.h>
#include <target/TargetParachute.h>
#include <target/TargetState.h>
#include <tank/Tank.h>
#include <tank/TankAccessories.h>

REGISTER_ACTION_SOURCE(TankFallingEnd);

TankFallingEnd::TankFallingEnd() :
	weapon_(0),
	fallingPlayerId_(0), firedPlayerId_(0),
	parachute_(0)
{
}

TankFallingEnd::TankFallingEnd(Weapon *weapon, 
	Vector &startPosition,
	Vector &endPosition,
	unsigned int fallingPlayerId,
	unsigned int firedPlayerId,
	Parachute *parachute,
	unsigned int data) :
	weapon_(weapon),
	startPosition_(startPosition),
	endPosition_(endPosition),
	fallingPlayerId_(fallingPlayerId),
	firedPlayerId_(firedPlayerId),
	parachute_(parachute),
	data_(data)
{
}

TankFallingEnd::~TankFallingEnd()
{
}

void TankFallingEnd::init()
{
}

void TankFallingEnd::simulate(float frameTime, bool &remove)
{
	Target *current = 
		context_->targetContainer->getTargetById(fallingPlayerId_);
	if (current)
	{
		if (current->getTargetState().getFalling())
		{
			current->getTargetState().getFalling()->remove();
		}
	}

	if (current && current->getAlive())
	{
		// Find how far we have falled to get the total damage
		float dist = (startPosition_ - endPosition_).Magnitude();
		float damage = dist * 20.0f;

		// Check we need to cancel the damage
		float minDist = float(context_->optionsGame->
			getMinFallingDistance()) / 10.0f;
		if (dist < minDist)
		{
			// No damage (or parachutes used for tiny falls)
			damage = 0.0f;
		}
		else
		if (parachute_)
		{
			const float ParachuteThreshold = 0.0f;
			if (dist >= ParachuteThreshold)
			{
				// No damage we were using parachutes
				damage = 0.0f;

				// Remove parachutes if we have one
				if (!current->isTarget())
				{
					Tank *currentTank = (Tank *) current;
					currentTank->getAccessories().rm(parachute_->getParent());
					if (currentTank->getAccessories().getAccessoryCount(parachute_->getParent()) == 0)
					{
						current->getParachute().setCurrentParachute(0);
					}
				}
			}
		}

		// Move the tank to the final position
		current->setTargetPosition(endPosition_);

		// Flatten the area around tanks
		if (!current->isTarget())
		{
			DeformLandscape::flattenArea(*context_, endPosition_, 0);
		}

		// Add the damage to the tank
		TargetDamageCalc::damageTarget(
			*context_,
			current, weapon_, 
			firedPlayerId_, damage, 
			false, false, false, data_);
	}

	remove=true;
}

bool TankFallingEnd::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(startPosition_);
	buffer.addToBuffer(endPosition_);
	buffer.addToBuffer(fallingPlayerId_);
	buffer.addToBuffer(firedPlayerId_);
	buffer.addToBuffer(data_);
	context_->accessoryStore->writeAccessoryPart(buffer, parachute_);
	context_->accessoryStore->writeWeapon(buffer, weapon_);
	return true;
}

bool TankFallingEnd::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(startPosition_)) return false;
	if (!reader.getFromBuffer(endPosition_)) return false;
	if (!reader.getFromBuffer(fallingPlayerId_)) return false;
	if (!reader.getFromBuffer(firedPlayerId_)) return false;
	if (!reader.getFromBuffer(data_)) return false;
	parachute_ = (Parachute *) context_->accessoryStore->readAccessoryPart(reader);
	weapon_ = context_->accessoryStore->readWeapon(reader); if (!weapon_) return false;
	return true;
}

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
#include <landscape/DeformLandscape.h>
#include <tank/TankContainer.h>
#include <tank/TankController.h>

REGISTER_ACTION_SOURCE(TankFallingEnd);

TankFallingEnd::TankFallingEnd() :
	weapon_(0),
	fallingPlayerId_(0), firedPlayerId_(0)
{
}

TankFallingEnd::TankFallingEnd(Weapon *weapon, 
	Vector &startPosition,
	Vector &endPosition,
	unsigned int fallingPlayerId,
	unsigned int firedPlayerId,
	unsigned int data) :
	weapon_(weapon),
	startPosition_(startPosition),
	endPosition_(endPosition),
	fallingPlayerId_(fallingPlayerId),
	firedPlayerId_(firedPlayerId),
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
	Tank *current = 
		context_->tankContainer->getTankById(fallingPlayerId_);
	if (current && current->getState().getState() == TankState::sNormal)
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
		if (current->getAccessories().getParachutes().parachutesEnabled())
		{
			if (dist >= current->getAccessories().getParachutes().getThreshold())
			{
				// No damage we were using parachutes
				damage = 0.0f;
				current->getAccessories().getParachutes().useParachutes();
			}
		}

		// Move the tank to the final position
		current->getPhysics().setTankPosition(endPosition_);
		DeformLandscape::flattenArea(*context_, endPosition_, 0);

		// Add the damage to the tank
		TankController::damageTank(
			*context_,
			current, weapon_, 
			firedPlayerId_, damage, 
			false, false, false, data_);
	}

	std::map<unsigned int, TankFalling *>::iterator findItor =
		TankFalling::fallingTanks.find(fallingPlayerId_);
	if (findItor != TankFalling::fallingTanks.end())
	{
		(*findItor).second->remove();
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
	weapon_ = context_->accessoryStore->readWeapon(reader); if (!weapon_) return false;
	return true;
}

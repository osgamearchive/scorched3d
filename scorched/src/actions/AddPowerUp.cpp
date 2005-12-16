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

#include <actions/AddPowerUp.h>
#include <engine/ScorchedContext.h>
#include <target/TargetContainer.h>
#include <target/TargetDamageCalc.h>
#include <weapons/AccessoryStore.h>
#include <weapons/PowerUp.h>
#include <weapons/Shield.h>

REGISTER_ACTION_SOURCE(AddPowerUp);

AddPowerUp::AddPowerUp() :
	powerUp_(0)
{
}

AddPowerUp::AddPowerUp(unsigned int playerId,
	Vector &position,
	PowerUp *powerUp) :
	position_(position),
	playerId_(playerId),
	powerUp_(powerUp)
{

}

AddPowerUp::~AddPowerUp()
{
}

void AddPowerUp::init()
{
}

void AddPowerUp::simulate(float frameTime, bool &remove)
{
	TargetModelId targetModelId(powerUp_->getPowerUpModel());
	Target *target = new Target(playerId_, 
		targetModelId, 
		"PowerUp", *context_);
	target->newGame();
	target->setTargetPosition(position_);

	if (powerUp_->getShield())
	{
		target->getShield().setCurrentShield(
			powerUp_->getShield()->getParent());
	}
	if (powerUp_->getParachute())
	{
		target->getParachute().setParachutesEnabled(true);
	}

	context_->targetContainer->addTarget(target);

	// Check if this new target can fall
	TargetDamageCalc::damageTarget(*context_, target, powerUp_, 
		0, 0.0f, false, true, false, 0);

	remove = true;
	Action::simulate(frameTime, remove);
}

bool AddPowerUp::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(position_);
	buffer.addToBuffer(playerId_);
	context_->accessoryStore->writeWeapon(buffer, powerUp_);
	return true;
}

bool AddPowerUp::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(position_)) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	powerUp_ = (PowerUp *) context_->accessoryStore->readWeapon(reader); 
	if (!powerUp_) return false;
	return true;
}

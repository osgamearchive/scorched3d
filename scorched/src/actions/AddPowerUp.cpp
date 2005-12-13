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
#include <weapons/AccessoryStore.h>

REGISTER_ACTION_SOURCE(AddPowerUp);

AddPowerUp::AddPowerUp()
{
}

AddPowerUp::AddPowerUp(unsigned int playerId,
	Vector &position) :
	position_(position),
	playerId_(playerId)
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
	ModelID model;
	model.initFromString("MilkShape", "data\\accessories\\banana\\banana.txt", "");
	TargetModelId targetModel(model);
	Target *target = new Target(playerId_, targetModel, "PowerUp", *context_);
	target->newGame();
	target->setTargetPosition(position_);

	Accessory *accessory = context_->accessoryStore->findByPrimaryAccessoryName("Shield");
	target->getShield().setCurrentShield(accessory);

	context_->targetContainer->addTarget(target);

	remove = true;
	Action::simulate(frameTime, remove);
}

bool AddPowerUp::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(position_);
	buffer.addToBuffer(playerId_);
	return true;
}

bool AddPowerUp::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(position_)) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	return true;
}

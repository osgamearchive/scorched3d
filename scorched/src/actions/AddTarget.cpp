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

#include <actions/AddTarget.h>
#include <engine/ScorchedContext.h>
#include <target/TargetContainer.h>
#include <target/TargetDamageCalc.h>
#include <weapons/AccessoryStore.h>
#include <weapons/WeaponAddTarget.h>
#include <weapons/Shield.h>
#include <common/RandomGenerator.h>

AddTarget::AddTarget(unsigned int playerId,
	Vector &position,
	WeaponAddTarget *addTarget) :
	position_(position),
	playerId_(playerId),
	addTarget_(addTarget)
{

}

AddTarget::~AddTarget()
{
}

void AddTarget::init()
{
}

void AddTarget::simulate(float frameTime, bool &remove)
{
	RandomGenerator generator;
	generator.seed(playerId_);
	Target *target = addTarget_->getTargetDefinition().createTarget(
		playerId_, position_, Vector::nullVector, *context_, generator);
	context_->targetContainer->addTarget(target);

	// Check if this new target can fall
	TargetDamageCalc::damageTarget(*context_, target, addTarget_, 
		0, 0.0f, false, true, false, 0);

	remove = true;
	Action::simulate(frameTime, remove);
}

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


#include <actions/TankDamage.h>
#include <actions/TankDead.h>
#include <actions/TankFalling.h>
#include <tank/TankContainer.h>
#include <engine/ActionController.h>
#include <landscape/GlobalHMap.h>

REGISTER_ACTION_SOURCE(TankDamage);

TankDamage::TankDamage() : firstTime_(true)
{
}

TankDamage::TankDamage(Weapon *weapon, 
		unsigned int damagedPlayerId, unsigned int firedPlayerId,
		float damage, bool useShieldDamage) :
	weapon_(weapon), firstTime_(true),
	damagedPlayerId_(damagedPlayerId), firedPlayerId_(firedPlayerId),
	damage_(damage), useShieldDamage_(useShieldDamage)
{
}

TankDamage::~TankDamage()
{
}

void TankDamage::init()
{
}

void TankDamage::simulate(float frameTime, bool &remove)
{
	if (firstTime_)
	{
		firstTime_ = false;

		// Find the tank that has been damaged
		Tank *damagedTank = 
			TankContainer::instance()->getTankById(damagedPlayerId_);
		if (damagedTank)
		{
			if (damagedTank->getState().getState() == TankState::sNormal)
			{
				// Tell this tanks ai that is has been hurt by another tank
				TankAI *ai = damagedTank->getTankAI();
				if (ai) ai->tankHurt(weapon_, firedPlayerId_);

				// Remove any damage from shield first
				if (damage_ > 0.0f)
				{
					float shieldDamage = 0.0f;
					Shield *sh = damagedTank->getAccessories().getShields().getCurrentShield();
					if (sh && useShieldDamage_)
					{
						// TODO: Hmm should different shield types and sizes take damage
						// at different rates
						float power = damagedTank->getAccessories().getShields().getShieldPower();
						if (power < damage_)
						{
							power = 0.0f;
							damage_ -= power;
						}
						else
						{
							power -= damage_;
							damage_ = 0.0f;
						}

						damagedTank->getAccessories().getShields().setShieldPower(power);
					}
				}

				// Remove the remaining damage from the tank
				if (damage_ > 0.0f)
				{
					damagedTank->getState().setLife(damagedTank->getState().getLife() - damage_);

					// Check if the tank is dead
					if (damagedTank->getState().getLife() == 0.0f)
					{
						// The tank has died, make it blow up etc...
						TankDead *deadTank = 
							new TankDead(weapon_, damagedPlayerId_, firedPlayerId_);
						ActionController::instance()->addAction(deadTank);
					}
				}

				// The tank is not dead check if it needs to fall
				Vector &position = damagedTank->getPhysics().getTankPosition();
				if (GlobalHMap::instance()->getHMap().
					getInterpHeight(position[0], position[1]) < position[2])
				{
					// Check this tank is not already falling
					std::set<unsigned int>::iterator findItor =
						TankFalling::fallingTanks.find(damagedPlayerId_);
					if (findItor == TankFalling::fallingTanks.end())
					{
						TankFalling::fallingTanks.insert(damagedPlayerId_);

						// Tank falling
						ActionController::instance()->addAction(
							new TankFalling(weapon_, damagedPlayerId_, firedPlayerId_));
					}
				}
			}
		}
	}

	remove = true;
	Action::simulate(frameTime, remove);
}

bool TankDamage::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(damagedPlayerId_);
	buffer.addToBuffer(firedPlayerId_);
	buffer.addToBuffer(damage_);
	buffer.addToBuffer(useShieldDamage_);
	Weapon::write(buffer, weapon_);
	return true;
}

bool TankDamage::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(damagedPlayerId_)) return false;
	if (!reader.getFromBuffer(firedPlayerId_)) return false;
	if (!reader.getFromBuffer(damage_)) return false;
	if (!reader.getFromBuffer(useShieldDamage_)) return false;
	weapon_ = Weapon::read(reader); if (!weapon_) return false;
	return true;
}

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

#include <landscape/GlobalHMap.h>
#include <landscape/Landscape.h>
#include <tank/TankContainer.h>
#include <tank/TankController.h>
#include <actions/Napalm.h>
#include <sprites/NapalmRenderer.h>
#include <common/OptionsParam.h>
#include <weapons/WeaponNapalm.h>
#include <weapons/AccessoryStore.h>

// TODO, should these be configurable in the OptionsGame file?
static const float NapalmHeight = 2.0f; // The height of a napalm point
static const float StepTime = 0.1f; // Add/rm napalm every StepTime secs
static const float HurtStepTime = 2.0f; // Calculate damage every HurtStepTime secs
static const float HurtPerSecond = 1.0f;
static const float HurtPerSecondHot = 2.0f;
static const int EffectRadius = 5; // The distance of the burn radius

REGISTER_ACTION_SOURCE(Napalm);

Napalm::Napalm() : hitWater_(false), totalTime_(0.0f), hurtTime_(0.0f)
{
}

Napalm::Napalm(int x, int y, float napalmTime, Weapon *weapon, unsigned int playerId) :
	x_(x), y_(y), napalmTime_(napalmTime), weapon_(weapon), playerId_(playerId), hitWater_(false),
	totalTime_(0.0f), hurtTime_(0.0f)
{

}

Napalm::~Napalm()
{

}

void Napalm::init()
{
	if (!OptionsParam::instance()->getOnServer()) 
	{
		setActionRender(new NapalmRenderer);
	}
}

void Napalm::simulate(float frameTime, bool &remove)
{
	// Add napalm for the period of the time interval
	// once the time interval has expired then start taking it away
	// Once all napalm has disapeared the simulation is over
	totalTime_ += frameTime;
	while (totalTime_ > StepTime)
	{
		totalTime_ -= StepTime;
		napalmTime_ -= StepTime;
		if (napalmTime_ > 0.0f)
		{
			// Still within the time period, add more napalm
			simulateAddStep();

			// Check for the case where we land in water
			if (napalmPoints_.empty())
			{
				remove = true;
				break;
			}
		}
		else
		{
			// Not within the time period remove napalm
			if (!napalmPoints_.empty())
			{
				simulateRmStep();
			}
			else
			{
				remove = true;
				break;
			}
		}
	}

	// Calculate how much damage to make to the tanks
	hurtTime_ += frameTime;
	while (hurtTime_ > HurtStepTime)
	{
		hurtTime_ -= HurtStepTime;

		simulateDamage();
	}

	Action::simulate(frameTime, remove);
}

float Napalm::getHeight(int x, int y)
{
	GlobalHMap *hmap = GlobalHMap::instance();
	if (x < 0 || y < 0 ||
		x > hmap->getHMap().getWidth() ||
		y > hmap->getHMap().getWidth())
	{
		// The height at the sides of the landscape is huge
		// so we will never go there with the napalm
		return 9999999.0f;
	}

	// Return the correct height the square + the
	// height of all the napalm on this square
	// the napalm builds up and get higher so
	// we can go over small bumps
	return hmap->getHMap().getHeight(x, y) +
		hmap->getNMap().getHeight(x, y);
}

void Napalm::simulateRmStep()
{
	// Remove the first napalm point from the list
	// and remove the height from the landscape
	NapalmEntry *entry = napalmPoints_.front();
	int x = entry->posX;
	int y = entry->posY;
	delete entry;

	GlobalHMap::instance()->getNMap().getHeight(x, y) -= NapalmHeight;
	napalmPoints_.pop_front();
}

void Napalm::simulateAddStep()
{
	// Get the height of this point
	float height = getHeight(x_, y_);

	// Napalm does not go under water (for now)
	// Perhaps we could add a boiling water sound at some point
	if (height < Landscape::instance()->getWater().getHeight())
	{
		if (!hitWater_)
		{
			// This is the first time we have hit the water
			hitWater_ = true;
			if (!OptionsParam::instance()->getOnServer()) 
			{
				// TODO: Play hit water sound
			}
		}
		return;
	}

	// Add this current point to the napalm map
	napalmPoints_.push_back(new NapalmEntry(x_, y_, int(RAND * 31)));
	GlobalHMap::instance()->getNMap().getHeight(x_, y_) += NapalmHeight;
	height += NapalmHeight;

	// Calculate every time as the landscape may change
	// due to other actions
	float heightL = getHeight(x_-1, y_);
	float heightR = getHeight(x_+1, y_);
	float heightU = getHeight(x_, y_+1);
	float heightD = getHeight(x_, y_-1);

	// Find the new point to move to (if any)
	// This point must be lower than the current point
	if (heightL < height &&
		heightL < heightR &&
		heightL < heightU &&
		heightL < heightD)
	{
		// Move left
		x_ -= 1;
	}
	else if (heightR < height &&
		heightR < heightL &&
		heightR < heightU &&
		heightR < heightD)
	{
		// Move right
		x_ += 1;
	}
	else if (heightU < height &&
		heightU < heightL &&
		heightU < heightR &&
		heightU < heightD)
	{
		// Move up
		y_ += 1;
	}
	else if (heightD < height)
	{
		// Move down
		y_ -= 1;
	}
	else
	{
		// None of the landscape is currently lower than the current point
		// Just wait, as this point will be now be covered in napalm
		// and may get higher and higher until it is
	}
}

void Napalm::simulateDamage()
{
	float damagePerPointSecond = HurtPerSecond;
	if (((WeaponNapalm *)weapon_)->getHot()) damagePerPointSecond = HurtPerSecondHot;

	// Store how much each tank is damaged
	// Keep in a map so we don't need to create multiple
	// damage actions.  Now we only create one per tank
	static std::map<Tank *, float> tankDamage;

	// Get the tanks
	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator tankItor;
	std::map<unsigned int, Tank *>::iterator endTankItor = tanks.end();

	// Add damage into the damage map for each napalm point that is near to
	// the tanks
	std::list<NapalmEntry *>::iterator itor;
	std::list<NapalmEntry *>::iterator endItor = 
		napalmPoints_.end();
	for (itor = napalmPoints_.begin();
		itor != endItor;
		itor++)
	{
		NapalmEntry *entry = (*itor);
		int napalmX = entry->posX;
		int napalmY = entry->posY;

		for (tankItor = tanks.begin();
			tankItor != endTankItor;
			tankItor++)
		{
			Tank *tank = (*tankItor).second;
			if (tank->getState().getState() == TankState::sNormal)
			{
				// Check if this tank is in the damage field
				Vector &tankPos = tank->getPhysics().getTankPosition();
				if (tankPos[0] > napalmX - EffectRadius &&
					tankPos[0] < napalmX + EffectRadius &&
					tankPos[1] > napalmY - EffectRadius &&
					tankPos[1] < napalmY + EffectRadius)
				{
					std::map<Tank *, float>::iterator damageItor = 
						tankDamage.find(tank);
					if (damageItor == tankDamage.end())
					{
						tankDamage[tank] = damagePerPointSecond;
					}
					else
					{
						tankDamage[tank] += damagePerPointSecond;
					}
				}
			}
		}
	}

	// Add all the damage to the tanks (if any)
	if (!tankDamage.empty())
	{
		std::map<Tank *, float>::iterator damageItor;
		for (damageItor = tankDamage.begin();
			damageItor != tankDamage.end();
			damageItor++)
		{
			Tank *tank = (*damageItor).first;
			float damage = (*damageItor).second;

			// Add damage to the tank
			TankController::damageTank(tank, weapon_, playerId_, damage, true);
		}
		tankDamage.clear();
	}
}

bool Napalm::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(x_);
	buffer.addToBuffer(y_);
	buffer.addToBuffer(napalmTime_);
	Weapon::write(buffer, weapon_);
	buffer.addToBuffer(playerId_);
	return true;
}

bool Napalm::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(x_)) return false;
	if (!reader.getFromBuffer(y_)) return false;
	if (!reader.getFromBuffer(napalmTime_)) return false;
	weapon_ = Weapon::read(reader); if (!weapon_) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	return true;
}

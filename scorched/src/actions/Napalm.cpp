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

#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <tank/TankContainer.h>
#include <tank/TankController.h>
#include <actions/Napalm.h>
#include <actions/CameraPositionAction.h>
#include <sprites/ExplosionTextures.h>
#include <sprites/NapalmRenderer.h>
#include <GLEXT/GLBitmapModifier.h>
#include <GLEXT/GLStateExtension.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/LandscapeDefinition.h>
#include <landscape/LandscapeTex.h>
#include <weapons/AccessoryStore.h>
#include <common/OptionsParam.h>
#include <common/Defines.h>
#include <client/ScorchedClient.h>

REGISTER_ACTION_SOURCE(Napalm);

Napalm::Napalm() : hitWater_(false), totalTime_(0.0f), hurtTime_(0.0f),
	napalmTime_(0.0f), counter_(0.1f, 0.1f), set_(0)
{
}

Napalm::Napalm(int x, int y, Weapon *weapon, 
	unsigned int playerId, unsigned int data) :
	x_(x), y_(y), napalmTime_(0.0f), 
	weapon_((WeaponNapalm *) weapon), 
	playerId_(playerId), data_(data), hitWater_(false),
	totalTime_(0.0f), hurtTime_(0.0f),
	counter_(0.1f, 0.1f), set_(0)
{
}

Napalm::~Napalm()
{
}

void Napalm::init()
{
	if (!context_->serverMode) 
	{
		set_ = ExplosionTextures::instance()->getTextureSetByName(
			weapon_->getNapalmTexture());
	}
	nmap_.create(
		context_->landscapeMaps->getGroundMaps().getMapWidth(),
		context_->landscapeMaps->getGroundMaps().getMapHeight());
}

void Napalm::simulate(float frameTime, bool &remove)
{
	if (!context_->serverMode)
	{
		if (!weapon_->getNoSmoke() &&
			counter_.nextDraw(frameTime))
		{
			int count = int(RAND * float(napalmPoints_.size()));

			std::list<Napalm::NapalmEntry *>::iterator itor;
			std::list<Napalm::NapalmEntry *>::iterator endItor = 
				napalmPoints_.end();
			for (itor = napalmPoints_.begin();
					itor != endItor;
					itor++, count--)
			{
				NapalmEntry *entry = (*itor);
				if (count == 0)
				{
					float posZ = 
						ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getHeight(
						entry->posX, entry->posY);
					Landscape::instance()->getSmoke().
						addSmoke(float(entry->posX), float(entry->posY), posZ);
					break;
				}
			}
		}
	}

	// Add napalm for the period of the time interval
	// once the time interval has expired then start taking it away
	// Once all napalm has disapeared the simulation is over
	const float StepTime = weapon_->getStepTime();
	const float HurtStepTime = weapon_->getHurtStepTime();

	totalTime_ += frameTime;
	while (totalTime_ > StepTime)
	{
		totalTime_ -= StepTime;
		napalmTime_ += StepTime;
		if (napalmTime_ < weapon_->getNapalmTime())
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
	LandscapeMaps *hmap = context_->landscapeMaps;
	if (x < 0 || y < 0 ||
		x > hmap->getGroundMaps().getMapWidth() ||
		y > hmap->getGroundMaps().getMapHeight())
	{
		// The height at the sides of the landscape is huge
		// so we will never go there with the napalm
		return 9999999.0f;
	}

	// Return the correct height the square + the
	// height of all the napalm on this square
	// the napalm builds up and get higher so
	// we can go over small bumps
	return hmap->getGroundMaps().getHeight(x, y) +
		nmap_.getNapalmHeight(x, y);
}

void Napalm::simulateRmStep()
{
	const float NapalmHeight = weapon_->getNaplamHeight();

	// Remove the first napalm point from the list
	// and remove the height from the landscape
	NapalmEntry *entry = napalmPoints_.front();
	napalmPoints_.pop_front();
	int x = entry->posX;
	int y = entry->posY;
	delete entry;

	nmap_.getNapalmHeight(x, y) -= NapalmHeight;
}

void Napalm::simulateAddStep()
{
	const float NapalmHeight = weapon_->getNaplamHeight();

	// Get the height of this point
	float height = getHeight(x_, y_);

	if (!weapon_->getAllowUnderWater())
	{
		// Napalm does not go under water (for now)
		// Perhaps we could add a boiling water sound at some point
		float waterHeight = -10.0f;
		LandscapeTex &tex = *context_->landscapeMaps->getDefinitions().getTex();
		if (tex.border->getType() == LandscapeTexType::eWater)
		{
			LandscapeTexBorderWater *water = 
				(LandscapeTexBorderWater *) tex.border;
       	 	waterHeight = water->height;
		}

		if (height < waterHeight) // Water height
		{
			if (!hitWater_)
			{
				// This is the first time we have hit the water
				hitWater_ = true;
				if (!context_->serverMode) 
				{
					// TODO: Play hit water sound
				}
			}
			return;
		}
	} 

	// Add this current point to the napalm map
	int offset = int(RAND * 31);
	NapalmEntry *newEntry = new NapalmEntry(x_, y_, offset);
	napalmPoints_.push_back(newEntry);
	if (!context_->serverMode)
	{
		ParticleEmitter emitter;
		emitter.setAttributes(
			weapon_->getNapalmTime(), weapon_->getNapalmTime(),
			0.5f, 1.0f, // Mass
			0.01f, 0.02f, // Friction
			Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), // Velocity
			Vector(1.0f, 1.0f, 1.0f), 0.9f, // StartColor1
			Vector(1.0f, 1.0f, 1.0f), 0.6f, // StartColor2
			Vector(1.0f, 1.0f, 1.0f), 0.0f, // EndColor1
			Vector(1.0f, 1.0f, 1.0f), 0.1f, // EndColor2
			1.5f, 1.5f, 1.5f, 1.5f, // Start Size
			1.5f, 1.5f, 1.5f, 1.5f, // EndSize
			Vector(0.0f, 0.0f, 0.0f), // Gravity
			true,
			false);
		Vector position1(float(x_) + 0.5f, float(y_) - 0.2f, 0.0f);
		Vector position2(float(x_) - 0.5f, float(y_) - 0.2f, 0.0f);
		Vector position3(float(x_) + 0.0f, float(y_) + 0.5f, 0.0f);
		emitter.emitNapalm(
			position1, 
			ScorchedClient::instance()->getParticleEngine(),
			set_);
		emitter.emitNapalm(
			position2, 
			ScorchedClient::instance()->getParticleEngine(),
			set_);
		emitter.emitNapalm(
			position3, 
			ScorchedClient::instance()->getParticleEngine(),
			set_);
	}

	if (!weapon_->getNoObjectDamage())
	{
		context_->landscapeMaps->getGroundMaps().getObjects().burnObjects(
			*context_,
			(unsigned int) x_, (unsigned int) y_,
			playerId_);
		context_->landscapeMaps->getGroundMaps().getObjects().burnObjects(
			*context_,
			(unsigned int) x_ + 1, (unsigned int) y_ + 1,
			playerId_);
		context_->landscapeMaps->getGroundMaps().getObjects().burnObjects(
			*context_,
			(unsigned int) x_ - 1, (unsigned int) y_ - 1,
			playerId_);
	}

	nmap_.getNapalmHeight(x_, y_) += NapalmHeight;
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
	float damagePerPointSecond = weapon_->getHurtPerSecond();
	const int EffectRadius = weapon_->getEffectRadius();

	// Store how much each tank is damaged
	// Keep in a map so we don't need to create multiple
	// damage actions.  Now we only create one per tank
	static std::map<Tank *, float> tankDamage;

	// Get the tanks
	std::map<unsigned int, Tank *> &tanks = 
		context_->tankContainer->getPlayingTanks();
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
			TankController::damageTank(*context_, tank, weapon_, 
				playerId_, damage, true, false, false, data_);
		}
		tankDamage.clear();
	}
}

bool Napalm::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(x_);
	buffer.addToBuffer(y_);
	context_->accessoryStore->writeWeapon(buffer, weapon_);
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(data_);
	return true;
}

bool Napalm::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(x_)) return false;
	if (!reader.getFromBuffer(y_)) return false;
	weapon_ = (WeaponNapalm *) context_->accessoryStore->readWeapon(reader); if (!weapon_) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(data_)) return false;

	const float ShowTime = 5.0f;
	Vector position((float) x_, (float) y_, getHeight(x_, y_));
	ActionMeta *pos = new CameraPositionAction(
		position, ShowTime, 5);
	context_->actionController->getBuffer().clientAdd(-4.0f, pos);

	return true;
}

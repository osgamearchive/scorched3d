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

#include <tankai/TankAICurrentMove.h>
#include <tankai/TankAIAimGuesser.h>
#include <tankai/TankAISniperGuesser.h>
#include <coms/ComsPlayedMoveMessage.h>
#include <coms/ComsDefenseMessage.h>
#include <server/ServerShotHolder.h>
#include <server/ServerDefenseHandler.h>
#include <server/ScorchedServer.h>
#include <tank/Tank.h>
#include <tank/TankLib.h>
#include <tank/TankPosition.h>
#include <tank/TankAccessories.h>
#include <target/TargetLife.h>
#include <target/TargetShield.h>
#include <target/TargetParachute.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapemap/MovementMap.h>
#include <landscapemap/GroundMaps.h>
#include <weapons/AccessoryStore.h>
#include <weapons/Shield.h>
#include <weapons/WeaponMoveTank.h>
#include <XML/XMLNode.h>

TankAICurrentMove::TankAICurrentMove() : useResign_(true)
{
}

TankAICurrentMove::~TankAICurrentMove()
{
}

bool TankAICurrentMove::parseConfig(XMLNode *node)
{
	{
		XMLNode *targets = 0;
		if (!node->getNamedChild("targets", targets)) return false;
		if (!targets_.parseConfig(targets)) return false;
	}

	return true;
}

void TankAICurrentMove::playMove(Tank *tank, 
	TankAIWeaponSets::WeaponSet *weapons, bool useBatteries)
{
	std::list<Tank *> sortedTanks;	
	// If we have no weapons, we can't do anything!!
	if (!weapons->weapons.empty())
	{
		// Find the list of tanks we can shoot at 
		// In the order we want to shoot at them
		targets_.getTargets(tank, sortedTanks);
	}

	// Check if we have been damaged by a target and we can move
	// TODO
	if (false)
	{
		// Bring the health back up
		if (useBatteries)
		{
			useAvailableBatteries(tank);
		}

		if (tank->getLife().getLife() > 75.0f)
		{
			// Try to move
			if (makeMoveShot(tank, weapons, sortedTanks)) return;
		}
	}

	while (!sortedTanks.empty())
	{
		// Get the first tank
		Tank *targetTank = sortedTanks.front();
		sortedTanks.pop_front();

		// Get the list of weapons that might make sense here
		TankAICurrentMoveWeapons moveWeapons(tank, targetTank, weapons);

		// Try to shoot at it
		if (shootAtTank(tank, targetTank, moveWeapons)) return;

		// Keeping trying to shoot at the tanks until we make a shot
		// or run out of tanks

		// Check if we can use batteries, as perhaps we couldn't shoot 
		// due to a lack of power
		if (useBatteries &&
			tank->getLife().getLife() < tank->getLife().getMaxLife())
		{
			// Bring the health back up
			useAvailableBatteries(tank);

			// Perhaps we can reach now so do this tank again
			sortedTanks.push_front(targetTank);
		}
	}

	// Try to move so we can get a better shot at the targets
	// Only move if we have a hope of hitting them
	if (tank->getLife().getLife() > 75.0f)
	{
		targets_.getTargets(tank, sortedTanks);
		if (makeMoveShot(tank, weapons, sortedTanks)) return;
	}

	// Is there any point in making a move
	// Done after select weapons to allow shields to be raised
	if (useResign_ &&
		tank->getLife().getLife() < 10) 
	{
		resign(tank);
		return;
	}

	// By default skip this move if we can't find anything to do
	// Perhaps we are burried etc...
	skipMove(tank);
}

bool TankAICurrentMove::shootAtTank(Tank *tank, Tank *targetTank, 
	TankAICurrentMoveWeapons &weapons)
{
	// Try to make a sniper shot
	if (makeSniperShot(tank, targetTank, weapons)) return true;

	// Try to make a laser shot
	if (makeLaserSniperShot(tank, targetTank, weapons)) return true;

	// Then a projectile shot
	if (makeProjectileShot(tank, targetTank, weapons)) return true;

	// Check if we are burried
	if (makeBurriedShot(tank, targetTank, weapons)) return true;

	// TODO sniperwobble
	// TODO missing and refining shots
	// TODO try to move
	// TODO try to find people close together

	return false;
}

bool TankAICurrentMove::makeProjectileShot(Tank *tank, Tank *targetTank, 
	TankAICurrentMoveWeapons &weapons)
{
	float aimDistance = 5.0f;

	// Get the place we want to shoot at
	Vector directTarget = targetTank->getPosition().getTankPosition();

	// Check if the person is in a hole
	bool inhole = false;
	if (weapons.roller &&
		inHole(directTarget))
	{
		inhole = true;

		// Check for reflective shields
		if (weapons.shield &&
			(weapons.shield->getShieldType() == Shield::ShieldTypeRoundReflective ||
			weapons.shield->getShieldType() == Shield::ShieldTypeSquareReflective))
		{
			// Pick an area outside the shield
			// and make sure its downhill if we can
			directTarget = lowestHighest(weapons, directTarget, false);
		}
	}
	else
	{
		// Check for reflective shields
		if (weapons.shield &&
			(weapons.shield->getShieldType() == Shield::ShieldTypeRoundReflective ||
			weapons.shield->getShieldType() == Shield::ShieldTypeSquareReflective))
		{
			// Pick an area outside the shield
			// and make sure its uphill if we can
			directTarget = lowestHighest(weapons, directTarget, true);
		}
	}

	// Check for all angles to see if we can shoot at this tank
	for (float degs=45.0f; degs<=85.0f; degs+=8.0f)
	{
		// Check this angle
		float actualDistance;
		TankAIAimGuesser aimGuesser;
		if (aimGuesser.guess(tank, directTarget, degs, aimDistance, actualDistance))
		{	
			// We can fire at this tank
			// ...
			// Check how close we are
			if (actualDistance < 10.0f)
			{
				// Check if the tank is in a hole
				if (inhole)
				{
					setWeapon(tank, weapons.roller);
				}
				else
				{
					// We are close
					// Choose a suitably good weapon
					if (weapons.shield)
					{
						// A shield beating weapon
						if (weapons.napalm) setWeapon(tank, weapons.napalm);
						else if (weapons.digger) setWeapon(tank, weapons.digger);
						else if (weapons.large) setWeapon(tank, weapons.large);
						else return false;
					}
					else
					{
						
						// A normal weapon
						if (weapons.large) setWeapon(tank, weapons.large);		
						else if (weapons.digger) setWeapon(tank, weapons.digger);
						else if (weapons.small) setWeapon(tank, weapons.small);					
						else return false;
					}
				}
			}
			else
			{
				// We are not close, choose a cheap weapon
				if (weapons.small) setWeapon(tank, weapons.small);	
				else if (weapons.large) setWeapon(tank, weapons.large);
				else return false;
			}

			// Fire the shot
			targets_.shotAt(targetTank);
			fireShot(tank);
			return true;
		}
	}

	return false;
}

bool TankAICurrentMove::makeSniperShot(Tank *tank, Tank *targetTank, 
	TankAICurrentMoveWeapons &weapons)
{
	float sniperDistance = 80.0f;

	// Get the place we want to shoot at
	Vector directTarget = targetTank->getPosition().getTankPosition();

	// First check to see if we can make a sniper shot that carries all the way
	// as this is generaly an easier shot
	TankAISniperGuesser sniperGuesser;
	if (sniperGuesser.guess(tank, directTarget, sniperDistance, true))
	{
		// We can make a ordinary sniper shot

		// Does this target have a bouncy shield
		if (!weapons.shield ||
			(weapons.shield->getShieldType() != Shield::ShieldTypeRoundReflective &&
			weapons.shield->getShieldType() != Shield::ShieldTypeSquareReflective))
		{
			// This is good, use a normal sniper shot
			if (weapons.shield)
			{
				// Use a shield beating weapon			
				if (weapons.digger) setWeapon(tank, weapons.digger);
				else if (weapons.napalm) setWeapon(tank, weapons.napalm);
				else if (weapons.laser) setWeapon(tank, weapons.laser);
				else if (weapons.large) setWeapon(tank, weapons.large);
				else return false;
			}
			else
			{
				// Just use an ordinary weapon
				if (weapons.digger) setWeapon(tank, weapons.digger);
				else if (weapons.large) setWeapon(tank, weapons.large);
				else if (weapons.small) setWeapon(tank, weapons.small);
				else return false;
			}

			// Fire the shot
			fireShot(tank);
			return true;
		}
		else if (weapons.laser)
		{
			// They have a reflective shield but we can use a laser
			// Set and fire the laser
			setWeapon(tank, weapons.laser);
			fireShot(tank);
			return true;
		}
	}

	return false;
}

bool TankAICurrentMove::makeLaserSniperShot(Tank *tank, Tank *targetTank, 
	TankAICurrentMoveWeapons &weapons)
{
	float sniperDistance = 80.0f;

	// Get the place we want to shoot at
	Vector directTarget = targetTank->getPosition().getTankPosition();
	
	// Second check to see if we can make a sniper shot that is obstructed
	// but could use a laser
	TankAISniperGuesser sniperGuesser;
	if (sniperGuesser.guess(tank, directTarget, sniperDistance, false))
	{
		if (weapons.laser)
		{
			// Set and fire the laser
			setWeapon(tank, weapons.laser);
			fireShot(tank);
			return true;
		}
	}

	return false;
}

bool TankAICurrentMove::makeBurriedShot(Tank *tank, Tank *targetTank, 
	TankAICurrentMoveWeapons &weapons)
{
	// Find a shot towards a target
	float xy, yz, power;
	TankLib::getSniperShotTowardsPosition(
		ScorchedServer::instance()->getContext(),
		tank->getPosition().getTankPosition(), targetTank->getPosition().getTankPosition(),
		100000.0f,
		xy, yz, power);

	// Check if this shot is burried
	if (TankLib::intersection(
		ScorchedServer::instance()->getContext(), 
		tank->getPosition().getTankGunPosition(), 
		xy, yz, power, 2))
	{
		// Try to uncover the tank
		if (weapons.uncover)
		{
			tank->getPosition().rotateGunXY(xy, false);
			tank->getPosition().rotateGunYZ(yz, false);
			tank->getPosition().changePower(power, false);

			setWeapon(tank, weapons.uncover);
			fireShot(tank);
			return true;
		}
	}

	return false;
}

bool TankAICurrentMove::inHole(Vector &position)
{
	// Find the lowest pos around
	Vector pos = position;
	for (;;)
	{
		Vector lowest = pos;
		for (float a=0.0f; a<360.0f; a+=45.0f)
		{
			float offSetX = sinf(a / 180.0f * PI) * 1.25f;
			float offSetY = cosf(a / 180.0f * PI) * 1.25f;

			Vector newPos(
				pos[0] + offSetX,
				pos[1] + offSetY);
			newPos[2] =
				ScorchedServer::instance()->getLandscapeMaps().
					getGroundMaps().getInterpHeight(newPos[0], newPos[1]);
			if (newPos[2] < lowest[2]) lowest = newPos;
		}

		if (lowest[2] < pos[2])
		{
			pos = lowest;
			Vector direction = pos - position;
			float dist =
				float(sqrt(direction[0]*direction[0] + direction[1]*direction[1]));
			if (dist > 6.0f) return false;
		}
		else
		{
			break;
		}
	}

	// Then see if this is in a hole
	for (float a=0.0f; a<360.0f; a+=22.5f)
	{
		bool ok = false;
		for (float radius=2.0f; radius<10.0f; radius+=1.0f)
		{
			float offSetX = sinf(a / 180.0f * PI) * radius;
			float offSetY = cosf(a / 180.0f * PI) * radius;
			
			Vector newPos(
				pos[0] + offSetX,
				pos[1] + offSetY);
			newPos[2] =
				ScorchedServer::instance()->getLandscapeMaps().
					getGroundMaps().getInterpHeight(newPos[0], newPos[1]);

			float heightDiff = newPos[2] - pos[2];
			if (heightDiff < -2.0f) 
			{
				return false; // Its lower
			}
			if (heightDiff > 2.0f) 
			{
				ok = true;
				break;
			}
		}
		if (!ok) return false;
	}

	return true;
}

bool TankAICurrentMove::makeMoveShot(Tank *tank, 
	TankAIWeaponSets::WeaponSet *weapons,
	std::list<Tank *> &sortedTanks)
{
	if (sortedTanks.empty()) return false;

	Accessory *fuel = weapons->getTankAccessoryByType(tank, "fuel");
	if (!fuel) return false;

	ScorchedContext &context = ScorchedServer::instance()->getContext();
	WeaponMoveTank *moveWeapon = (WeaponMoveTank *)
		context.accessoryStore->findAccessoryPartByAccessoryId(
			fuel->getAccessoryId(), "WeaponMoveTank");
	if (moveWeapon)
	{
		MovementMap mmap(
			context.landscapeMaps->getGroundMaps().getMapWidth(),
			context.landscapeMaps->getGroundMaps().getMapHeight(),
			tank, 
			moveWeapon, 
			context);
		mmap.calculateAllPositions();

		if (false)
		{
			setWeapon(tank, fuel);
			fireShot(tank);
			return true;
		}
	}

	return false;
}

void TankAICurrentMove::checkGrouping()
{

}

void TankAICurrentMove::useAvailableBatteries(Tank *tank)
{
	// Use batteries
	while (tank->getLife().getLife() < 
		tank->getLife().getMaxLife() &&
		tank->getAccessories().getBatteries().getNoBatteries() != 0)
	{
		std::list<Accessory *> &entries =
			tank->getAccessories().getAllAccessoriesByType(
				AccessoryPart::AccessoryBattery);			
		if (!entries.empty())
		{
			useBattery(tank, entries.front()->getAccessoryId());
		}
	}
}

Vector TankAICurrentMove::lowestHighest(TankAICurrentMoveWeapons &weapons, 
	Vector &directTarget, bool highest)
{
	float radius = weapons.shield->getBoundingSize() + 2.0f;
	Vector bestPos = directTarget;
	bestPos[1] += radius;
	bestPos[2] = 
		ScorchedServer::instance()->getLandscapeMaps().
			getGroundMaps().getInterpHeight(bestPos[0], bestPos[1]);
	for (float a=0.0f; a<360.0f; a+=22.5f)
	{
		float offSetX = sinf(a / 180.0f * PI) * radius;
		float offSetY = cosf(a / 180.0f * PI) * radius;
		
		Vector newPos(
			directTarget[0] + offSetX,
			directTarget[1] + offSetY);
		newPos[2] =
			ScorchedServer::instance()->getLandscapeMaps().
				getGroundMaps().getInterpHeight(newPos[0], newPos[1]);

		if (highest)
		{
			if (newPos[2] > bestPos[2]) bestPos = newPos;
		}
		else
		{
			if (newPos[2] < bestPos[2]) bestPos = newPos;
		}
	}
	return bestPos;
}


void TankAICurrentMove::setWeapon(Tank *tank, Accessory *accessory)
{
	tank->getAccessories().getWeapons().setWeapon(accessory);
}

void TankAICurrentMove::skipMove(Tank *tank)
{
	ComsPlayedMoveMessage *message = 
		new ComsPlayedMoveMessage(tank->getPlayerId(), ComsPlayedMoveMessage::eSkip);
	ServerShotHolder::instance()->addShot(tank->getPlayerId(), message);
}

void TankAICurrentMove::resign(Tank *tank)
{
	ComsPlayedMoveMessage *message = 
		new ComsPlayedMoveMessage(tank->getPlayerId(), ComsPlayedMoveMessage::eResign);
	ServerShotHolder::instance()->addShot(tank->getPlayerId(), message);
}

void TankAICurrentMove::fireShot(Tank *tank)
{
	Accessory *currentWeapon = 
		tank->getAccessories().getWeapons().getCurrent();
	if (currentWeapon)
	{
		ComsPlayedMoveMessage *message = 
			new ComsPlayedMoveMessage(tank->getPlayerId(), ComsPlayedMoveMessage::eShot);
		message->setShot(
			currentWeapon->getAccessoryId(),
			tank->getPosition().getRotationGunXY(),
			tank->getPosition().getRotationGunYZ(),
			tank->getPosition().getPower(),
			tank->getPosition().getSelectPositionX(),
			tank->getPosition().getSelectPositionY());
	
		ServerShotHolder::instance()->addShot(tank->getPlayerId(), message);
	}
}

void TankAICurrentMove::useBattery(Tank *tank, unsigned int batteryId)
{
	ComsDefenseMessage defenseMessage(
		tank->getPlayerId(),
		ComsDefenseMessage::eBatteryUse,
		batteryId);

	ServerDefenseHandler::instance()->processDefenseMessage(defenseMessage, tank);
}

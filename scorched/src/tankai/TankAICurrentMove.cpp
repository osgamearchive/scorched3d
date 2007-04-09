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
#include <coms/ComsPlayedMoveMessage.h>
#include <server/ServerShotHolder.h>
#include <tank/Tank.h>
#include <tank/TankPosition.h>
#include <tank/TankAccessories.h>
#include <weapons/Accessory.h>
#include <target/TargetLife.h>
#include <target/TargetShield.h>
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

void TankAICurrentMove::playMove(Tank *tank)
{
	// Is there any point in making a move
	// Done after select weapons to allow batteries to be used
	// and shields to be raised
	if (useResign_ &&
		tank->getLife().getLife() < 10 &&
		!tank->getShield().getCurrentShield()) 
	{
		resign(tank);
		return;
	}

	// Find the list of tanks we can shoot at 
	// In the order we want to shoot at them
	std::list<Tank *> sortedTanks;
	targets_.getTargets(tank, sortedTanks);
	while (!sortedTanks.empty())
	{
		// Get the first tank
		Tank *targetTank = sortedTanks.front();
		sortedTanks.pop_front();

		// Check the tank for shields
		// Check for in holes etc...
		// Choose a weapon
		float distance = 5.0f;

		// Get the place we want to shoot at
		Vector target = targetTank->getPosition().getTankPosition();

		// Check for all angles to see if we can shoot at this tank
		for (float degs=45.0f; degs<=85.0f; degs+=8.0f)
		{
			// Check this angle
			float actualDistance;
			TankAIAimGuesser guesser;
			if (guesser.guess(tank, target, degs, distance, actualDistance))
			{	
				// We can fire at this tank
				targets_.shotAt(targetTank);
				fireShot(tank);
				return;
			}
		}
	}

	// By default resign if we can't find anything to do
	skipMove(tank);
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

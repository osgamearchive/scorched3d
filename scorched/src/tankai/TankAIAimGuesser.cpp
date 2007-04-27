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

#include <tankai/TankAIAimGuesser.h>
#include <tank/Tank.h>
#include <tank/TankLib.h>
#include <tank/TankPosition.h>
#include <server/ScorchedServer.h>
#include <common/Logger.h>
#include <common/RandomGenerator.h>
#include <math.h>

TankAIAimGuesser::TankAIAimGuesser()
{
}

TankAIAimGuesser::~TankAIAimGuesser()
{
}

bool TankAIAimGuesser::guess(Tank *tank, Vector &target, 
	float angleYZDegs, float distance, 
	Vector &actualPosition)
{
	tank->getPosition().rotateGunYZ(angleYZDegs, false);

	// Make an initial randomish shot up
	initialShot(tank, target);

	for (int i=0;;i++)
	{
		// Find out where this may land
		getCurrentGuess(tank);
		if (!collision_)
		{
			// No collision
			// A bad thing
			return false; 
		}

		// Its landed
		actualPosition = currentGuess_.getPosition();
		Vector direction = currentGuess_.getPosition() - target;
		float actualDistance = 
			float(sqrt(direction[0]*direction[0] + direction[1]*direction[1]));
		if (actualDistance < distance)
		{
			// Its close
			// A good thing
			return true;
		}

		if (i > 50)
		{
			// No soultion
			// A bad thing
			return false;
		}

		// Not close
		refineShot(tank, currentGuess_.getPosition(), target);
	}

	// Never gets here
	return false;
}

void TankAIAimGuesser::initialShot(Tank *tank, Vector &target)
{
	float angleXYDegs;
	float angleYZDegs;
	float power;

	// Makes a randow powered shot towards the target
	RandomGenerator random;
	random.seed(rand());
	TankLib::getShotTowardsPosition(
		ScorchedServer::instance()->getContext(),
		random,
		tank->getPosition().getTankPosition(), 
		target, 
		angleXYDegs, angleYZDegs, power);

	// Set the parameters
	// Sets the angle of the gun and the power
	tank->getPosition().rotateGunXY(angleXYDegs, false);
	tank->getPosition().changePower(power, false);
}

void TankAIAimGuesser::refineShot(Tank *tank,
	Vector &currentPos, Vector &wantedPos)
{
	// Get the used velocity
	Vector shotVelocity = tank->getPosition().getVelocityVector() *
		(tank->getPosition().getPower() + 1.0f);

	// Figure out how much the last shot missed by
	Vector missedBy = wantedPos - currentPos;
	
	// Adjust velocity to take this into account
	shotVelocity[0] += missedBy[0] * 0.04f;
	shotVelocity[1] += missedBy[1] * 0.04f;

	// Figure out a new XY angle based on these
	float angleXYRads = atan2f(shotVelocity[1], shotVelocity[0]);
	float angleXYDegs = (angleXYRads / 3.14f) * 180.0f - 90.0f;

	tank->getPosition().rotateGunXY(angleXYDegs, false);

	// And the new best power
	Vector resultingVelocity = tank->getPosition().getVelocityVector();
	float resultingDistance = sqrtf(
		resultingVelocity[0] * resultingVelocity[0] +
		resultingVelocity[1] * resultingVelocity[1]);
	float wantedDistance = sqrtf(
		shotVelocity[0] * shotVelocity[0] +
		shotVelocity[1] * shotVelocity[1]);
	float power = wantedDistance / resultingDistance - 1.0f;

	tank->getPosition().changePower(power, false);
}

void TankAIAimGuesser::collision(PhysicsParticleObject &position, 
	ScorchedCollisionId collisionId)
{
	currentGuess_ = position;
	collision_ = true;
}

void TankAIAimGuesser::wallCollision(PhysicsParticleObject &position,
	ScorchedCollisionId collisionId)
{
	// For now the AIs cannot cope with bouncy and wrap walls.
	// So they will count the shot end where they hit the wall
	currentGuess_ = position;
	collision_ = true;
}

void TankAIAimGuesser::getCurrentGuess(Tank *tank)
{
	Vector shotVelocity = tank->getPosition().getVelocityVector() *
		(tank->getPosition().getPower() + 1.0f);
	Vector shotPosition = tank->getPosition().getTankGunPosition();

	PhysicsParticleObject particleObject;
	PhysicsParticleInfo info(ParticleTypeShot, tank->getPlayerId(), 0);
	particleObject.setPhysics(info, ScorchedServer::instance()->getContext(),
		shotPosition, shotVelocity);
	particleObject.setHandler(this);

	collision_ = false;
	for (int i=0; i<10000 && !collision_; i++)
	{
		particleObject.simulate(1.0f);
	}
}

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

#include <tank/TankLib.h>
#include <tank/TankContainer.h>
#include <common/OptionsTransient.h>
#include <common/Defines.h>
#include <landscape/LandscapeMaps.h>
#include <GLEXT/GLState.h>
#include <math.h>

float TankLib::getDistanceToTank(Vector &position, Tank *targetTank)
{
	Vector currentdirection = 
			targetTank->getPhysics().getTankPosition() - position;
	float maxdistance2D = sqrtf(currentdirection[0] * 
		currentdirection[0] + currentdirection[1] * currentdirection[1]);
	return maxdistance2D;
}

void TankLib::getTanksSortedByDistance(ScorchedContext &context,
									   Vector &position, 
									  std::list<std::pair<float, Tank *> > &result,
									  unsigned int teams,
									  float maxDistance)
{
	std::list<std::pair<float, Tank *> > tankDistList;
	std::map<unsigned int, Tank *> &allCurrentTanks = 
		context.tankContainer->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = allCurrentTanks.begin();
		itor != allCurrentTanks.end();
		itor++)
	{
		Tank *targetTank = (*itor).second;
		if (targetTank->getState().getState() == TankState::sNormal)
		{
			if (teams > 0 && teams == targetTank->getTeam()) continue;

			float maxdistance2D = getDistanceToTank(position, targetTank);
			tankDistList.push_back(std::pair<float, Tank*>(maxdistance2D, targetTank));
		}
	}

	while (!tankDistList.empty())
	{
		std::list<std::pair<float, Tank *> >::iterator removeItor = tankDistList.begin();
		std::list<std::pair<float, Tank *> >::iterator itor = tankDistList.begin(); itor++;
		for (;itor != tankDistList.end(); itor++)
		{
			if ((*itor).first <  (*removeItor).first) 
			{
				removeItor = itor;
			}
		}

		if ((*removeItor).first <= maxDistance || maxDistance == -1) result.push_back(*removeItor);
		tankDistList.erase(removeItor);
	}
}

bool TankLib::intersection(ScorchedContext &context,
	Vector position, float xy, float yz, float power, 
	int dist, bool drawDots)
{
	Vector startPosition = position;
	Vector velocity = getVelocityVector(xy, yz) * power / 250.0f;

	do
	{
		if (position[2] < context.landscapeMaps->getHMap().getInterpHeight(
			position[0], position[1]))
		{
			return true;
		}
		if (drawDots) glVertex3fv(position);
		position += velocity;
		velocity += Vector(0.0f, 0.0f, -0.00015f);
	} while ((position - startPosition).Magnitude() < dist);

    return false;
}

bool TankLib::getSniperShotTowardsPosition(ScorchedContext &context,
	Vector &position, Vector &shootAt, float distForSniper, 
	float &angleXYDegs, float &angleYZDegs, float &power,
	bool checkIntersection)
{
	// Calculate direction
	Vector direction = shootAt - position;
	float angleXYRads = atan2f(direction[1], direction[0]);
	angleXYDegs = (angleXYRads / 3.14f) * 180.0f - 90.0f;
	float distance2D = sqrtf(direction[0] * direction[0] + 
		direction[1] * direction[1]);

	// Special case
	// If we are less than a certain distance and under the position we
	// will use a direct shot on full power
	bool useSniper = ((distance2D < distForSniper) && (shootAt[2] >= position[2])) ||
		(distForSniper == -1.0f);
	if (!useSniper) return false;

	power = 1000.0f;
	float angleYZRads = atan2f(distance2D, direction[2]);
	angleYZDegs = 90.0f - ((angleYZRads / 3.14f) * 180.0f);

	// If we check intersection
	if (checkIntersection)
	{
		// Ensure that the sniper shot wont collide with the ground
		// nearer than 80% dist to target away
		int allowedIntersectDist = int(distance2D * 0.8f);
		if (intersection(context, position + Vector(0.0f, 0.0f, 0.5f), 
			angleXYDegs, angleYZDegs, power, allowedIntersectDist))
		{
			return false;
		}
	}

	return true;
}

void TankLib::getShotTowardsPosition(ScorchedContext &context,
	Vector &position, Vector &shootAt,
	float &angleXYDegs, float &angleYZDegs, float &power)
{
	// Calculate direction
	Vector direction = shootAt - position;
	float angleXYRads = atan2f(direction[1], direction[0]);
	angleXYDegs = (angleXYRads / 3.14f) * 180.0f - 90.0f;
	float distance2D = sqrtf(direction[0] * direction[0] + 
		direction[1] * direction[1]);

	// Calculate power (very roughly)
	power = 1000.0f;
	if (distance2D < 200.0f)
	{
		power = 750;
		if (distance2D < 100.0f)
		{
			power = 550;
			if (distance2D < 50.0f)
			{
				power = 350;
			}
		}
	}

	// Add some randomness to the power
	power += (RAND * 200.0f) - 100.0f;
	if (power < 100) power = 100;

	if (context.optionsTransient->getWindOn())
	{
		// Make less adjustments for less wind
		float windMag = context.optionsTransient->getWindSpeed() / 5.0f;

		// Try to account for the wind direction
		Vector ndirection = direction;
		ndirection[2] = 0.0f;
		ndirection = ndirection.Normalize();
		ndirection = ndirection.get2DPerp();
		float windoffsetLR = context.optionsTransient->getWindDirection().dotP(ndirection);
		angleXYDegs += windoffsetLR * distance2D * (0.12f + RAND * 0.04f) * windMag;

		float windoffsetFB = context.optionsTransient->getWindDirection().dotP(direction.Normalize());
		windoffsetFB /= 10.0f;
		windoffsetFB *= windMag;
		windoffsetFB += 1.0f; // windowoffset FB 0.9 > 1.1

		power *= windoffsetFB;
	}

	// Angle
	angleYZDegs = 45.0f;
}

Vector &TankLib::getVelocityVector(float xy, float yz)
{
	static Vector diff;
	diff = Vector((float) sinf(-xy / 180.0f * 3.14f) *
				cosf(yz / 180.0f * 3.14f),
				(float) cosf(-xy / 180.0f * 3.14f) * 
				cosf(yz / 180.0f * 3.14f),
				(float) sinf(yz/ 180.0f * 3.14f));	
	diff /= 20.0f;
	return diff;
}

Vector &TankLib::getGunPosition(float xy, float yz)
{
	static Vector pos;
	const float gunLength = 1.0f;
	const float degToRad = 180.0f * 3.14f;
	pos[0] = gunLength * sinf(xy / degToRad) * 
		sinf((90.0f - yz) / degToRad);
	pos[1] = gunLength * cosf(xy / degToRad) * 
		sinf((90.0f - yz) / degToRad);
	pos[2] = gunLength * cosf((90.0f - yz) / degToRad);

	return pos;
}
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

#include <target/TargetDamageCalc.h>
#include <tank/TankContainer.h>
#include <target/TargetLife.h>
#include <common/Vector.h>
#include <common/Logger.h>
#include <engine/ActionController.h>
#include <actions/TankDamage.h>

void TargetDamageCalc::explosion(ScorchedContext &context,
							   Weapon *weapon, unsigned int firer, 
							   Vector &position, float radius,
							   float damageAmount, bool checkFall,
							   bool shieldOnlyDamage,
							   unsigned int data)
{
	std::map<unsigned int, Target *>::iterator itor;
	std::map<unsigned int, Target *> &targets = 
		context.targetContainer->getTargets();
	for (itor = targets.begin();
		itor != targets.end();
		itor++)
	{
		Target *current = (*itor).second;
		if (!current->getAlive()) continue;

		Vector &currentPosition = current->getCenterPosition();
		Vector direction = position - currentPosition;
		float dist = 0.0f;

		// Get how close the exposion was
		if (current->getLife().getBoundingSphere())
		{
			// Find how close the explosion was to the 
			// outside of the sphere
			float sphereRadius = MAX(
				current->getLife().getSize()[0], 
				current->getLife().getSize()[1]) / 2.0f;

			dist = direction.Magnitude() - sphereRadius;
			if (dist < 0.0f) dist = 0.0f;
		}
		else
		{
			// Make the direction relative to the geom
			// incase the geom has been rotated
			Vector relativeDirection = 
				current->getLife().getGeomRelativePosition(direction);

			// Find how close the explosion was to the 
			// outside edge of the cube
			Vector touchPosition = relativeDirection;

			// Check each size of the cube to see if the point is outside.
			// If it is, then scale it back until the point sits on the
			// outside edge of the cube.
			int inner = 0;
			for (int i=0; i<3; i++)
			{
				float halfSize = current->getLife().getSize()[i] / 2.0f;
				if (touchPosition[i] < -halfSize)
				{
					// Scale the point so it sits on this edge
					float diff = -halfSize / touchPosition[i];
					touchPosition *= diff;
				}
				else if (touchPosition[i] > halfSize)
				{
					// Scale the point so it sits on this edge
					float diff = halfSize / touchPosition[i];
					touchPosition *= diff;
				}
				else inner++; // The point is inside this edge
			}

			if (inner == 3)
			{
				// We are inside the cube
				dist = 0.0f;
			}
			else
			{
				// We are outside the cube
				relativeDirection -= touchPosition;
				dist = relativeDirection.Magnitude();
			}

			//Logger::log(formatString("%.1f", dist));
		}
		
		// Check if the explosion causes damage
		if (dist < radius)
		{
			// Direct hit by explosion
			float damage = 100.0f;
			if (dist > radius / 3.0f)
			{
				damage = ((dist - (radius / 3.0f)) / (radius * 0.66f)) * 100.0f;
				damage = 100.0f - damage;
			}

			damageTarget(context, current, weapon, firer, 
				damage * damageAmount, true, checkFall, shieldOnlyDamage, data);
		}
		else 
		{
			float dist2d = sqrtf(direction[0] * direction[0] + 
				direction[1] * direction[1]);
			if (dist2d < radius + 5.0f)
			{
				// explosion under tank
				damageTarget(context, current, weapon, firer, 
					0, true, checkFall, shieldOnlyDamage, data);
			}
		}
	}
}

void TargetDamageCalc::damageTarget(ScorchedContext &context,
								Target *target, Weapon *weapon, 
								unsigned int firer, float damage,
								bool useShieldDamage, bool checkFall,
								bool shieldOnlyDamage,
								unsigned int data)
{
	// Remove the correct damage from the tanks
	TankDamage *tankDamage = new TankDamage(
		weapon, target->getPlayerId(), firer, 
		damage, useShieldDamage, checkFall, shieldOnlyDamage, data);
	context.actionController->addAction(tankDamage);
}

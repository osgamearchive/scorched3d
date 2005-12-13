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
#include <common/Vector.h>
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

		// Get how close the exposion was
		Vector direction = position - current->getTargetPosition();
		float dist = direction.Magnitude();
		float dist2d = sqrtf(direction[0] * direction[0] + 
			direction[1] * direction[1]);

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
		else if (dist2d < radius + 5.0f)
		{
			// explosion under tank
			damageTarget(context, current, weapon, firer, 
				0, true, checkFall, shieldOnlyDamage, data);
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

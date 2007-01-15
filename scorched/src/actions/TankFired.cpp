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

#include <actions/TankFired.h>
#include <actions/Explosion.h>
#include <actions/TankSay.h>
#include <weapons/AccessoryStore.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <sound/SoundUtils.h>
#include <common/Defines.h>
#include <tank/TankContainer.h>
#include <tank/TankPosition.h>
#include <tankai/TankAIStrings.h>
#include <target/TargetRenderer.h>

TankFired::TankFired(unsigned int playerId,
					 Weapon *weapon,
					 float rotXY, float rotXZ) :
	playerId_(playerId), weapon_(weapon),
	rotXY_(rotXY), rotXZ_(rotXZ), firstTime_(true)
{

}

TankFired::~TankFired()
{
}

void TankFired::init()
{

}

void TankFired::simulate(float frameTime, bool &remove)
{
	if (firstTime_)
	{
		firstTime_ = false;
		Tank *tank = 
			context_->tankContainer->getTankById(playerId_);
		if (tank)
		{
			tank->getPosition().rotateGunXY(rotXY_, false);
			tank->getPosition().rotateGunYZ(rotXZ_, false);
			tank->getPosition().madeShot();

			if (tank->getDestinationId() == 0)
			{
				const char *line = TankAIStrings::instance()->getAttackLine(
					context_->actionController->getRandom());
				if (line)
				{
					std::string newText(tank->getName());
					newText += ": ";
					unsigned int infoLen = newText.length();
					newText += line;

					context_->actionController->addAction(
						new TankSay(tank->getPlayerId(), newText.c_str(), infoLen));
				}
			}

#ifndef S3D_SERVER
			if (!context_->serverMode) 
			{
				TargetRenderer *renderer = tank->getRenderer();
				if (renderer)
				{
					renderer->fired();
				}

				// play fired sound
				if (weapon_->getParent()->getActivationSound() &&
					0 != strcmp("none", weapon_->getParent()->getActivationSound()))
				{
					SoundBuffer *firedSound = 
						Sound::instance()->fetchOrCreateBuffer( (char*)
							getDataFile(formatString("data/wav/%s", 
							weapon_->getParent()->getActivationSound())));
					SoundUtils::playAbsoluteSound(VirtualSoundPriority::eAction,
						firedSound, tank->getPosition().getTankPosition());
				}
			}
#endif // #ifndef S3D_SERVER

			if (weapon_->getParent()->getMuzzleFlash())
			{
				WeaponFireContext weaponContext(playerId_, 0);
				Weapon *muzzleFlash = context_->accessoryStore->getMuzzelFlash();
				if (muzzleFlash) muzzleFlash->fireWeapon(*context_, weaponContext, 
					tank->getPosition().getTankGunPosition(), Vector::nullVector);
			}
		}
	}

	remove = true;
	Action::simulate(frameTime, remove);
}

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

#include <actions/ShieldHit.h>
#include <sound/SoundUtils.h>
#include <common/Defines.h>
#include <engine/ScorchedContext.h>
#include <weapons/Accessory.h>
#include <weapons/Shield.h>
#include <target/TargetContainer.h>
#include <target/TargetShield.h>
#include <GLEXT/GLLenseFlare.h>
#include <tankgraph/TargetRendererImplTank.h>

REGISTER_ACTION_SOURCE(ShieldHit);

ShieldHit::ShieldHit() : 
	totalTime_(0.0f),
	firstTime_(true)
{
}

ShieldHit::ShieldHit(unsigned int playerId,
	Vector &position,
	float hitPercentage) :
	totalTime_(0.0f),
	firstTime_(true), playerId_(playerId),
	hitPercentage_(hitPercentage),
	position_(position)
{
}

ShieldHit::~ShieldHit()
{
}

void ShieldHit::init()
{
}

void ShieldHit::simulate(float frameTime, bool &remove)
{
	if (firstTime_)
	{
		firstTime_ = false;

		Target *target = 
			context_->targetContainer->getTargetById(playerId_);
		if (target)
		{
			Accessory *accessory = 
				target->getShield().getCurrentShield();
			if (accessory)
			{
				Shield *shield = (Shield *) accessory->getAction();
#ifndef S3D_SERVER
				if (!context_->serverMode) 
				{
					SoundBuffer *shieldSound = 
						Sound::instance()->fetchOrCreateBuffer( (char *)
							getDataFile(formatString("data/wav/%s", shield->getCollisionSound())));
					SoundUtils::playAbsoluteSound(VirtualSoundPriority::eAction,
						shieldSound, position_);

					TargetRenderer *renderer = target->getRenderer();
					if (renderer)
					{
						renderer->shieldHit();
					}
				}
#endif // #ifndef S3D_SERVER

				target->getShield().setShieldPower(
					target->getShield().getShieldPower() -
					shield->getHitRemovePower() * hitPercentage_);
			}
		}
	}

	totalTime_ += frameTime;
	if (totalTime_ > 0.2f) remove = true;
	Action::simulate(frameTime, remove);
}

void ShieldHit::draw()
{
#ifndef S3D_SERVER
	if (!context_->serverMode)
	{
		GLLenseFlare::instance()->draw(position_, false, 0, 
			1.0f, 1.0f);
	}
#endif // #ifndef S3D_SERVER
}

bool ShieldHit::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(hitPercentage_);
	buffer.addToBuffer(position_);
	return true;
}

bool ShieldHit::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(hitPercentage_)) return false;
	if (!reader.getFromBuffer(position_)) return false;
	return true;
}

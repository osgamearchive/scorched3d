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
#include <sound/Sound.h>
#include <common/OptionsParam.h>
#include <engine/ScorchedContext.h>
#include <weapons/Accessory.h>
#include <weapons/Shield.h>
#include <tank/TankContainer.h>
#include <tankgraph/TankModelRenderer.h>

REGISTER_ACTION_SOURCE(ShieldHit);

ShieldHit::ShieldHit() : firstTime_(true)
{
}

ShieldHit::ShieldHit(unsigned int playerId,
	Vector &position,
	float hitPercentage) :
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

		Tank *tank = 
			context_->tankContainer->getTankById(playerId_);
		if (tank)
		{
			Accessory *accessory = 
				tank->getAccessories().getShields().getCurrentShield();
			if (accessory)
			{
				Shield *shield = (Shield *) accessory->getAction();
				if (!context_->serverMode) 
				{
					SoundBuffer *shieldSound = 
						Sound::instance()->fetchOrCreateBuffer( (char *)
							getDataFile( "data/wav/%s", shield->getCollisionSound()));
					SoundSource *source = Sound::instance()->createSource();
					source->setPosition(position_);
					source->play(shieldSound);
					Sound::instance()->manageSource(source);

					TankModelRenderer *model = (TankModelRenderer *) 
						tank->getModel().getModelIdRenderer();
					if (model)
					{
						model->shieldHit();
					}
				}

				tank->getAccessories().getShields().setShieldPower(
					tank->getAccessories().getShields().getShieldPower() -
					shield->getHitRemovePower() * hitPercentage_);
			}
		}
	}

	if (!renderer_) remove = true;
	Action::simulate(frameTime, remove);
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

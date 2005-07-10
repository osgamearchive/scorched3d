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

#include <actions/Teleport.h>
#include <actions/CameraPositionAction.h>
#include <sprites/WallActionRenderer.h>
#include <common/OptionsParam.h>
#include <tank/TankContainer.h>
#include <engine/ActionController.h>
#include <engine/ScorchedContext.h>
#include <weapons/AccessoryStore.h>
#include <landscape/DeformLandscape.h>
#include <landscape/LandscapeMaps.h>
#include <sound/SoundUtils.h>

REGISTER_ACTION_SOURCE(Teleport);

Teleport::Teleport() : 
	totalTime_(0.0f),
	firstTime_(true),
	vPoint_(0)
{
}

Teleport::Teleport(Vector position,
		unsigned int playerId,
		WeaponTeleport *weapon) :
	position_(position), 
	playerId_(playerId),
	weapon_(weapon),
	totalTime_(0.0f),
	firstTime_(true),
	vPoint_(0)
{

}

Teleport::~Teleport()
{
	if (vPoint_) context_->viewPoints->releaseViewPoint(vPoint_);
}

void Teleport::init()
{
	vPoint_ = context_->viewPoints->getNewViewPoint(playerId_);
}

void Teleport::simulate(float frameTime, bool &remove)
{
	if (vPoint_) vPoint_->setPosition(position_);

	if (firstTime_)
	{
		firstTime_ = false;

		if (!context_->serverMode)
		{
			Tank *tank = context_->tankContainer->getTankById(playerId_);
			if (tank && tank->getState().getState() == TankState::sNormal)
			{
				SoundBuffer *activateSound = 
					Sound::instance()->fetchOrCreateBuffer((char *)
						getDataFile(weapon_->getSound()));
				SoundUtils::playAbsoluteSound(VirtualSoundPriority::eAction,
					activateSound, tank->getPhysics().getTankPosition());
			}
		}
	}

	totalTime_ += frameTime;
	if (totalTime_ > weapon_->getDelay())
	{
		Tank *tank = context_->tankContainer->getTankById(playerId_);
		if (tank && tank->getState().getState() == TankState::sNormal)
		{
			float height = context_->landscapeMaps->getHMap().getInterpHeight(
				position_[0], position_[1]);
			position_[2] = height;

			tank->getPhysics().setTankPosition(position_);
			DeformLandscape::flattenArea(*context_, position_, 0);
		}

		remove = true;
	}

	Action::simulate(frameTime, remove);
}

bool Teleport::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(position_);
	buffer.addToBuffer(playerId_);
	context_->accessoryStore->writeWeapon(buffer, weapon_);
	return true;
}

bool Teleport::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(position_)) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	weapon_ = (WeaponTeleport *) context_->accessoryStore->readWeapon(reader); if (!weapon_) return false;

	Tank *movedTank = context_->tankContainer->getTankById(playerId_);
	if (movedTank)
	{
		if (movedTank->getState().getState() == TankState::sNormal)
		{
			const float ShowTime = 5.0f;
			ActionMeta *pos = new CameraPositionAction(
				position_, ShowTime, 5);
			context_->actionController->getBuffer().clientAdd(-4.0f, pos);
		}
	}

	return true;
}

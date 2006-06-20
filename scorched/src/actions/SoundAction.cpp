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

#include <actions/SoundAction.h>
#include <weapons/WeaponSound.h>
#include <weapons/AccessoryStore.h>
#include <sound/SoundUtils.h>
#include <common/Defines.h>

REGISTER_ACTION_SOURCE(SoundAction);

SoundAction::SoundAction(Vector &position, WeaponSound *weapon) :
	weapon_(weapon), position_(position)
{

}

SoundAction::~SoundAction()
{
}

void SoundAction::init()
{
}

void SoundAction::simulate(float frameTime, bool &remove)
{
	if (!context_->serverMode)
	{
		SoundBuffer *activateSound = 
			Sound::instance()->fetchOrCreateBuffer((char *)
				getDataFile(weapon_->getSound()));

		VirtualSoundSource *source = new VirtualSoundSource(
			VirtualSoundPriority::eAction, false, true);
		if (weapon_->getRelative())	source->setRelative();
		else source->setPosition(position_);
		source->setGain(weapon_->getGain());
		source->setReferenceDistance(weapon_->getReferenceDistance());
		source->setRolloff(weapon_->getRolloff());
		source->play(activateSound);
	}

	remove = true;
	Action::simulate(frameTime, remove);
}

bool SoundAction::writeAction(NetBuffer &buffer)
{
	context_->accessoryStore->writeWeapon(buffer, weapon_);
	buffer.addToBuffer(position_);
	return true;
}

bool SoundAction::readAction(NetBufferReader &reader)
{
	weapon_ = (WeaponSound *) context_->
		accessoryStore->readWeapon(reader); if (!weapon_) return false;
	if (!reader.getFromBuffer(position_)) return false;
	return true;
}

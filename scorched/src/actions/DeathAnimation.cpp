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

#include <actions/DeathAnimation.h>
#include <engine/ScorchedContext.h>
#include <sprites/ExplosionLaserBeamRenderer.h>

REGISTER_ACTION_SOURCE(DeathAnimation);

DeathAnimation::DeathAnimation()
{
}

DeathAnimation::DeathAnimation(unsigned int playerId,
		Vector &position, 
		Vector &velocity,
		const char *rendererName) : 
	position_(position), playerId_(playerId), velocity_(velocity),
	rendererName_(rendererName)
{
}

DeathAnimation::~DeathAnimation()
{
}

void DeathAnimation::init()
{
	if (!context_->serverMode)
	{
		if (strcmp(rendererName_.c_str(), "ExplosionLaserBeamRenderer"))
		{
			setActionRender(
				new ExplosionLaserBeamRenderer(position_, 4.0f));
		}
		else
		{
			dialogMessage("DeathAnimation",
						  "No renderer named \"%s\"",
						  rendererName_.c_str());
		}
	}
}

void DeathAnimation::simulate(float frameTime, bool &removeAction)
{
	ActionMeta::simulate(frameTime, removeAction);
	if (!renderer_) removeAction = true;
}

bool DeathAnimation::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(position_[0]);
	buffer.addToBuffer(position_[1]);
	buffer.addToBuffer(position_[2]);
	buffer.addToBuffer(velocity_[0]);
	buffer.addToBuffer(velocity_[1]);
	buffer.addToBuffer(velocity_[2]);
	buffer.addToBuffer(rendererName_);
	return true;
}

bool DeathAnimation::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(position_[0])) return false;
	if (!reader.getFromBuffer(position_[1])) return false;
	if (!reader.getFromBuffer(position_[2])) return false;
	if (!reader.getFromBuffer(velocity_[0])) return false;
	if (!reader.getFromBuffer(velocity_[1])) return false;
	if (!reader.getFromBuffer(velocity_[2])) return false;
	if (!reader.getFromBuffer(rendererName_)) return false;
	return true;
}

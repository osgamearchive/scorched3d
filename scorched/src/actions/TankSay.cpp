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

#include <actions/TankSay.h>
#include <sprites/TalkRenderer.h>
#include <sprites/ExplosionTextures.h>
#include <tank/TankContainer.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <common/Defines.h>
#include <common/OptionsParam.h>
#include <common/Logger.h>

REGISTER_ACTION_SOURCE(TankSay);

TankSay::TankSay()
{
}

TankSay::TankSay(unsigned int playerId,
		const char *text) :
	playerId_(playerId), text_(text)
{

}

TankSay::~TankSay()
{
}

void TankSay::init()
{
	Tank *tank = 
		context_->tankContainer->getTankById(playerId_);
	if (tank)
	{
		if (!context_->serverMode) 
		{
			// put a speach bubble over the talking tank
			Vector white(1.0f, 1.0f, 1.0f);
			TalkRenderer *talk = new TalkRenderer(
				tank->getPhysics().getTankTurretPosition(),
				white);
			context_->actionController->addAction(new SpriteAction(talk));
		}

		if (!context_->serverMode)
		{
			Logger::log(
				LoggerInfo(tank->getPlayerId(),
					tank->getAvatar().getTexture()), 
				text_.c_str());
		}
		else if (OptionsParam::instance()->getDedicatedServer())
		{
			Logger::log(tank->getPlayerId(), text_.c_str());
		}
	}
}

void TankSay::simulate(float frameTime, bool &remove)
{
	remove = true;
	Action::simulate(frameTime, remove);
}

bool TankSay::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(text_);
	return true;
}

bool TankSay::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(text_)) return false;
	return true;
}

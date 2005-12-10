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
#include <common/LoggerI.h>

REGISTER_ACTION_SOURCE(TankSay);

TankSay::TankSay()
{
}

TankSay::TankSay(unsigned int playerId,
		const char *text,
		unsigned int infoLen) :
	playerId_(playerId), text_(text), infoLen_(infoLen)
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
				tank->getPosition().getTankTurretPosition(),
				white);
			context_->actionController->addAction(new SpriteAction(talk));

			LoggerInfo info(LoggerInfo::TypeTalk, text_.c_str());
			info.setPlayerId(tank->getPlayerId());
			info.setIcon(tank->getAvatar().getTexture());
			info.setInfoLen(infoLen_);
			Logger::log(info);
		}
		else if (OptionsParam::instance()->getDedicatedServer())
		{
			Logger::log(text_.c_str());
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
	buffer.addToBuffer(infoLen_);
	return true;
}

bool TankSay::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(text_)) return false;
	if (!reader.getFromBuffer(infoLen_)) return false;
	return true;
}

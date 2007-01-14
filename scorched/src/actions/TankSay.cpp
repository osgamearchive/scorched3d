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
#include <tank/TankPosition.h>
#include <tank/TankAvatar.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <common/LoggerI.h>

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
#ifndef S3D_SERVER
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
#endif // #ifndef S3D_SERVER
	}
}

void TankSay::simulate(float frameTime, bool &remove)
{
	remove = true;
	Action::simulate(frameTime, remove);
}

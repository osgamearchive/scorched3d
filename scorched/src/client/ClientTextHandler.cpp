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

#include <client/ClientTextHandler.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <client/MessageDisplay.h>
#include <sprites/TalkRenderer.h>
#include <coms/ComsTextMessage.h>
#include <common/SoundStore.h>
#include <common/Logger.h>

ClientTextHandler *ClientTextHandler::instance_ = 0;

ClientTextHandler *ClientTextHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ClientTextHandler;
	}
	return instance_;
}

ClientTextHandler::ClientTextHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsTextMessage",
		this);
}

ClientTextHandler::~ClientTextHandler()
{
}

bool ClientTextHandler::processMessage(unsigned int id,
	const char *messageType,
	NetBufferReader &reader)
{
	ComsTextMessage message;
	if (!message.readMessage(reader)) return false;

	if (message.getShowAsMessage())
	{
		MessageDisplay::instance()->addMessage(message.getText());
	}
	else
	{
		if (ScorchedClient::instance()->getGameState().getState() == 
			ClientState::StateConnect)
		{
			Logger::log(0, message.getText());
		}
		else
		{
			CACHE_SOUND(sound,  PKGDIR "data/wav/misc/text.wav");
			sound->play();

			Tank *tank = 
				ScorchedClient::instance()->getTankContainer().getTankById(message.getPlayerId());
			if (tank && tank->getState().getState() == TankState::sNormal)
			{
				// put a speach bubble over the talking tank
				TalkRenderer *talk = new TalkRenderer(
					tank->getPhysics().getTankTurretPosition());
				ScorchedClient::instance()->getActionController().addAction(new SpriteAction(talk));
			}

			Logger::log(message.getPlayerId(), message.getText());
		}
	}

	return true;
}

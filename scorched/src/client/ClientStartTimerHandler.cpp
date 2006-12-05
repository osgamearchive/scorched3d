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

#include <client/ClientStartTimerHandler.h>
#include <client/ScorchedClient.h>
#include <graph/ShotCountDown.h>
#include <coms/ComsTimerStartMessage.h>

ClientStartTimerHandler *ClientStartTimerHandler::instance_ = 0;

ClientStartTimerHandler *ClientStartTimerHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ClientStartTimerHandler;
	}
	return instance_;
}

ClientStartTimerHandler::ClientStartTimerHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsTimerStartMessage",
		this);
}

ClientStartTimerHandler::~ClientStartTimerHandler()
{
}

bool ClientStartTimerHandler::processMessage(
	NetMessage &netMessage,
	const char *messageType,
	NetBufferReader &reader)
{
	ComsTimerStartMessage message;
	if (!message.readMessage(reader)) return false;

	ShotCountDown::instance()->reset((float) message.getTimerValue());
	
	return true;
}

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


#include <client/ClientMessageHandler.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <common/Logger.h>

ClientMessageHandler *ClientMessageHandler::instance_ = 0;

ClientMessageHandler *ClientMessageHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ClientMessageHandler;
	}
	return instance_;
}

ClientMessageHandler::ClientMessageHandler()
{
}

ClientMessageHandler::~ClientMessageHandler()
{
}

void ClientMessageHandler::clientConnected(NetMessage &message)
{
	Logger::log(0, "Connected");
}

void ClientMessageHandler::clientDisconnected(NetMessage &message)
{
	Logger::log(0, "Disconnected");
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimDisconnected);
}

void ClientMessageHandler::clientError(NetMessage &message,
		const char *errorString)
{
	Logger::log(0, "***Client Error*** \"%s\"", errorString);
}

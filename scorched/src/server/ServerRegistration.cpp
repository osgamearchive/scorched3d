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

#include <server/ServerRegistration.h>
#include <server/ScorchedServer.h>
#include <common/Logger.h>
#include <common/OptionsGame.h>
#include <time.h>

ServerRegistration *ServerRegistration::instance_ = 0;

ServerRegistration *ServerRegistration::instance()
{
	if (!instance_)
	{
		instance_ = new ServerRegistration();
	}
	return instance_;
}

ServerRegistration::ServerRegistration() : 
	netServer_(new NetServerHTTPProtocol)
{
	static char buffer[1024];
	sprintf(buffer, 
		"GET %s/servers.php?register=%s&port=%i HTTP/1.0\r\n"
		"User-Agent: Scorched3D\r\n"
		"Host: %s\r\n"
		"Connection: Keep-Alive\r\n"
		"\r\n"
		"\r\n",
		ScorchedServer::instance()->getOptionsGame().getMasterListServerURI(),
		ScorchedServer::instance()->getOptionsGame().getPublishAddress(),
		ScorchedServer::instance()->getOptionsGame().getPortNo(),
		ScorchedServer::instance()->getOptionsGame().getMasterListServer());
	sendNetBuffer_.addDataToBuffer(buffer, strlen(buffer)); // Note no null

	netServer_.setMessageHandler(this);
}

ServerRegistration::~ServerRegistration()
{
}

void ServerRegistration::start()
{
	SDL_CreateThread(ServerRegistration::threadFunc, (void *) this);
}

int ServerRegistration::threadFunc(void *param)
{
	const int TimeBetweenRegistrations = 540;

	for (;;)
	{
		// Ensure no connections are hanging around
		instance_->netServer_.disconnectAllClients();

		// Register this game on the web
		registerGame();
		time_t lastTime = time(0);

		// Wait for TimeBetweenRegistrations seconds before registering again
		int waitTime = TimeBetweenRegistrations;
		for (;;)
		{
			// Check for any replies or timeout every 1 seconds
			SDL_Delay(1000);
			instance_->netServer_.processMessages();

			// If we have finished and it was not a success only
			// wait 30 seconds before registering again
			if (instance_->finished_ && !instance_->success_)
			{
				waitTime = 30;
			}

			// Wait for TimeBetweenRegistrations seconds before sending again
			time_t currentTime = time(0);
			if (currentTime - lastTime > waitTime)
			{
				break;
			}
		}
	}
	return 0;
}

void ServerRegistration::registerGame()
{
	// Connect to the web server
	Logger::log( "Contacting registration server...");
	if (!instance_->netServer_.connect(ScorchedServer::instance()->getOptionsGame().getMasterListServer(), 80))
	{
		Logger::log( "Failed to contact registration server");
		instance_->finished_ = true;
		instance_->success_ = false;
		return;
	}
	instance_->finished_ = false;
	instance_->success_ = false;

	// Send the web request
	instance_->netServer_.sendMessage(instance_->sendNetBuffer_);
	Logger::log( "Registration request sent.");
}

void ServerRegistration::processMessage(NetMessage &message)
{
	// We have received a reply from the web server
	if (message.getMessageType() == NetMessage::BufferMessage)
	{
		message.getBuffer().getBuffer()[message.getBuffer().getBufferUsed()] = '\0';
		success_ = (strstr(message.getBuffer().getBuffer(), "success") != 0);
	}
	else if (message.getMessageType() == NetMessage::DisconnectMessage)
	{
		Logger::log( "Registration complete (%s).", (success_?"Success":"Failure"));
		finished_ = true;
	}
}


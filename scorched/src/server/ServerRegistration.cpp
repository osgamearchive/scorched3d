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

#include <time.h>
#include <server/ServerRegistration.h>
#include <server/ScorchedServer.h>
#include <common/Logger.h>
#include <common/OptionsMasterListServer.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>

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
	mainServer_(
		OptionsMasterListServer::instance()->getMasterListServer(),
		OptionsMasterListServer::instance()->getMasterListServerURI()),
	backupServer_(
		OptionsMasterListServer::instance()->getMasterListBackupServer(),
		OptionsMasterListServer::instance()->getMasterListBackupServerURI())
{
}

ServerRegistration::~ServerRegistration()
{
}

void ServerRegistration::start()
{
	mainServer_.start();
	backupServer_.start();
}

ServerRegistrationEntry::ServerRegistrationEntry(
	const char *masterListServer, 
	const char *masterListServerURI) : 
	netServer_(new NetServerHTTPProtocolSend),
	masterListServer_(masterListServer)
{
	static char buffer[1024];
	snprintf(buffer, 
		1024,
		"GET %s/servers.php?register=%s&port=%i HTTP/1.0\r\n"
		"User-Agent: Scorched3D\r\n"
		"Host: %s\r\n"
		"Connection: Keep-Alive\r\n"
		"\r\n"
		"\r\n",
		masterListServerURI,
		ScorchedServer::instance()->getOptionsGame().getPublishAddress(),
		ScorchedServer::instance()->getOptionsGame().getPortNo(),
		masterListServer);
	sendNetBuffer_.addDataToBuffer(buffer, strlen(buffer)); // Note no null

	netServer_.setMessageHandler(this);
}

ServerRegistrationEntry::~ServerRegistrationEntry()
{
}

void ServerRegistrationEntry::start()
{
	SDL_CreateThread(ServerRegistrationEntry::threadFunc, (void *) this);
}

int ServerRegistrationEntry::threadFunc(void *param)
{
	ServerRegistrationEntry *entry = 
		(ServerRegistrationEntry *) param;

	entry->actualThreadFunc();
	return 0;
}

void ServerRegistrationEntry::actualThreadFunc()
{
	const int TimeBetweenRegistrations = 540;

	for (;;)
	{
		// Ensure no connections are hanging around
		netServer_.disconnectAllClients();

		// Register this game on the web
		registerGame();
		time_t lastTime = time(0);

		// Wait for TimeBetweenRegistrations seconds before registering again
		int waitTime = TimeBetweenRegistrations;
		for (;;)
		{
			// Check for any replies or timeout every 1 seconds
			SDL_Delay(1000);
			netServer_.processMessages();

			// If we have finished and it was not a success only
			// wait 30 seconds before registering again
			if (finished_ && !success_)
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
}

void ServerRegistrationEntry::registerGame()
{
	// Connect to the web server
	Logger::log(formatString(
		"Connecting to registration server %s...", masterListServer_));
	if (!netServer_.connect(masterListServer_, 80))
	{
		Logger::log(formatString(
			"Failed to connect to registration server %s", masterListServer_));
		finished_ = true;
		success_ = false;
		return;
	}
	finished_ = false;
	success_ = false;

	// Send the web request
	netServer_.sendMessage(sendNetBuffer_);
}

void ServerRegistrationEntry::processMessage(NetMessage &message)
{
	// We have received a reply from the web server
	if (message.getMessageType() == NetMessage::BufferMessage)
	{
		message.getBuffer().addToBuffer("");
		success_ = (strstr(message.getBuffer().getBuffer(), "success") != 0);
	}
	else if (message.getMessageType() == NetMessage::DisconnectMessage)
	{
		Logger::log(
			formatString("Registration to %s %s.", 
			masterListServer_,
			(success_?"was successfull":"failed")));
		finished_ = true;
	}
}


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

#include <dialogs/ConnectDialog.h>
#include <dialogs/PlayerDialog.h>
#include <dialogs/LogDialog.h>
#include <client/ScorchedClient.h>
#include <tank/TankContainer.h>
#include <server/ScorchedServer.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>
#include <coms/ComsMessageHandler.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsConnectMessage.h>
#include <coms/NetInterface.h>

ConnectDialog *ConnectDialog::instance_ = 0;

ConnectDialog *ConnectDialog::instance()
{
	if (!instance_)
	{
		instance_ = new ConnectDialog;
	}
	return instance_;
}

ConnectDialog::ConnectDialog() : 
	GLWWindow("Connect", -100.0f, 10.0f, 20.0f, 20.0f, eNoDraw | eNoTitle,
		"Connection dialog"),
	connectionState_(eWaiting),
	tryCount_(0), lastTime_(0)
{
	// Get the unique id
	if (!idStore_.loadStore())
	{
		LoggerInfo info (LoggerInfo::TypeNormal, 
			"Failed to load id store", "");
		LogDialog::instance()->logMessage(info);
	}
}

ConnectDialog::~ConnectDialog()
{
}

void ConnectDialog::simulate(float frameTime)
{
	time_t currentTime = time(0);
	if (connectionState_ == eWaiting)
	{
		if (currentTime - lastTime_ > 3)
		{
			if (tryCount_<3)
			{
				lastTime_ = currentTime;
				tryConnection();
			}
		}
	}
	else if (connectionState_ == eFinishedTryingConnection)
	{
		finishedTryingConnection();
	}
}

void ConnectDialog::tryConnection()
{
	const char *serverName = 
		(OptionsParam::instance()->getConnect()[0]?
		OptionsParam::instance()->getConnect():
		"Localhost");

	std::string hostPart = serverName;
	char *colon = strchr((char *)serverName, ':');
	int port = ScorchedPort;
	if (colon) 
	{
		char *stop;
		*colon = '\0';
		colon++;
		port = strtol(colon, &stop, 10);
		hostPart = serverName;
		colon--;
		*colon = ':';
	}	

	LogDialog::instance()->setServerName(
		formatString("Connecting to : %s", serverName));

	{
		LoggerInfo info (LoggerInfo::TypeNormal, 
			"Attempting connection", "");
		LogDialog::instance()->logMessage(info);
	}
	{
		LoggerInfo info (LoggerInfo::TypeNormal, 
			formatString("  Trying \"%s\"....", 
			serverName[0]?serverName:"Loopback"), "");
		LogDialog::instance()->logMessage(info);
	}

	connectionState_ = eTryingConnection;
	if (OptionsParam::instance()->getConnectedToServer())
	{
		// This point should only ever be called by one thread
		// so cheat
		static ThreadParams params;
		params.host = hostPart;
		params.port = port;
		params.dialog = this;

		SDL_CreateThread(ConnectDialog::tryRemoteConnection, &params);
	}
	else
	{
		tryLocalConnection();
	}
}

int ConnectDialog::tryRemoteConnection(void *inParams)
{
	ThreadParams *params = (ThreadParams *) inParams;
	char *host = (char *) params->host.c_str();
	int port = params->port;
	ConnectDialog *dialog = params->dialog;

	if (ScorchedClient::instance()->getNetInterface().
		connect(host, port))
	{
		IPaddress address;
		if (SDLNet_ResolveHost(&address, host, 0) == 0)
		{
			unsigned int ipAddress = SDLNet_Read32(&address.host);
			dialog->uniqueId_ = dialog->idStore_.getUniqueId(ipAddress);
		}
	}

	dialog->connectionState_ = eFinishedTryingConnection;
	return 0;
}

void ConnectDialog::tryLocalConnection()
{
	connectionState_ = eFinishedTryingConnection;
}

void ConnectDialog::finishedTryingConnection()
{
	// Check to see if the connection attempt was successful
	if (!ScorchedClient::instance()->getNetInterface().started())
	{
		LoggerInfo info (LoggerInfo::TypeNormal, 
			"  Connection Failed.", "");
		LogDialog::instance()->logMessage(info);

		if (++tryCount_>2)
		{
			LoggerInfo info (LoggerInfo::TypeNormal, 
				"Could not connect to server.", "");
			LogDialog::instance()->logMessage(info);

			connectionState_ = eFinished;
		}
		else
		{
			connectionState_ = eWaiting;
		}

		return;
	}

	// Check the number of players that are connecting
	unsigned int noPlayers = 1;
	if (!OptionsParam::instance()->getConnectedToServer())
	{
		noPlayers = ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers() -
			ScorchedServer::instance()->getTankContainer().getNoOfTanks();
	}

	// If we connected then send our details to the server
	ComsConnectMessage connectMessage;
	connectMessage.setVersion(ScorchedVersion);
	connectMessage.setProtocolVersion(ScorchedProtocolVersion);
	connectMessage.setUserName(OptionsParam::instance()->getUserName());
	connectMessage.setPassword(OptionsParam::instance()->getPassword());
	connectMessage.setUniqueId(uniqueId_.c_str());
	connectMessage.setHostDesc(OptionsDisplay::instance()->getHostDescription());
	connectMessage.setNoPlayers(noPlayers);
	if (!ComsMessageSender::sendToServer(connectMessage))
	{
		LoggerInfo info (LoggerInfo::TypeNormal, 
			"  Connection Send Failed!", "");
		LogDialog::instance()->logMessage(info);
	}

	connectionState_ = eFinished;
}


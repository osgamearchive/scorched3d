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
#include <common/Logger.h>
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
	tryCount_(0), lastTime_(0), idStore_(0)
{
}

ConnectDialog::~ConnectDialog()
{
}

UniqueIdStore &ConnectDialog::getIdStore()
{
	if (!idStore_)
	{
		idStore_ = new UniqueIdStore();
		// Get the unique id
		if (!idStore_->loadStore())
		{
			LoggerInfo info (LoggerInfo::TypeNormal, 
				"Failed to load id store", "");
			LogDialog::instance()->logMessage(info);
		}
	}
	return *idStore_;
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
				tryCount_++;
				lastTime_ = currentTime;
				tryConnection();
			}
			else
			{
				connectionState_ = eFinished;
				Logger::log("Connection Failed");
			}
		}
	}
}

void ConnectDialog::tryConnection()
{
	const char *serverName = 
		(OptionsParam::instance()->getConnect()[0]?
		OptionsParam::instance()->getConnect():
		"Localhost");

	host_ = serverName;
	port_ = ScorchedPort;

	char *colon = strchr((char *)serverName, ':');
	if (colon) 
	{
		char *stop;
		*colon = '\0';
		colon++;
		port_ = strtol(colon, &stop, 10);
		host_ = serverName;
		colon--;
		*colon = ':';
	}	

	LogDialog::instance()->setServerName(
		formatString("Connecting to : %s", serverName));
	Logger::log("Attempting connection");
	Logger::log(formatString("  Trying \"%s\"....", 
		serverName[0]?serverName:"Loopback"));

	connectionState_ = eTryingConnection;
	if (OptionsParam::instance()->getConnectedToServer())
	{
		// Do in a thread so connect can block if it wants!
		SDL_CreateThread(ConnectDialog::tryRemoteConnection, 0);
	}
	else
	{
		// Or connect localy
		tryLocalConnection();
	}
}

int ConnectDialog::tryRemoteConnection(void *)
{
	char *host = (char *) instance_->host_.c_str();
	int port = instance_->port_;

	// Try to connect to the server
	ScorchedClient::instance()->getNetInterface().connect(host, port);

	// Wait for result
	instance_->connectionState_ = eWaiting;
	return 0;
}

void ConnectDialog::tryLocalConnection()
{
	// Try to connect localy
	ScorchedClient::instance()->getNetInterface().connect("Local", 0);

	// Wait for result
	connectionState_ = eWaiting;;
}

void ConnectDialog::connected()
{
	Logger::log(formatString("Connected."));

	// Update unique id store
	if (OptionsParam::instance()->getConnectedToServer())
	{
		IPaddress address;
		if (SDLNet_ResolveHost(&address, (char *) host_.c_str(), 0) == 0)
		{
			unsigned int ipAddress = SDLNet_Read32(&address.host);
			uniqueId_ = getIdStore().getUniqueId(ipAddress);
		}
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
		Logger::log("  Connection Send Failed!");
	}

	connectionState_ = eFinished;
}


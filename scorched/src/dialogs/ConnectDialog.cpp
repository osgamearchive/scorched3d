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
#include <dialogs/ProgressDialog.h>
#include <dialogs/MsgBoxDialog.h>
#include <client/ScorchedClient.h>
#include <client/ClientState.h>
#include <tank/TankContainer.h>
#include <server/ScorchedServer.h>
#include <graph/OptionsDisplay.h>
#include <client/ClientParams.h>
#include <common/OptionsScorched.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <coms/ComsMessageHandler.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsConnectMessage.h>
#include <net/NetInterface.h>

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
			Logger::log("Failed to load id store");
		}
	}
	return *idStore_;
}

void ConnectDialog::windowInit(const unsigned state)
{
	connectionState_ = eWaiting;
	tryCount_ = 0;
	lastTime_ = 0;
}

void ConnectDialog::simulate(float frameTime)
{
	time_t currentTime = time(0);
	if (connectionState_ == eWaiting)
	{
		if (currentTime - lastTime_ > 3)
		{
			ScorchedClient::instance()->getNetInterface().stop();

			if (tryCount_<3)
			{
				tryCount_++;
				lastTime_ = currentTime;
				tryConnection();
			}
			else
			{
				connectionState_ = eFinished;

				const char *msg = 
					formatString("Failed to connect to server \"%s:%i\", timeout.",
						host_.c_str(), port_);
				Logger::log(msg);
				MsgBoxDialog::instance()->show(msg);

				ScorchedClient::instance()->getGameState().stimulate(
					ClientState::StimOptions);
			}
		}
	}
}

void ConnectDialog::tryConnection()
{
	const char *serverName = 
		(ClientParams::instance()->getConnect()[0]?
		ClientParams::instance()->getConnect():
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

	ProgressDialog::instance()->progressChange(
		formatString("Connecting to \"%s:%i\" (%i)....", 
			host_.c_str(), port_, tryCount_), 0);

	connectionState_ = eTryingConnection;
	if (ClientParams::instance()->getConnectedToServer())
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
	ProgressDialog::instance()->progressChange("Connected", 100);

	// Update unique id store
	if (ClientParams::instance()->getConnectedToServer())
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
	if (!ClientParams::instance()->getConnectedToServer())
	{
		noPlayers = ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers() -
			ScorchedServer::instance()->getTankContainer().getNoOfTanks();
	}

	// If we connected then send our details to the server
	ComsConnectMessage connectMessage;
	connectMessage.setVersion(ScorchedVersion);
	connectMessage.setProtocolVersion(ScorchedProtocolVersion);
	connectMessage.setUserName(ClientParams::instance()->getUserName());
	connectMessage.setPassword(ClientParams::instance()->getPassword());
	connectMessage.setUniqueId(uniqueId_.c_str());
	connectMessage.setHostDesc(OptionsDisplay::instance()->getHostDescription());
	connectMessage.setNoPlayers(noPlayers);
	if (!ComsMessageSender::sendToServer(connectMessage))
	{
		ScorchedClient::instance()->getNetInterface().stop();

		const char *msg = 
			formatString("Failed to connect to server \"%s:%i\", send failed.",
				host_.c_str(), port_);
		Logger::log(msg);
		MsgBoxDialog::instance()->show(msg);

		ScorchedClient::instance()->getGameState().stimulate(
			ClientState::StimOptions);
	}

	connectionState_ = eFinished;
}


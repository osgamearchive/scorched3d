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
#include <engine/MainLoop.h>
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
	tryConnection_(true)
{
}

ConnectDialog::~ConnectDialog()
{
}

void ConnectDialog::simulate(float frameTime)
{
	if (tryConnection_)
	{
		tryConnection_ = false;

		int tryCount = 0;
		while (!tryConnection())
		{
			if (++tryCount>2)
			{
				LoggerInfo info (LoggerInfo::TypeNormal, 
					"Could not connect to server.", "");
				LogDialog::instance()->logMessage(info);
				break;
			}
		}
	}
}

bool ConnectDialog::tryConnection()
{
	const char *serverName = 
		(OptionsParam::instance()->getConnect()[0]?
		OptionsParam::instance()->getConnect():
		"Localhost");
	unsigned int noPlayers = 1;
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

	ScorchedClient::instance()->getMainLoop().draw();
	ScorchedClient::instance()->getMainLoop().swapBuffers();

	const char *uniqueId = "";
	if (OptionsParam::instance()->getConnectedToServer())
	{
		std::string hostPart;
		hostPart = serverName;
		char *colon = strchr((char *)serverName, ':');
		DWORD port = ScorchedPort;
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

		// Try to connect to the server
		if (!ScorchedClient::instance()->getNetInterface().
			connect((char *) hostPart.c_str(), port))
		{
			{
				LoggerInfo info (LoggerInfo::TypeNormal, 
					"  Connection Failed.", "");
				LogDialog::instance()->logMessage(info);
			}

			ScorchedClient::instance()->getMainLoop().draw();
			ScorchedClient::instance()->getMainLoop().swapBuffers();

			SDL_Delay(3 * 1000);
			return false;
		}

		// Get the unique id
		if (!idStore_.loadStore())
		{
			LoggerInfo info (LoggerInfo::TypeNormal, 
				"Failed to load id store", "");
			LogDialog::instance()->logMessage(info);
			return false;
		}
		IPaddress address;
		if (SDLNet_ResolveHost(&address, (char *) hostPart.c_str(), 0) != 0)
		{
			LoggerInfo info (LoggerInfo::TypeNormal, 
				formatString("Failed to resolve server name %s", 
				hostPart.c_str()), "");
			LogDialog::instance()->logMessage(info);
			return false;
		}
		unsigned int ipAddress = SDLNet_Read32(&address.host);
		uniqueId = idStore_.getUniqueId(ipAddress);
	}
	else
	{
		noPlayers = ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers() -
			ScorchedServer::instance()->getTankContainer().getNoOfTanks();
	}

	// If we connected then send our details to the server
	ComsConnectMessage connectMessage;
	connectMessage.setVersion(ScorchedVersion);
	connectMessage.setProtocolVersion(ScorchedProtocolVersion);
	connectMessage.setPassword(OptionsParam::instance()->getPassword());
	connectMessage.setUniqueId(uniqueId);
	connectMessage.setHostDesc(OptionsDisplay::instance()->getHostDescription());
	connectMessage.setNoPlayers(noPlayers);
	if (!ComsMessageSender::sendToServer(connectMessage))
	{
		LoggerInfo info (LoggerInfo::TypeNormal, 
			"  Connection Send Failed!", "");
		LogDialog::instance()->logMessage(info);
	}

	return true;
}


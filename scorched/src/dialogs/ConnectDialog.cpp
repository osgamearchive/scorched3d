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
#include <client/MainBanner.h>
#include <server/ScorchedServer.h>
#include <common/OptionsDisplay.h>
#include <coms/NetServer.h>
#include <coms/ComsMessageHandler.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsConnectMessage.h>
#include <common/WindowManager.h>
#include <common/OptionsParam.h>

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
	GLWWindow("Connect", 10.0f, 10.0f, 300.0f, 240.0f, 0),
	tryConnection_(true)
{
}

ConnectDialog::~ConnectDialog()
{
}

void ConnectDialog::draw()
{
	if (tryConnection_)
	{
		tryConnection_ = false;

		int tryCount = 0;
		while (!tryConnection())
		{
			if (++tryCount>2)
			{
				Logger::log(0, "Could not connect to server.");
				break;
			}
		}
	}
}

bool ConnectDialog::tryConnection()
{
	const char *serverName = 
		OptionsParam::instance()->getConnect();
	unsigned int noPlayers = 1;
	LogDialog::instance()->setServerName(serverName);

	Logger::log(0, "Atempting connection");
	Logger::log(0, "  Trying \"%s\"....", serverName);
	ScorchedClient::instance()->getMainLoop().draw();

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
		NetServer &netServer = (NetServer &) ScorchedClient::instance()->getNetInterface();
		if (!netServer.connect((char *) hostPart.c_str(), port))
		{
			Logger::log(0, "  Connection Failed.");
			ScorchedClient::instance()->getMainLoop().draw();
			SDL_Delay(3 * 1000);
			return false;
		}
	}
	else
	{
		noPlayers = ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers() -
			ScorchedServer::instance()->getTankContainer().getNoOfTanks();
	}

	// If we connected then send our details to the server
	ComsConnectMessage connectMessage(
		ScorchedVersion,
		ScorchedProtocolVersion,
		OptionsParam::instance()->getPassword(),
		OptionsDisplay::instance()->getUniqueUserId(),
		noPlayers);
	if (!ComsMessageSender::sendToServer(connectMessage))
	{
		Logger::log(0, "  Connection Send Failed!");
	}

	Logger::log(0, "  Connection Succeeded.");
	MainBanner::instance();
	return true;
}


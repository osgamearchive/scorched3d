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
#include <dialogs/QuitDialog.h>
#include <GLW/GLWTextButton.h>
#include <client/ScorchedClient.h>
#include <client/MainBanner.h>
#include <client/ClientGameStoppedHandler.h>
#include <client/ClientMessageHandler.h>
#include <client/ClientTextHandler.h>
#include <client/ClientStartGameHandler.h>
#include <client/ClientScoreHandler.h>
#include <client/ClientAddPlayerHandler.h>
#include <client/ClientNewGameHandler.h>
#include <client/ClientBuyAccessoryHandler.h>
#include <client/ClientConnectionAcceptHandler.h>
#include <client/ClientRmPlayerHandler.h>
#include <client/ClientActionsHandler.h>
#include <client/ClientDefenseHandler.h>
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
	needCentered_ = true;
	quit_ = (GLWTextButton *) 
		addWidget(new GLWTextButton(" Quit", 205, 10, 85, this, false));
	listView_ = (GLWListView *) addWidget(new GLWListView(10, 40, 280, 140, 100));
	serverName_ = (GLWLabel *) addWidget(new GLWLabel(5, 195));

	// Add this class as a log handler
	Logger::addLogger(this);
}

ConnectDialog::~ConnectDialog()
{
}

void ConnectDialog::logMessage(
		const char *time,
		const char *message,
		unsigned int playerId)
{
	listView_->addLine(message);
}

void ConnectDialog::draw()
{
	static float fVPort[4];
	glGetFloatv(GL_VIEWPORT, fVPort);
	float width = fVPort[2] - 40.0f;
	if (width < 320) width = 320;
	if (width > 640) width = 640;
	setW(width - 20);
	listView_->setW(width - 40);
	quit_->setX(width - 115);

	float height = fVPort[3] - 40.0f;
	if (height < 200) height = 200;
	if (height > 300) height = 300;
	serverName_->setY(height - 50);
	listView_->setH(height - 90);
	setH(height - 20);

	GLWWindow::draw();

	if (tryConnection_ )
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
	std::string hostPart;
	const char *serverName = 
		OptionsParam::instance()->getConnect();
	serverName_->setText(serverName);
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

	Logger::log(0, "Atempting connection");
	Logger::log(0, "  Trying \"%s\"....", serverName);
	ScorchedClient::instance()->getMainLoop().draw();

	// Setup the coms handlers
	ScorchedClient::instance()->getNetInterface().setMessageHandler(
		&ScorchedClient::instance()->getComsMessageHandler());
	ScorchedClient::instance()->getComsMessageHandler().setConnectionHandler(
		ClientMessageHandler::instance());
	ClientTextHandler::instance();
	ClientConnectionAcceptHandler::instance();
	ClientAddPlayerHandler::instance();
	ClientNewGameHandler::instance();
	ClientRmPlayerHandler::instance();
	ClientGameStoppedHandler::instance();
	ClientStartGameHandler::instance();
	ClientDefenseHandler::instance();
	ClientActionsHandler::instance();
	ClientBuyAccessoryHandler::instance();
	ClientScoreHandler::instance();

	// Try to connect to the server
	NetServer &netServer = (NetServer &) ScorchedClient::instance()->getNetInterface();
	if (!netServer.connect((char *) hostPart.c_str(), port))
	{
		Logger::log(0, "  Connection Failed.");
		ScorchedClient::instance()->getMainLoop().draw();
		SDL_Delay(3 * 1000);
		return false;
	}

	// If we connected then send our details to the server
	ComsConnectMessage connectMessage(
		ScorchedVersion,
		ScorchedProtocolVersion,
		OptionsParam::instance()->getPassword(),
		OptionsDisplay::instance()->getUniqueUserId(),
		PlayerDialog::instance()->getPlayerName(),
		PlayerDialog::instance()->getModelName());
	if (!ComsMessageSender::sendToServer(connectMessage))
	{
		Logger::log(0, "  Connection Send Failed!");
	}

	Logger::log(0, "  Connection Succeeded.");
	MainBanner::instance();
	return true;
}

void ConnectDialog::buttonDown(unsigned int id)
{
	if (id == quit_->getId())
	{
		WindowManager::instance()->showWindow(QuitDialog::instance()->getId());
	}
}

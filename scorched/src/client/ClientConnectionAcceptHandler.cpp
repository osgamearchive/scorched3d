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

#include <client/ClientConnectionAcceptHandler.h>
#include <client/ScorchedClient.h>
#include <dialogs/RulesDialog.h>
#include <dialogs/ConnectDialog.h>
#include <engine/ModFiles.h>
#include <GLEXT/GLGif.h>
#include <tank/TankContainer.h>
#include <coms/ComsConnectAcceptMessage.h>
#include <coms/ComsHaveModFilesMessage.h>
#include <coms/ComsMessageSender.h>
#include <coms/NetServer.h>
#include <common/Logger.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>

ClientConnectionAcceptHandler *ClientConnectionAcceptHandler::instance_ = 0;

ClientConnectionAcceptHandler *ClientConnectionAcceptHandler::instance()
{
	if (!instance_)
	{
	  instance_ = new ClientConnectionAcceptHandler();
	}

	return instance_;
}

ClientConnectionAcceptHandler::ClientConnectionAcceptHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsConnectAcceptMessage",
		this);
}

ClientConnectionAcceptHandler::~ClientConnectionAcceptHandler()
{

}

bool ClientConnectionAcceptHandler::processMessage(unsigned int id,
												   const char *messageType,
												   NetBufferReader &reader)
{
	ComsConnectAcceptMessage message;
	if (!message.readMessage(reader)) return false;

	if (OptionsParam::instance()->getConnectedToServer())
	{
		unsigned int ip = NetServer::getIpAddress((TCPsocket) id);
		if (!ConnectDialog::instance()->getIdStore().saveUniqueId(
			ip, message.getUniqueId(), message.getPublishAddress()))
		{
			Logger::log( "Server failed ip security check!");
			return false;
		}
	}

	// The server tells us what our id is.
	// Set this id so we know what our players are
	ScorchedClient::instance()->getTankContainer().
		setCurrentDestinationId(message.getDestinationId());

	// Tell the user to wait
	Logger::log(
		"Connection accepted by \"%s\".\nPlease wait...",
		message.getServerName());

	// Show the MOTD (Message of the Day) on the screen and
	// rules dialog
	RulesDialog::instance()->addMOTD(
		ScorchedClient::instance()->getOptionsGame().getMOTD());

	// Set the server specific gif for the current server
	{
		GLGif map;
		if (message.getServerGif().getBufferUsed() > 0)
		{
			// Use a custom icon
			map.loadFromBuffer(message.getServerGif());
		}
		else
		{
			// Use the default icon
			map.loadFromFile(getDataFile("data/windows/scorched.gif"));
		}
		GLTexture *texture = new GLTexture;
		texture->create(map);
		RulesDialog::instance()->addIcon(texture);
	}

	// Set the mod
	setDataFileMod(
		ScorchedClient::instance()->getOptionsGame().getMod());

	// Load any mod files we currently have for the mod
	// the server is using.
	if (OptionsParam::instance()->getConnectedToServer() ||
		OptionsParam::instance()->getLoadModFiles())
	{
		if (!ScorchedClient::instance()->getModFiles().loadModFiles(
			ScorchedClient::instance()->getOptionsGame().getMod(), true))
		{
			dialogMessage("ModFiles", 
				"Failed to load mod \"%s\"",
				ScorchedClient::instance()->getOptionsGame().getMod());
			return false;
		}
	}

	// Tell the server what mod files we actually have
	ComsHaveModFilesMessage comsFileMessage;
	std::map<std::string, ModFileEntry *> &files = 
		ScorchedClient::instance()->getModFiles().getFiles();
	std::map<std::string, ModFileEntry *>::iterator itor;
	for (itor = files.begin();
		itor != files.end();
		itor++)
	{
		const std::string &name = (*itor).first;
		ModFileEntry *file = (*itor).second;

		comsFileMessage.getFiles().push_back(
			ModIdentifierEntry(
				name.c_str(),
				file->getCompressedSize(),
				file->getCompressedCrc()));
	}
	if (!ComsMessageSender::sendToServer(comsFileMessage)) return false;

	return true;
}

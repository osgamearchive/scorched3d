////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <server/ServerHaveModFilesHandler.h>
#include <server/ScorchedServer.h>
#include <server/ServerCommon.h>
#include <engine/ModFiles.h>
#include <coms/ComsHaveModFilesMessage.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <tank/TankContainer.h>
#include <time.h>

ServerHaveModFilesHandler *ServerHaveModFilesHandler::instance()
{
	static ServerHaveModFilesHandler *instance = 
		new ServerHaveModFilesHandler;
	return instance;
}

ServerHaveModFilesHandler::ServerHaveModFilesHandler()
{
	ScorchedServer::instance()->getComsMessageHandler().addHandler(
		"ComsHaveModFilesMessage",
		this);
}

ServerHaveModFilesHandler::~ServerHaveModFilesHandler()
{
}

bool ServerHaveModFilesHandler::processMessage(unsigned int destinationId,
	const char *messageType,
	NetBufferReader &reader)
{
	ComsHaveModFilesMessage message;
	if (!message.readMessage(reader)) return false;

	std::list<ModFileEntry *> neededEntries_;
	unsigned int neededLength = 0;

	// Build up a list of mod files that this client needs
	// and does not already have
	{
		std::map<std::string, ModFileEntry *> &modFiles = 
			ScorchedServer::instance()->getModFiles().getFiles();
		std::map<std::string, ModFileEntry *>::iterator itor;
		for (itor = modFiles.begin();
			itor != modFiles.end();
			itor++)
		{
			const std::string &fileName = (*itor).first;
			ModFileEntry *fileEntry = (*itor).second;

			ModIdentifierEntry *hasEntry = 
				message.getFile(fileName.c_str());
			if (!hasEntry ||
				hasEntry->crc != fileEntry->getCompressedCrc() ||
				hasEntry->length != fileEntry->getCompressedSize())
			{
				neededEntries_.push_back(fileEntry);
				neededLength += fileEntry->getCompressedSize();
			}
		}
	}

	if (neededEntries_.empty())
	{
		// No files need downloading
		ServerCommon::sendString(destinationId, 
			"No mod files need downloading");
	}
	else if (OptionsParam::instance()->getSinglePlayer())
	{
		// Do a sanity check that single player games don't need to download
		// any mod files.  As the server and client is the same process and
		// should be using the same mod directory.
		dialogMessage("ModFiles",
			"ERROR: Single player client required mod files");
		//exit(1);
	}
	else if (ScorchedServer::instance()->getOptionsGame().getModDownloadSpeed() == 0)
	{
		// If this server does not allow file downloads tell the client
		// and disconnect them
		Logger::log(destinationId, "No mod and mod download disabled");
		ServerCommon::sendString(destinationId,
			"This server requires the \"%s\" Scorched3D mod.\n"
			"The server does not allow in game file downloads.\n"
			"You must download and install this mod before you\n"
			"can connect to this server.",
			ScorchedServer::instance()->getOptionsGame().getMod());
		ServerCommon::kickDestination(destinationId);
	}
	else 
	{
		// This server allows file downloads
		// The the client how much needs to be downloaded
		ServerCommon::sendString(destinationId, 
			"This server requires the \"%s\" Scorched3D mod.\n"
			"This will require downloading %u bytes at "
			"%i bytes per second = %i seconds.",
			ScorchedServer::instance()->getOptionsGame().getMod(),
			neededLength,
			ScorchedServer::instance()->getOptionsGame().getModDownloadSpeed(),
			neededLength / ScorchedServer::instance()->getOptionsGame().getModDownloadSpeed());
	}

	// Set the files to download in this tanks profile
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		// For each tank
		Tank *tank = (*itor).second;
		if (destinationId == tank->getDestinationId())
		{
			// and for each needed entry
			std::list<ModFileEntry *>::iterator neededItor;
			for (neededItor = neededEntries_.begin();
				neededItor != neededEntries_.end();
				neededItor ++)
			{
				ModFileEntry *entry = (*neededItor);

				// Add the entry this tank needs to download
				ModIdentifierEntry newEntry(entry->getFileName(),
					0, entry->getCompressedCrc());
				tank->getMod().addFile(newEntry);
			}
			tank->getMod().setInit(true);
		}
	}

	return true;
}

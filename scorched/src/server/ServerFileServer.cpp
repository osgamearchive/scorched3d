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

#include <server/ServerFileServer.h>
#include <server/ScorchedServer.h>
#include <tank/TankContainer.h>
#include <common/OptionsGame.h>
#include <coms/ComsMessageSender.h>
#include <engine/ModFiles.h>
#include <time.h>

ServerFileServer *ServerFileServer::instance_ = 0;

ServerFileServer *ServerFileServer::instance()
{
	if (!instance_)
	{
		instance_ = new ServerFileServer;
	}
	return instance_;
}

ServerFileServer::ServerFileServer() : lastTime_(0), bytesSent_(0)
{
}

ServerFileServer::~ServerFileServer()
{
}

void ServerFileServer::simulate(float timeDifference)
{
	int downloadCount = 0;

	// Check how many people are currently downloading
	// also check for any that have finished downloading
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getState().getLoading())
		{
			if (!tank->getMod().getFiles().empty())
			{
				downloadCount++;
			}
			else
			{
				tank->getState().setLoading(false);
			}
		}
	}

	// If no people are downloading then there is nothing to do
	if (downloadCount == 0) return;

	// Cacluate the maximum amount that can be downloaded by each
	// client per second
	int maxDownloadPerClient = 
		ScorchedServer::instance()->getOptionsGame().getModDownloadSpeed() /
		downloadCount;

	// Check if this is the next second
	unsigned int theTime = time(0);
	if (theTime > lastTime_)
	{
		// If so reset the amount that can be sent by this server
		lastTime_ = theTime;
		bytesSent_ = 0;

		// Reset the sent state for each client
		for (itor = tanks.begin();
			itor != tanks.end();
			itor++)
		{
			Tank *tank = (*itor).second;
			if (tank->getState().getLoading())
			{
				tank->getMod().setSent(false);
			}
		}
	}

	// Send bytes to each ready client
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		// Make sure we have not sent too much this second already
		if (bytesSent_ + maxDownloadPerClient > 
			(unsigned int) ScorchedServer::instance()->getOptionsGame().getModDownloadSpeed() + 5)
		{
			return;
		}

		Tank *tank = (*itor).second;
		if (tank->getState().getLoading())
		{
			// Check if the client is ready to recieve more
			// bytes and there is some to send and
			// we have not sent to this client this second
			if (tank->getMod().getReadyToReceive() &&
				!tank->getMod().getFiles().empty() &&
				!tank->getMod().getSent())
			{
				// Send bytes to this tank
				tank->getMod().setSent(true);
				tank->getMod().setReadyToReceive(false);
				bytesSent_ += maxDownloadPerClient;

				sendBytes(tank, maxDownloadPerClient);
			}
		}
	}
}

void ServerFileServer::sendBytes(Tank *tank, unsigned int size)
{
	ComsFileMessage message;
	// Fill up the message with files, until it is full
	while (size > 0)
	{
		unsigned int bytesSent = sendNextFile(message, tank, size);
		if (bytesSent == 0) break; // Check that we have files to send
		size -= bytesSent;
	}
	message.fileBuffer.addToBuffer("");

	// Send the message to the client
	ComsMessageSender::sendToSingleClient(message, 
		tank->getDestinationId());

}

unsigned int ServerFileServer::sendNextFile(ComsFileMessage &message,
											Tank *tank, unsigned int size)
{
	// Get the next file to send
	std::list<ModIdentifierEntry> &files = 
		tank->getMod().getFiles();
	if (files.empty()) return 0;
	ModIdentifierEntry &entry = files.front();

	// Find the next file in the modfiles
	std::map<std::string, ModFileEntry *> &modfiles =
		ScorchedServer::instance()->getModFiles().getFiles();
	std::map<std::string, ModFileEntry *>::iterator findItor =
		modfiles.find(entry.fileName);
	DIALOG_ASSERT(findItor != modfiles.end());
	ModFileEntry *modentry = (*findItor).second;

    // Check how much still needs to be sent
	unsigned int sizeSent = entry.length;
	unsigned int sizeLeftToSend = modentry->getCompressedSize() - sizeSent;
	unsigned int sizeToSend = MIN(sizeLeftToSend, size);

	// Add the bytes to the buffer
	message.fileBuffer.addToBuffer(modentry->getFileName());
	message.fileBuffer.addToBuffer(modentry->getCompressedSize());
	message.fileBuffer.addToBuffer(modentry->getUncompressedSize());
	message.fileBuffer.addToBuffer(modentry->getCompressedCrc());
	message.fileBuffer.addToBuffer(sizeToSend);
	message.fileBuffer.addDataToBuffer(modentry->getCompressedBytes() + entry.length,
		sizeToSend);

	// Update how much we have sent
	entry.length += sizeToSend;

	// Have we sent the whole file
	if (sizeToSend == sizeLeftToSend)
	{
		// If so remove the file from the list that
		// still needs to be sent
		tank->getMod().rmFile(modentry->getFileName());
	}

	return sizeToSend;
}

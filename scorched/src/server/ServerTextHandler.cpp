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

#include <server/ServerTextHandler.h>
#include <server/ScorchedServer.h>
#include <server/ScorchedServerUtil.h>
#include <server/ServerCommon.h>
#include <tank/TankContainer.h>
#include <coms/ComsTextMessage.h>
#include <coms/ComsMessageSender.h>
#include <common/Logger.h>

ServerTextHandler *ServerTextHandler::instance()
{
	static ServerTextHandler *instance = 
		new ServerTextHandler;
	return instance;
}

ServerTextHandler::ServerTextHandler()
{
	ScorchedServer::instance()->getComsMessageHandler().addHandler(
		"ComsTextMessage",
		this);
}

ServerTextHandler::~ServerTextHandler()
{
}

bool ServerTextHandler::processMessage(unsigned int destinationId,
	const char *messageType,
	NetBufferReader &reader)
{
	ComsTextMessage message;
	if (!message.readMessage(reader)) return false;

	// Check that we don't recieve too much text
	if (strlen(message.getText()) > 1024) return true;

	// If the client does not supply a tank id then
	// get one from the destination
	unsigned int tankId = message.getPlayerId();
	if (tankId == 0)
	{
		std::map<unsigned int, Tank *> &tanks =
			ScorchedServer::instance()->getTankContainer().getPlayingTanks();
		std::map<unsigned int, Tank *>::iterator itor;
		for (itor = tanks.begin();
			itor != tanks.end();
			itor++)
		{
			Tank *tank = (*itor).second;
			if (tank->getDestinationId() == destinationId) 
				tankId = tank->getPlayerId();
		}
	}

	// Check this is a tank, and comes from the correct destination
	Tank *tank = ScorchedServer::instance()->getTankContainer().getTankById(tankId);
	if (tank && tank->getDestinationId() == destinationId)
	{
		// Construct the message to send to all the clients
		std::string newText(tank->getName());
		if (tank->getState().getMuted())
		{
			newText += " **MUTED**";
		}
		if (tank->getState().getState() != TankState::sNormal)
		{
			newText += " (";
			newText += tank->getState().getSmallStateString();
			newText += ")";
		}
		if (message.getTeamOnlyMessage())
		{
			newText += " (Team)";
		}
		newText += ": ";
		unsigned int infoLen = newText.length();

		// Filter the string
		std::string filteredText(message.getText());
		ScorchedServerUtil::instance()->textFilter.filterString(filteredText);
		newText += filteredText;
		
		// Remove any bad characters
		for (char *r = (char *) newText.c_str(); *r; r++)
		{
			if (*r == '%') *r = ' ';
		}

		// Update the server console with the say text
		ServerCommon::serverLog(formatString("Says \"%s\"", newText.c_str()));

		ComsTextMessage newMessage(newText.c_str(), 
			tank->getPlayerId(), false, message.getTeamOnlyMessage(), infoLen);

		// Store message
		addMessage(newMessage.getText());

		// Send to players
		if (!tank->getState().getMuted())
		{
			if (message.getTeamOnlyMessage())
			{
				// Send all team messages to everyone in the team (or any admins)
				// Only send to the same destination once
				std::set<unsigned int> doneDests;
				std::map<unsigned int, Tank *> &tanks =
					ScorchedServer::instance()->getTankContainer().getPlayingTanks();
				std::map<unsigned int, Tank *>::iterator itor;
				for (itor = tanks.begin();
					itor != tanks.end();
					itor++)
				{
					Tank *currentTank = (*itor).second;
					if (tank->getTeam() == currentTank->getTeam() ||
						currentTank->getState().getAdmin())
					{
						if (doneDests.find(currentTank->getDestinationId()) ==
							doneDests.end())
						{
							doneDests.insert(currentTank->getDestinationId());
							ComsMessageSender::sendToSingleClient(newMessage,
								currentTank->getDestinationId());
						}
					}
				}
			}
			else
			{
				// Else send to everyone
				ComsMessageSender::sendToAllConnectedClients(newMessage);
			}
		}
		else
		{
			// Only send admin muted texts to the player that said it
			// and all admins
			std::map<unsigned int, Tank *> &tanks =
				ScorchedServer::instance()->getTankContainer().getPlayingTanks();
			std::map<unsigned int, Tank *>::iterator itor;
			for (itor = tanks.begin();
				itor != tanks.end();
				itor++)
			{
				Tank *currentTank = (*itor).second;
				if (currentTank->getDestinationId() == destinationId ||
					currentTank->getState().getAdmin())
				{
					ComsMessageSender::sendToSingleClient(newMessage,
						currentTank->getDestinationId());
				}
			}			
		}
	}

	return true;
}

void ServerTextHandler::addMessage(const char *text)
{
	lastMessages_.push_back(text);
	if (lastMessages_.size() > 25) lastMessages_.pop_front();
}

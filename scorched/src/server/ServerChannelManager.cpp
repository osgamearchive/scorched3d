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

#include <server/ServerChannelManager.h>
#include <server/ScorchedServer.h>
#include <server/ScorchedServerUtil.h>
#include <server/ServerCommon.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsChannelMessage.h>
#include <coms/ComsChannelTextMessage.h>
#include <tank/TankContainer.h>
#include <tank/TankState.h>

ServerChannelManager::ChannelEntry::ChannelEntry(
	const char *channelName,
	ServerChannelFilter *filter) :
	channelName_(channelName),
	filter_(filter)
{
}

ServerChannelManager::ChannelEntry::~ChannelEntry()
{
}

ServerChannelManager::DestinationLocalEntry::DestinationLocalEntry(
	unsigned int localId) :
	localId_(localId)
{
}

ServerChannelManager::DestinationEntry::DestinationEntry(
	unsigned int destinationId) :
	destinationId_(destinationId)
{
}

bool ServerChannelManager::DestinationEntry::hasChannel(const char *channel) 
{ 
	return (channels_.find(channel) != channels_.end()); 
}

void ServerChannelManager::DestinationEntry::addChannel(const char *channel, unsigned int localId)
{
	if (!hasLocalId(localId)) return;
	localEntries_[localId].getChannels().insert(channel);
	updateChannels();
}

void ServerChannelManager::DestinationEntry::removeChannel(const char *channel, unsigned int localId)
{
	if (!hasLocalId(localId)) return;
	localEntries_[localId].getChannels().erase(channel);
	updateChannels();
}

void ServerChannelManager::DestinationEntry::getLocalIds(const char *channel, std::list<unsigned int> &ids)
{
	std::map<unsigned int, DestinationLocalEntry>::iterator itor;
	for (itor = localEntries_.begin();
		itor != localEntries_.end();
		itor++)
	{
		DestinationLocalEntry &entry = (*itor).second;
		if (entry.getChannels().find(channel) != entry.getChannels().end())
		{
			ids.push_back((*itor).first);
		}
	}
}

bool ServerChannelManager::DestinationEntry::hasLocalId(unsigned int localId)
{
	return (localEntries_.find(localId) != localEntries_.end());
}

void ServerChannelManager::DestinationEntry::addLocalId(unsigned int localId)
{
	if (hasLocalId(localId)) return;
	localEntries_[localId] = DestinationLocalEntry(localId);
	updateChannels();
}

void ServerChannelManager::DestinationEntry::removeLocalId(unsigned int localId)
{
	if (!hasLocalId(localId)) return;
	localEntries_.erase(localId);
	updateChannels();
}

void ServerChannelManager::DestinationEntry::updateChannels()
{
	channels_.clear();
	std::map<unsigned int, DestinationLocalEntry>::iterator itor;
	for (itor = localEntries_.begin();
		itor != localEntries_.end();
		itor++)
	{
		DestinationLocalEntry &entry = (*itor).second;
		channels_.insert(entry.getChannels().begin(), entry.getChannels().end());
	}
}

ServerChannelManager *ServerChannelManager::instance_ = 0;

ServerChannelManager *ServerChannelManager::instance()
{
	if (!instance_)
	{
		instance_ = new ServerChannelManager;
	}
	return instance_;
}

ServerChannelManager::ServerChannelManager()
{
	// Register to recieve comms messages
	ScorchedServer::instance()->getComsMessageHandler().addHandler(
		"ComsChannelMessage",
		this);
	ScorchedServer::instance()->getComsMessageHandler().addHandler(
		"ComsChannelTextMessage",
		this);

	// Create some default channels
	channelEntries_.push_back(new ChannelEntry("info"));
	channelEntries_.push_back(new ChannelEntry("general"));
	channelEntries_.push_back(new ChannelEntry("team", 
		new ServerChannelFilterTeams()));
	channelEntries_.push_back(new ChannelEntry("spam"));
	channelEntries_.push_back(new ChannelEntry("combat"));
	channelEntries_.push_back(new ChannelEntry("banner"));
}

ServerChannelManager::~ServerChannelManager()
{
}

void ServerChannelManager::destinationDisconnected(unsigned int destinationId)
{
	DestinationEntry *entry = getDestinationEntryById(destinationId);
	if (!entry) return;

	destinationEntries_.erase(destinationId);
	delete entry;
}

ServerChannelManager::ChannelEntry *ServerChannelManager::getChannelEntryByName(const char *name)
{
	std::list<ChannelEntry *>::iterator itor;
	for (itor = channelEntries_.begin();
		itor != channelEntries_.end();
		itor++)
	{
		ChannelEntry *entry = *itor;
		if (0 == strcmp(entry->getName(), name)) return entry;
	}
	return 0;
}

ServerChannelManager::DestinationEntry *ServerChannelManager::getDestinationEntryById(
	unsigned int destinationId)
{
	std::map<unsigned int, DestinationEntry *>::iterator findItor = 
		destinationEntries_.find(destinationId);
	if (findItor == destinationEntries_.end()) return 0;

	return (*findItor).second;
}

void ServerChannelManager::registerClient(unsigned int destinationId, unsigned int localId,
	std::list<std::string> &startChannels)
{
	// Get the sender for this message
	DestinationEntry *destEntry = getDestinationEntryById(destinationId);
	if (destEntry && destEntry->hasLocalId(localId)) return; // Already an entry

	// Add the new entry
	if (!destEntry)
	{
		destEntry = new DestinationEntry(destinationId);
		destinationEntries_[destinationId] = destEntry;
	}
	// Add the new local entry
	if (!destEntry->hasLocalId(localId))
	{
		destEntry->addLocalId(localId);
	}

	// Join the client to the new channels
	joinClient(destinationId, localId, startChannels);
}

void ServerChannelManager::deregisterClient(unsigned int destinationId, unsigned int localId)
{
	// Get the sender for this message
	DestinationEntry *destEntry =  getDestinationEntryById(destinationId);
	if (!destEntry || !destEntry->hasLocalId(localId)) return; // No such user

	// Remove the entry
	destEntry->removeLocalId(localId);
}

void ServerChannelManager::joinClient(unsigned int destinationId, unsigned int localId,
	std::list<std::string> &startChannels)
{
	// Get the sender for this message
	DestinationEntry *destEntry = getDestinationEntryById(destinationId);
	if (!destEntry || !destEntry->hasLocalId(localId)) return; // No such user

	// Form the list of available and joined channels
	// and send them back to the client
	ComsChannelMessage message(ComsChannelMessage::eJoinRequest, localId);
	std::list<ChannelEntry *>::iterator itor;
	for (itor = channelEntries_.begin();
		itor != channelEntries_.end();
		itor++)
	{
		ChannelEntry *channelEntry = (*itor);

		// TODO check if a user is allowed a channel at all

		// Check if the user has asked for this channel
		bool add = false;
		std::list<std::string>::iterator startItor;
		for (startItor = startChannels.begin();
			startItor != startChannels.end();
			startItor++)
		{
			std::string &startChannel = *startItor;
			if (0 == strcmp(startChannel.c_str(), channelEntry->getName()))
			{
				add = true;
				break;
			}
		}


		if (add)
		{
			message.getChannels().push_back(channelEntry->getName());
			destEntry->addChannel(channelEntry->getName(), localId);
		}
		else
		{
			message.getAvailableChannels().push_back(channelEntry->getName());
			destEntry->removeChannel(channelEntry->getName(), localId);
		}
	}
	ComsMessageSender::sendToSingleClient(message, destinationId);
}

void ServerChannelManager::sendText(const ChannelText &constText, 
	unsigned int destination)
{
	DestinationEntry *destinationEntry = getDestinationEntryById(
		destination);
	if (destinationEntry)
	{
		std::map<unsigned int, DestinationEntry *> destinations;
		destinations[destination] = destinationEntry;
		actualSend(constText, destinations);
	}
}

void ServerChannelManager::sendText(const ChannelText &constText)
{
	actualSend(constText, destinationEntries_);
}

void ServerChannelManager::actualSend(const ChannelText &constText,
	std::map<unsigned int, DestinationEntry *> &destinations)
{
	ChannelText text = constText;

	// Get the tank for this message (if any)
	Tank *tank = ScorchedServer::instance()->getTankContainer().getTankById(
		text.getPlayerId());

	// Check that this channel exists
	ChannelEntry *channelEntry = getChannelEntryByName(text.getChannel());
	if (!channelEntry)
	{
		// This channel does not exist
		return;
	}

	// Filter the string for bad language
	std::string filteredText(text.getMessage());
	ScorchedServerUtil::instance()->textFilter.filterString(filteredText);
	std::string newText = filteredText;

	// Remove any bad characters
	for (char *r = (char *) newText.c_str(); *r; r++)
	{
		if (*r == '%' || *r < 0 || *r == '[' || *r == ']') *r = ' ';
	}

	// Update the server console with the say text
	const char *logtext = formatString("[%s] : %s \"%s\"", 
		text.getChannel(),
		(tank?tank->getName():""),
		newText.c_str());
	ServerCommon::serverLog(logtext);
	lastMessages_.push_back(logtext);
	if (lastMessages_.size() > 25) lastMessages_.pop_front();

	// Send to all clients
	std::map<unsigned int, DestinationEntry *>::iterator destItor;
	for (destItor = destinations.begin();
		destItor != destinations.end();
		destItor++)
	{
		DestinationEntry *entry = (*destItor).second;
		if (!channelEntry->getFilter() || channelEntry->getFilter()->sentToDestination(
			text, entry->getDestinationId()))
		{
			if (entry->hasChannel(text.getChannel()))
			{
				ComsChannelTextMessage newTextMessage(text);
				entry->getLocalIds(text.getChannel(), newTextMessage.getIds());
				ComsMessageSender::sendToSingleClient(newTextMessage, entry->getDestinationId());
			}
		}
	}
}

bool ServerChannelManager::processMessage(
	NetMessage &netNessage,
	const char *messageType,
	NetBufferReader &reader)
{
	// Check which message we have got
	if (0 == strcmp("ComsChannelMessage", messageType))
	{
		// We have a ChannelMessage from the server
		ComsChannelMessage channelMessage;
		if (!channelMessage.readMessage(reader)) return false;

		// Check which ChannelMessage was sent
		switch (channelMessage.getType())
		{
		case ComsChannelMessage::eRegisterRequest:
			registerClient(netNessage.getDestinationId(), channelMessage.getId(),
				channelMessage.getChannels());
			break;
		case ComsChannelMessage::eDeregisterRequest:
			deregisterClient(netNessage.getDestinationId(), channelMessage.getId());
			break;
		case ComsChannelMessage::eJoinRequest:
			joinClient(netNessage.getDestinationId(), channelMessage.getId(),
				channelMessage.getChannels());
			break;
		}
	}
	else if (0 == strcmp("ComsChannelTextMessage", messageType))
	{
		// We have a ChannelTextMessage from the server
		ComsChannelTextMessage textMessage;
		if (!textMessage.readMessage(reader)) return false;

		// Validate that this message came from this tank
		Tank *tank = ScorchedServer::instance()->getTankContainer().getTankById(
			textMessage.getChannelText().getPlayerId());
		if (!tank || tank->getDestinationId() != netNessage.getDestinationId()) return true;

		// Check this tank has not been admin muted
		if (tank->getState().getMuted()) return true;

		// Check that this client has this channel
		DestinationEntry *destEntry = getDestinationEntryById(netNessage.getDestinationId());
		if (!destEntry || !destEntry->hasChannel(textMessage.getChannelText().getChannel()))
		{
			// This client does not have this channel
			return true;
		}

		// Check that we don't recieve too much text
		if (strlen(textMessage.getChannelText().getMessage()) > 1024) return true;

		// Send the text
		sendText(textMessage.getChannelText());
	}
	else return false;

	return true;
}
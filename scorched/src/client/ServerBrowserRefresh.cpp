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

#include <XML/XMLStringBuffer.h>
#include <client/ServerBrowserRefresh.h>
#include <common/Defines.h>
#include <time.h>

ServerBrowserRefresh::ServerBrowserRefresh(ServerBrowserServerList &list) :
	list_(list)
{
	recvPacket_ = SDLNet_AllocPacket(10000);
	sendPacket_ = SDLNet_AllocPacket(20);
	sendPacket_->len = 7;
	memcpy(sendPacket_->data, "status", 7);
}

ServerBrowserRefresh::~ServerBrowserRefresh()
{
}

void ServerBrowserRefresh::refreshList()
{
	for (int i=0; i<list_.getNoEntries(); i++)
	{
		refreshEntries_.push_back(&list_.getEntry(i));
	}

	while (!refreshEntries_.empty() || !entryMap_.empty())
	{
		time_t theTime = time(0);

		// Add a number of new entries
		for (int i=entryMap_.size(); i<20 && !refreshEntries_.empty(); i++)
		{
			ServerBrowserEntry *entry = refreshEntries_.front();
			refreshEntries_.pop_front();

			sendNextEntry(entry, theTime);
		}
		SDL_Delay(1000);

		// Process all new messages
		processMessages(theTime);
	}
	list_.getRefreshId()++;
}

void ServerBrowserRefresh::sendNextEntry(ServerBrowserEntry *entry, time_t theTime)
{
	const char *name = entry->getAttribute("address");
	if (!name[0]) return;

	static char buffer[256];
	strcpy(buffer, name);
	char *port = strchr(buffer, ':');
	if (!port) return;
	*port = '\0';
	port++;

	IPaddress address;
	if (SDLNet_ResolveHost(&address, buffer, atoi(port)+1) != 0) return;

	// Connect to the client
	UDPsocket udpsock = SDLNet_UDP_Open(0);
	if (!udpsock) return;

	int chan = SDLNet_UDP_Bind(udpsock, -1, &address);
	if (chan == -1) return;

	// Add the client to the map of currently processed clients
	entryMap_[udpsock] = entry;
	entry->retries_ ++;
	entry->sentTime_ = theTime;

	// Send the request for info
	SDLNet_UDP_Send(udpsock, chan, sendPacket_);
}

void ServerBrowserRefresh::processMessages(time_t theTime)
{
	std::list<UDPsocket> finished;
	std::map<UDPsocket, ServerBrowserEntry*>::iterator itor;
	for (itor = entryMap_.begin();
		itor != entryMap_.end();
		itor++)
	{
		UDPsocket socket = (*itor).first;
		ServerBrowserEntry *entry = (*itor).second;
		if (SDLNet_UDP_Recv(socket, recvPacket_))
		{
			processMessage(recvPacket_, entry);
			finished.push_back(socket);

			list_.getRefreshId()++;
		}
		else if (theTime - entry->sentTime_ > 5)
		{
			if (entry->retries_ < 3) refreshEntries_.push_back(entry);
			finished.push_back(socket);
		}
	}

	while (finished.size())
	{
		UDPsocket socket = finished.front();
		finished.pop_front();

		entryMap_.erase(socket);
		SDLNet_UDP_Close(socket);
	}
}

void ServerBrowserRefresh::processMessage(UDPpacket *packet, ServerBrowserEntry *serverEntry)
{
	const char *buffer = (char *) packet->data;
	unsigned int len = packet->len - 1;

	// Parse the buffer
	XMLStringBuffer xmlBuffer;
	if (xmlBuffer.create(buffer, len))
	{
		// Itterate all of the keys in the buffer
		std::list<XMLNode *>::iterator childrenItor;
		std::list<XMLNode *> &children = xmlBuffer.getRootNode()->getChildren();
		for (childrenItor = children.begin();
			childrenItor != children.end();
			childrenItor++)
		{
			XMLNode *currentNode = (*childrenItor);
			serverEntry->addAttribute(
				currentNode->getName(),
				currentNode->getContent());
		}
	}
}

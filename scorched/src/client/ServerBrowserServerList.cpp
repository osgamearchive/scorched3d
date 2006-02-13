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
#include <client/ServerBrowserServerList.h>
#include <client/ScorchedClient.h>
#include <common/Logger.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <time.h>

ServerBrowserEntry::ServerBrowserEntry() : retries_(0), sentTime_(0), recieved_(0)
{
}

ServerBrowserEntry::~ServerBrowserEntry()
{
}

void ServerBrowserEntry::addAttribute(const char *name, const char *value)
{
	attributes_[name] = value;
}

const char *ServerBrowserEntry::getAttribute(const char *attrName)
{
	static const char *empty = "";

	std::map<std::string, std::string>::iterator itor = 
		attributes_.find(attrName);
	if (itor == attributes_.end()) return empty;
	
	return (*itor).second.c_str();
}

ServerBrowserServerList::ServerBrowserServerList() : 
	netServer_(new NetServerHTTPProtocolSend), complete_(false), refreshId_(0)
{
	// Create the message that will be sent to the master server
	static char buffer[1024];
	snprintf(buffer, 1024,
		"GET %s/servers.php HTTP/1.0\r\n"
		"User-Agent: Scorched3D\r\n"
		"Host: %s\r\n"
		"Connection: Keep-Alive\r\n"
		"\r\n"
		"\r\n",
		ScorchedClient::instance()->getOptionsGame().getMasterListServerURI(),
		ScorchedClient::instance()->getOptionsGame().getMasterListServer());
	sendNetBuffer_.addDataToBuffer(buffer, strlen(buffer)); // Note no null

	// All messages will come to this class
	netServer_.setMessageHandler(this);
	vectorMutex_ = SDL_CreateMutex();

	recvPacket_ = SDLNet_AllocPacket(10000);
	sendPacket_ = SDLNet_AllocPacket(20);
	sendPacket_->len = 5;
	memcpy(sendPacket_->data, "ping", 5);
}


ServerBrowserServerList::~ServerBrowserServerList()
{
	SDL_DestroyMutex(vectorMutex_);
}

bool ServerBrowserServerList::fetchServerList()
{
	complete_ = false;

	SDL_LockMutex(vectorMutex_);
	servers_.clear();
	SDL_UnlockMutex(vectorMutex_);

	// Connect to the master server
	if (!netServer_.connect(ScorchedClient::instance()->getOptionsGame().getMasterListServer(), 80))
	{
		Logger::log( "Failed to contact registration server");
		return false;
	}

	// Send the master server request
	netServer_.sendMessage(sendNetBuffer_);
	
	const unsigned int WaitTime = 30;
	// Wait WaitTime seconds for the result
	time_t startTime, currentTime;
	startTime = currentTime = time(0);
	while (currentTime - startTime < WaitTime)
	{
		// Process any waiting messages
		netServer_.processMessages();

		// Check if the messages have made us complete
		if (complete_)
		{
			refreshId_++;
			return true;
		}

		SDL_Delay(100);
		currentTime = time(0);
	}

	// Ensure that we only have one open connection to the server
	netServer_.disconnectAllClients();

	complete_ = true;
	return false;
}

bool ServerBrowserServerList::fetchLANList()
{
	complete_ = false;

	if(SDLNet_Init()==-1) false;

	SDL_LockMutex(vectorMutex_);
	servers_.clear();
	SDL_UnlockMutex(vectorMutex_);
	
	SDLNet_ResolveHost(&(sendPacket_->address), "255.255.255.255", ScorchedPort + 1);
	
	// Connect to the client
	UDPsocket udpsock = SDLNet_UDP_Open(0);
	if (!udpsock) return false;
	
	// Send the request for info
	SDLNet_UDP_Send(udpsock, -1, sendPacket_);

	// Accept the results
	time_t startTime = time(0);
	for (;;)
	{
		SDL_Delay(100);
		if (SDLNet_UDP_Recv(udpsock, recvPacket_))
		{
			unsigned int addr = SDLNet_Read32(&recvPacket_->address.host);
			unsigned int port = SDLNet_Read16(&recvPacket_->address.port);

			// Get the name attribute
			ServerBrowserEntry newEntry;
			char hostName[256];
			snprintf(hostName,
					256,
					"%s:%i",
					NetInterface::getIpName(addr),
					(port - 1));
			newEntry.addAttribute("address", hostName);
			
			// Add the new and its attributes
			SDL_LockMutex(vectorMutex_);
			servers_.push_back(newEntry);
			SDL_UnlockMutex(vectorMutex_);
			
			refreshId_++;
		}
		
		time_t theTime = time(0);
		if (theTime - startTime > 5) break;
	}
	
	SDLNet_UDP_Close(udpsock);
	complete_ = true;

	return true;
}

void ServerBrowserServerList::processMessage(NetMessage &message)
{
	// We have received a reply from the web server
	if (message.getMessageType() == NetMessage::DisconnectMessage)
	{
		complete_ = true;
	}
	else if (message.getMessageType() == NetMessage::BufferMessage)
	{
		// Add the null to the message
		message.getBuffer().addDataToBuffer("\0", 1);
		const char *buffer = message.getBuffer().getBuffer();

		// Find the start of the XML
		while (*buffer != '<' && *buffer != '\0') buffer++;
		unsigned int len = strlen(buffer);

		// Parse the buffer
		if (*buffer == '<')
		{
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
					// Find each server node
					XMLNode *currentNode = (*childrenItor);
					if (0 == strcmp(currentNode->getName(), "server"))
					{
						ServerBrowserEntry newEntry;

						// Get the name attribute
						XMLNode *nameNode = 0;
						currentNode->getNamedChild("name", nameNode, false);
						if (nameNode) newEntry.addAttribute("address", nameNode->getContent());

						// Add the new and its attributes
						SDL_LockMutex(vectorMutex_);
						servers_.push_back(newEntry);
						SDL_UnlockMutex(vectorMutex_);
					}
				}
			}
		}
	}
}

const char *ServerBrowserServerList::getEntryValue(int pos, const char *name)
{
	SDL_LockMutex(vectorMutex_);
	ServerBrowserEntry &entry = getEntry(pos);
	static char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s", entry.getAttribute(name));
	SDL_UnlockMutex(vectorMutex_);

	return buffer;
}

int ServerBrowserServerList::getNoEntries()
{
	return servers_.size();
}

ServerBrowserEntry &ServerBrowserServerList::getEntry(int pos)
{
	DIALOG_ASSERT(pos >=0 && pos < (int) servers_.size());
	return servers_[pos];
}

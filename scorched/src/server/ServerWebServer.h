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

#if !defined(__INCLUDE_ServerWebServerh_INCLUDE__)
#define __INCLUDE_ServerWebServerh_INCLUDE__

#include <net/NetServerTCP.h>
#include <common/FileLogger.h>
#include <server/ServerWebServerQueue.h>

class ServerWebServerI
{
public:
	virtual ServerWebServerI *createCopy() = 0;
	virtual bool processRequest(const char *url,
		std::map<std::string, std::string> &fields,
		std::map<std::string, NetMessage *> &parts,
		std::string &text) = 0;
};

class ServerWebServer : public NetMessageHandlerI
{
public:
	struct SessionParams
	{
		unsigned int sessionTime;
		std::string ipAddress;
		std::string userName;
	};

	static ServerWebServer *instance();

	void start(int port);
	void processMessages();
	void addRequestHandler(const char *url,
		ServerWebServerI *handler);
	void addThrededRequestHandler(const char *url,
		ServerWebServerI *handler);
	void addAsyncRequestHandler(const char *url,
		ServerWebServerI *handler);

	std::map<unsigned int, SessionParams> &getSessions() { return sessions_; }

protected:
	static ServerWebServer *instance_;

	struct HandlerEntry
	{
		enum Flags
		{
			eAsync = 1,
			eThreaded = 2
		};

		ServerWebServerI *handler;
		unsigned int flags;
	};
	std::map<std::string, HandlerEntry> handlers_;

	ServerWebServerQueue asyncQueue_;
	ServerWebServerQueue threadedQueue_;
	ServerWebServerQueue normalQueue_;

	unsigned int asyncTimer_;
	std::map<unsigned int, SessionParams> sessions_;
	std::list<std::pair<unsigned int, NetMessage *> > delayedMessages_;

	SDL_Thread *sendThread_;
	NetServerTCP netServer_;
	FileLogger *logger_;

	bool processRequest(
		unsigned int destinationId,
		const char *ip,
		const char *url,
		std::map<std::string, std::string> &fields,
		std::map<std::string, NetMessage *> &parts);
	bool validateUser(
		const char *ip,
		const char *url,
		std::map<std::string, std::string> &fields);
	unsigned int validateSession(
		const char *ip,
		const char *url,
		std::map<std::string, std::string> &fields);

	static int sendThreadFunc(void *);
	bool processQueue(ServerWebServerQueue &queue, bool keepEntries);

	// Inherited from NetMessageHandlerI
	virtual void processMessage(NetMessage &message);

private:
	ServerWebServer();
	virtual ~ServerWebServer();
};

#endif

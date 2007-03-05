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
#include <string>
#include <map>

class ServerWebServerI
{
public:
	virtual bool processRequest(const char *url,
		std::map<std::string, std::string> &fields,
		std::map<std::string, NetMessage *> &parts,
		std::string &text) = 0;
};

class ServerWebServerAsyncI
{
public:
	virtual ServerWebServerAsyncI *create() = 0;

	virtual bool processRequest(std::string &text) = 0;
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
	void addAsyncRequestHandler(const char *url,
		ServerWebServerAsyncI *handler);

	std::map<unsigned int, SessionParams> &getSessions() { return sessions_; }

protected:
	static ServerWebServer *instance_;

	struct AsyncEntry
	{
		unsigned int sid;
		ServerWebServerAsyncI *handler;
	};
	std::map<unsigned int, AsyncEntry> asyncProcesses_;
	std::map<unsigned int, SessionParams> sessions_;
	std::map<std::string, ServerWebServerI *> handlers_;
	std::map<std::string, ServerWebServerAsyncI *> asyncHandlers_;
	std::list<std::pair<unsigned int, NetMessage *> > delayedMessages_;
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
	bool generatePage(
		unsigned int destinationId,
		unsigned int sid,
		const char *url,
		std::map<std::string, std::string> &fields,
		std::map<std::string, NetMessage *> &parts,
		std::string &text);

	// Inherited from NetMessageHandlerI
	virtual void processMessage(NetMessage &message);

private:
	ServerWebServer();
	virtual ~ServerWebServer();
};

#endif

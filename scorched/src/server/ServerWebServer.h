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

#include <coms/NetServer.h>
#include <common/FileLogger.h>
#include <string>
#include <map>

class ServerWebServer : public NetMessageHandlerI
{
public:
	static ServerWebServer *instance();

	void start();
	void processMessages();

protected:
	static ServerWebServer *instance_;
	NetServer netServer_;
	FileLogger *logger_;

	bool processRequest(
		unsigned int destinationId,
		const char *url,
		std::map<std::string, std::string> &fields);
	bool validateUser(
		std::map<std::string, std::string> &fields);
	bool generatePage(
		const char *url,
		std::map<std::string, std::string> &fields,
		std::string &text);
	bool getTemplate(
		const char *name,
		std::map<std::string, std::string> &fields,
		std::string &result);

	// Inherited from NetMessageHandlerI
	virtual void processMessage(NetMessage &message);

private:
	ServerWebServer();
	virtual ~ServerWebServer();
};

#endif

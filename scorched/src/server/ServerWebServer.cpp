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

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <server/ServerWebServer.h>
#include <server/ServerWebHandler.h>
#include <server/ServerWebSettingsHandler.h>
#include <server/ServerWebAppletHandler.h>
#include <server/ServerCommon.h>
#include <server/ServerAdminHandler.h>
#include <server/ServerWebServerUtil.h>
#include <server/ScorchedServer.h>
#include <net/NetMessagePool.h>
#include <common/OptionsScorched.h>
#include <common/Logger.h>
#include <common/LoggerI.h>
#include <common/Defines.h>

ServerWebServer *ServerWebServer::instance_ = 0;

ServerWebServer *ServerWebServer::instance()
{
	if (!instance_)
	{
		instance_ = new ServerWebServer();
	}
	return instance_;
}

ServerWebServer::ServerWebServer() : 
	netServer_(new NetServerHTTPProtocolRecv),
	logger_(0)
{
	addRequestHandler("/players", new ServerWebHandler::PlayerHandler());
	addRequestHandler("/logs", new ServerWebHandler::LogHandler());
	addRequestHandler("/logfile", new ServerWebHandler::LogFileHandler());
	addRequestHandler("/game", new ServerWebHandler::GameHandler());
	addRequestHandler("/server", new ServerWebHandler::ServerHandler());
	addRequestHandler("/talk", new ServerWebHandler::TalkHandler());
	addRequestHandler("/banned", new ServerWebHandler::BannedHandler());
	addRequestHandler("/mods", new ServerWebHandler::ModsHandler());
	addRequestHandler("/sessions", new ServerWebHandler::SessionsHandler());
	addRequestHandler("/stats", new ServerWebHandler::StatsHandler());
	addRequestHandler("/settingsmain", new ServerWebSettingsHandler::SettingsMainHandler());
	addRequestHandler("/settingsall", new ServerWebSettingsHandler::SettingsAllHandler());
	addRequestHandler("/settingslandscape", new ServerWebSettingsHandler::SettingsLandscapeHandler());
	addRequestHandler("/settingsplayers", new ServerWebSettingsHandler::SettingsPlayersHandler());
	addRequestHandler("/settingsmod", new ServerWebSettingsHandler::SettingsModHandler());

	addRequestHandler("/applet", new ServerWebAppletHandler::AppletHtmlHandler());
	addRequestHandler("/Applet.jar", new ServerWebAppletHandler::AppletFileHandler());
}

ServerWebServer::~ServerWebServer()
{
}

void ServerWebServer::start(int port)
{
	Logger::log(formatString("Starting management web server on port %i", port));
	netServer_.setMessageHandler(this);
	netServer_.start(port);

	if (0 != strcmp(ScorchedServer::instance()->getOptionsGame().
		getServerFileLogger(), "none"))
	{
		logger_ = new FileLogger(
			formatString("ServerWeb-%i-",
				ScorchedServer::instance()->getOptionsGame().getPortNo()));
	}
}

void ServerWebServer::addRequestHandler(const char *url,
	ServerWebServerI *handler)
{
	handlers_[url] = handler;
}

void ServerWebServer::addAsyncRequestHandler(const char *url,
	ServerWebServerAsyncI *handler)
{
	asyncHandlers_[url] = handler;
}

void ServerWebServer::processMessages()
{
	// Check if any delayed messages should be sent
	while (!delayedMessages_.empty())
	{
		unsigned int theTime = (unsigned int) time(0);
		std::pair<unsigned int, NetMessage *> &delayedMessage =
			delayedMessages_.front();
		if (delayedMessage.first <= theTime)
		{
			// Get the message
			NetMessage *message = delayedMessage.second;
			delayedMessages_.pop_front();

			// Send this message now
			netServer_.sendMessageDest(message->getBuffer(), message->getDestinationId());
			NetMessagePool::instance()->addToPool(message);
		}
		else break;
	}

	// Check if any non-delayed messages should be processed
	netServer_.processMessages();

	// Check if anything needs to be done for the async processing
	std::map<unsigned int, AsyncEntry>::iterator asyncItor;
	for (asyncItor = asyncProcesses_.begin();
		asyncItor != asyncProcesses_.end();
		asyncItor++)
	{
		unsigned int destinationId = asyncItor->first;
		unsigned int sid = asyncItor->second.sid;
		ServerWebServerAsyncI *process = asyncItor->second.handler;

		// Ask the async processor to generate the message
		std::string text;
		if (process->processRequest(text))
		{
			// It has generated some text
			// Generate the message to send
			NetMessage *message = NetMessagePool::instance()->getFromPool(
				NetMessage::BufferMessage, destinationId, 0, 0);
			message->getBuffer().addDataToBuffer(text.data(), text.size()); // No null

			// Send this message now
			netServer_.sendMessageDest(message->getBuffer(), message->getDestinationId());
			NetMessagePool::instance()->addToPool(message);

			// Update the session
			std::map <unsigned int, SessionParams>::iterator sessionItor =
				sessions_.find(sid);
			if (sessionItor != sessions_.end())
			{
				sessionItor->second.sessionTime = (unsigned int) time(0);
			}
		}
	}
}

void ServerWebServer::processMessage(NetMessage &message)
{
	if (message.getMessageType() == NetMessage::BufferMessage)
	{
		// We have received a request for the web server

		// Add a NULL to the end of the buffer so we can 
		// do string searches on it and they dont run out of
		// the buffer.
		message.getBuffer().addToBuffer("");
		const char *buffer = message.getBuffer().getBuffer();
		
		// Check it is a GET
		bool ok = false;
		bool get = (strstr(buffer, "GET ") == buffer);
		bool post = (strstr(buffer, "POST ") == buffer);
		if (get || post)
		{
			std::map<std::string, std::string> fields;
			std::map<std::string, NetMessage *> parts;
		
			// Get POST query fields if any
			if (post)
			{
				// Find the end of the header
				char *headerend = (char *) strstr(buffer, "\r\n\r\n");
				if (headerend)
				{
					// Try to find the multipart POST information
					// in the header only
					// (So make the headerend a null to bound the search)
					headerend[0] = '\0';
					const char *findStr = "Content-Type: multipart/form-data; boundary=";
					const char *multipart = strstr(buffer, findStr);
					headerend[0] = '\r';
					if (multipart)
					{
						// We have a multipart message
						// Get the boundry type
						const char *boundryStart = multipart + strlen(findStr);
						char *boundrysep = (char *) strstr(boundryStart, "\r\n");
						if (boundrysep)
						{
							// Get the multi-part boundry
							boundrysep[0] = '\0';
							std::string boundry = boundryStart;
							boundrysep[0] = '\r';

							// Extract the multi-part data from after the header
							headerend += 4; // Skip past /r/n/r/n
							int headersize = headerend - buffer;
							int sizeleft = message.getBuffer().getBufferUsed() - headersize;

							ServerWebServerUtil::extractMultiPartPost(headerend, boundry.c_str(), sizeleft, parts);
						}
					}
					else
					{
						// Extract the query fields from after the header
						headerend += 4; // Skip past /r/n/r/n
						ServerWebServerUtil::extractQueryFields(fields, headerend);
					}
				}
			}
		
			// Check it has a url
			const char *url = buffer + (get?4:5);
			char *eol = (char *) strchr(url, ' ');
			if (eol)
			{
				*eol = '\0';
				if (*url)
				{
					// Get GET query fields if any
					char *sep = (char *) strchr(url, '?');
					if (sep)
					{
						*sep = '\0'; sep++;
						ServerWebServerUtil::extractQueryFields(fields, sep);
					}

					// Add ip address into fields
					fields["ipaddress"] = 
						NetInterface::getIpName(message.getIpAddress());

					// Log info
					if (logger_)
					{
						time_t t = time(0);
						std::string f;
						std::map<std::string, std::string>::iterator itor;
						for (itor = fields.begin();
							itor != fields.end();
							itor++)
						{
							if (0 != strcmp((*itor).first.c_str(), "password"))
							{
								f += formatString("%s=%s ",
									(*itor).first.c_str(),
									(*itor).second.c_str());
							}
						}

						std::string username;
						if (fields.find("sid") != fields.end())
						{
							unsigned int sid = (unsigned int) atoi(fields["sid"].c_str());
							std::map <unsigned int, SessionParams>::iterator findItor =
								sessions_.find(sid);
							if (findItor != sessions_.end())
							{
								username = (*findItor).second.userName;
							}
						}

						LoggerInfo info(
							formatString("%u %s http://%s [%s]", 
							message.getDestinationId(), 
							username.c_str(), url, f.c_str()),
							ctime(&t));
						logger_->logMessage(info);
					}
					
					// Process request
					const char *ipaddress = NetInterface::getIpName(message.getIpAddress());
					ok = processRequest(message.getDestinationId(), ipaddress, url, fields, parts);
				}
			}

			// Add any message parts back to the pool
			std::map<std::string, NetMessage *>::iterator partitor;
			for (partitor = parts.begin();
				partitor != parts.end();
				partitor++)
			{
				NetMessage *newMessage = (*partitor).second;
				NetMessagePool::instance()->addToPool(newMessage);
			}
		}

		if (!ok)
		{
			// Disconnect the client
			netServer_.disconnectClient(message.getDestinationId());
		}
	}
	else if (message.getMessageType() == NetMessage::SentMessage)
	{
		// Check if this is a sync or async destination
		std::map<unsigned int, AsyncEntry>::iterator itor = 
			asyncProcesses_.find(message.getDestinationId());
		if (itor != asyncProcesses_.end())
		{
			// Its an async destination do nothing
		}
		else
		{
			// Its a sync destination
			// Once a sync message has been fully sent close the connection
			netServer_.disconnectClient(message.getDestinationId());
		}
	}
	else if (message.getMessageType() == NetMessage::DisconnectMessage)
	{
		// Remove any async processes we may be processing for this 
		// destination
		std::map<unsigned int, AsyncEntry>::iterator itor = 
			asyncProcesses_.find(message.getDestinationId());
		if (itor != asyncProcesses_.end())
		{
			delete itor->second.handler;
			asyncProcesses_.erase(itor);
		}
	}
}

bool ServerWebServer::processRequest(
	unsigned int destinationId,
	const char *ip,
	const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts)
{
	bool delayed = false; // Set delayed on authentication failure
	std::string text;
	if (0 == strcmp(url, "/"))
	{
		// We have requested the login page
		// Have the login credentials been supplied
		if (validateUser(ip, url, fields))
		{
			// Yes, and credentials are correct
			// Show the starting (players) page
			ServerWebServerUtil::getHtmlRedirect(formatString("/players?sid=%s", fields["sid"].c_str()), text);
		}
		else
		{
			// No, or credentials are not correct
			// Show the login page after a delay
			if (!ServerWebServerUtil::getHtmlTemplate("login.html", fields, text)) return false;
			delayed = true;
		}
	}
	else
	{
		// A "normal" page has been requested
		// Check the session is valid
		unsigned int sid = validateSession(ip, url, fields);
		if (sid)
		{
			// The session is valid, show the page
			if (!generatePage(destinationId, sid, url, fields, parts, text)) return false;
		}
		else
		{
			// The session is invalid show the login page after a delay
			ServerWebServerUtil::getHtmlRedirect("/", text);
			delayed = true;
		}
	}

	// Generate the message to send
	NetMessage *message = NetMessagePool::instance()->getFromPool(
		NetMessage::BufferMessage, destinationId, 0, 0);
	message->getBuffer().addDataToBuffer(text.data(), text.size()); // No null
	if (delayed)
	{
		// Generate an outgoing message, that will be sent after a time delay
		unsigned int delayedTime = (unsigned int) time(0) + 5;
		std::pair<unsigned int, NetMessage *> delayedMessage(delayedTime, message);
		delayedMessages_.push_back(delayedMessage);
	}
	else
	{
		// Send this message now
		netServer_.sendMessageDest(message->getBuffer(), message->getDestinationId());
		NetMessagePool::instance()->addToPool(message);
	}
	
	return true;
}

unsigned int ServerWebServer::validateSession(
	const char *ip,
	const char *url,
	std::map<std::string, std::string> &fields)
{
	const unsigned int SessionTimeOut = 60 * 15;
	unsigned int currentTime = (unsigned int) time(0);

	// Tidy expired sessions
	std::map <unsigned int, SessionParams>::iterator sitor;
	for (sitor = sessions_.begin();
		sitor != sessions_.end();
		sitor++)
	{
		SessionParams &params = (*sitor).second;
		if (currentTime > params.sessionTime + SessionTimeOut)
		{
			sessions_.erase(sitor);
			break;
		}
	}

	// Check if user has a valid session
	if (fields.find("sid") != fields.end())
	{
		unsigned int sid = (unsigned int) atoi(fields["sid"].c_str());
		std::map <unsigned int, SessionParams>::iterator findItor =
			sessions_.find(sid);
		if (findItor != sessions_.end())
		{
			SessionParams &params = (*findItor).second;
			if (currentTime < params.sessionTime + SessionTimeOut)
			{
				params.sessionTime = currentTime;
				return sid;
			}
			else
			{
				sessions_.erase(findItor);
			}
		}
	}

	return 0;
}

bool ServerWebServer::validateUser(
	const char *ip,
	const char *url,
	std::map<std::string, std::string> &fields)
{
	unsigned int currentTime = (unsigned int) time(0);
	bool authenticated = false;

	// Check if this is a local user
	if (0 == strcmp(ip, "127.0.0.1"))
	{
		fields["name"] = "localconnection";
		authenticated = true;
	}
	else
	{
		// Check if user has a valid username and password
		if (fields.find("name") != fields.end() ||
			fields.find("password") != fields.end()) 
		{
			if (ServerAdminHandler::instance()->login(
				fields["name"].c_str(),
				fields["password"].c_str()))
			{
				authenticated = true;
			}
		}
	}

	// Create a session for the authenticated user
	if (authenticated)
	{
		unsigned int sid = 0;

		// Try to find an existing session for this user
		std::map<unsigned int, SessionParams>::iterator itor;
		for (itor = sessions_.begin();
			itor != sessions_.end();
			itor++)
		{
			SessionParams &params = (*itor).second;
			if (0 == strcmp(params.userName.c_str(), fields["name"].c_str()))
			{
				// Found one
				sid = (*itor).first;
				break;
			}
		}

		// Generate a sid if we didn't find an existing login
		if (sid == 0)
		{
			// Generate a new unique session id
			do 
			{
				sid = rand();
			} while (sessions_.find(sid) != sessions_.end());
		}

		// Update the session params
		SessionParams params;
		params.sessionTime = currentTime;
		params.userName = fields["name"];
		params.ipAddress = ip;
		sessions_[sid] = params;

		// Set the sid for use in the html templates
		fields["sid"] = formatString("%u", sid);

		ServerCommon::sendString(0,
			formatString("server admin \"%s\" logged in",
			fields["name"].c_str()));
	}

	return authenticated;
}

bool ServerWebServer::generatePage(
	unsigned int destinationId,
	unsigned int sid,
	const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	{
		// First check for an async handler
		std::map<std::string, ServerWebServerAsyncI *>::iterator itor =
			asyncHandlers_.find(url);
		if (itor != asyncHandlers_.end())
		{
			ServerWebServerAsyncI *handler = (*itor).second;

			AsyncEntry entry;
			entry.handler = handler->create();
			entry.sid = sid;
			asyncProcesses_[destinationId] = entry;
			return true;
		}
	}
	{
		// Then check for a sync handler
		std::map <std::string, ServerWebServerI *>::iterator itor =
			handlers_.find(url);
		if (itor != handlers_.end())
		{
			ServerWebServerI *handler = (*itor).second;
			return handler->processRequest(url, fields, parts, text);
		}
	}
	return false; // No handler
}

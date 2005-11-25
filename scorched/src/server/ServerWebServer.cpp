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
#include <server/ServerCommon.h>
#include <server/ServerAdminHandler.h>
#include <server/ScorchedServer.h>
#include <common/OptionsGame.h>
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
	addRequestHandler("/", new ServerWebHandler::PlayerHandler());
	addRequestHandler("/logs", new ServerWebHandler::LogHandler());
	addRequestHandler("/game", new ServerWebHandler::GameHandler());
	addRequestHandler("/settings", new ServerWebHandler::SettingsHandler());
	addRequestHandler("/talk", new ServerWebHandler::TalkHandler());
	addRequestHandler("/banned", new ServerWebHandler::BannedHandler());
	addRequestHandler("/mods", new ServerWebHandler::ModsHandler());
}

ServerWebServer::~ServerWebServer()
{
}

void ServerWebServer::start(int port)
{
	Logger::log("Starting management web server on port %i", port);
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

void ServerWebServer::processMessages()
{
	netServer_.processMessages();
}

static void extractQueryFields(std::map<std::string, std::string> &fields, char *sep)
{
	char *token = strtok(sep, "&");
	while(token)
	{
		char *eq = strchr(token, '=');
		if (eq)
		{
			*eq = '\0';
			std::string value;
			for (const char *valueStr = (eq + 1); *valueStr; valueStr++)
			{
				char c = *valueStr;
				if (c == '+') c = ' ';
				else if (c == '%')
				{
					char buf[3] = { 0, 0, 0 };

					buf[0] = *(valueStr + 1);
					if (!buf[0]) break;
					buf[1] = *(valueStr + 2);
					if (!buf[1]) break;

					c = (char) strtol(buf, 0, 16);

					valueStr += 2;
				}

				if (c != '\r') value += c;
			}

			if (fields.find(token) == fields.end())
			{
				fields[token] = value;
			}
			*eq = '=';
		}				
		token = strtok(0, "&");
	}
}

void ServerWebServer::processMessage(NetMessage &message)
{
	if (message.getMessageType() == NetMessage::BufferMessage)
	{
		// We have received a request for the web server
		message.getBuffer().addToBuffer("");
		const char *buffer = message.getBuffer().getBuffer();
		
		// Check it is a GET
		bool ok = false;
		bool get = (strstr(buffer, "GET ") == buffer);
		bool post = (strstr(buffer, "POST ") == buffer);
		if (get || post)
		{
			std::map<std::string, std::string> fields;
		
			// Get POST query fields if any
			if (post)
			{
				char *sep = strstr(buffer, "\r\n\r\n");
				if (sep)
				{
					sep += 4;
					extractQueryFields(fields, sep);
				}
			}
		
			// Check it has a url
			const char *url = buffer + (get?4:5);
			char *eol = strchr(url, ' ');
			if (eol)
			{
				*eol = '\0';
				if (*url)
				{
					// Get GET query fields if any
					char *sep = strchr(url, '?');
					if (sep)
					{
						*sep = '\0'; sep++;
						extractQueryFields(fields, sep);
					}

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
							f += formatString("%s=%s ",
								(*itor).first.c_str(),
								(*itor).second.c_str());
						}
						LoggerInfo info(LoggerInfo::TypeNormal,
							formatString("%u:%s:%s", 
							message.getDestinationId(), 
							url, f.c_str()),
							ctime(&t));
						logger_->logMessage(info);
					}
					
					// Process request
					ok = processRequest(message.getDestinationId(), url, fields);
				}
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
		netServer_.disconnectClient(message.getDestinationId());
	}
}

bool ServerWebServer::processRequest(
	unsigned int destinationId,
	const char *url,
	std::map<std::string, std::string> &fields)
{
	std::string text;
	if (validateUser(fields))
	{
		if (!generatePage(url, fields, text)) return false;
	}
	else
	{
		if (!getTemplate("login.html", fields, text)) return false;
	}
	
	const char *header = 
		"HTTP/1.1 200 OK\r\n"
		"Server: Scorched3D\r\n"
		"Content-Type: text/html\r\n"
		"Connection: Close\r\n"
		"\r\n";

	NetBuffer buffer;
	buffer.addDataToBuffer(header, strlen(header)); // No null
	buffer.addDataToBuffer(text.c_str(), text.size()); // No null
	netServer_.sendMessage(buffer, destinationId);
	
	return true;
}

bool ServerWebServer::validateUser(
	std::map<std::string, std::string> &fields)
{
	const unsigned int SessionTimeOut = 60 * 15;
	unsigned int currentTime = time(0);

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
				return true;
			}
			else
			{
				sessions_.erase(findItor);
			}
		}
	}

	// Check if user has a valid username and password
	if (fields.find("name") != fields.end() ||
		fields.find("password") != fields.end()) 
	{
		if (ServerAdminHandler::instance()->login(
			fields["name"].c_str(),
			fields["password"].c_str()))
		{
			SessionParams params;
			params.sessionTime = currentTime;

			unsigned int sid = rand();
			sessions_[sid] = params;
			fields["sid"] = formatString("%u", sid);

			ServerCommon::sendString(0,
				"server admin \"%s\" logged in",
				fields["name"].c_str());
			ServerCommon::serverLog(0,
				"server admin \"%s\" logged in",
				fields["name"].c_str());

			return true;
		}
	}

	return false;
}

bool ServerWebServer::generatePage(
	const char *url,
	std::map<std::string, std::string> &fields,
	std::string &text)
{
	std::map <std::string, ServerWebServerI *>::iterator itor =
		handlers_.find(url);
	if (itor == handlers_.end()) return false;

	ServerWebServerI *handler = (*itor).second;
	return handler->processRequest(url, fields, text);
}

bool ServerWebServer::getTemplate(
	const char *name,
	std::map<std::string, std::string> &fields,
	std::string &result)
{
	// Perhaps cache this
	const char *fileName = getDataFile("data/html/server/%s", name);
	FILE *in = fopen(fileName, "ra");
	if (!in) 
	{
		Logger::log("ERROR: Failed to open web template \"%s\"", fileName);
		return false;
	}

	char buffer[1024], include[256];
	while (fgets(buffer, 1024, in))
	{
		// Check for an include line
		if (sscanf(buffer, "#include %s",
			include) == 1)
		{
			// Add the included file
			std::string tmp;
			if (!getTemplate(include, fields, tmp))
			{
				return false;
			}

			result += tmp;
		}
		else
		{
			// Check for any value replacements
			char *position = buffer;
			for (;;)
			{
				char *start, *end;
				if ((start = strstr(position, "[[")) &&
					(end = strstr(position, "]]")) &&
					(end > start))
				{
					// Replace the text [[name]] with the value
					*start = '\0';
					*end = '\0';
					result += position;
					position = end + 2;

					char *name = start + 2;

					// First check to see if it is in the supplied fields
					if (fields.find(name) != fields.end())
					{
						result += fields[name];
					}
					else
					{
						// Then in the scorched3d settings
						std::list<OptionEntry *>::iterator itor;
						std::list<OptionEntry *> &options = 
							ScorchedServer::instance()->getOptionsGame().getOptions();
						for (itor = options.begin();
							itor != options.end();
							itor++)
						{
							OptionEntry *entry = (*itor);
							if ((strcmp(entry->getName(), name) == 0) &&
								!(entry->getData() & OptionEntry::DataProtected))
							{
								result += entry->getValueAsString();
							}
						}
					}
				}
				else
				{
					// No replacements
					result += position;
					break;
				}
			}
		}
	}
	fclose(in);

	return true;
}


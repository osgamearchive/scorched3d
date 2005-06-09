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

#include <server/ServerWebServer.h>
#include <server/ScorchedServer.h>
#include <server/ServerAdminHandler.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <common/LoggerI.h>
#include <string.h>
#include <time.h>

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

void ServerWebServer::processMessages()
{
	netServer_.processMessages();
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
		if (strstr(buffer, "GET ") == buffer)
		{
			// Check it has a url
			const char *url = buffer + 4;
			char *eol = strchr(url, ' ');
			if (eol)
			{
				*eol = '\0';
				if (*url)
				{
					// Log info
					if (logger_)
					{
						time_t t = time(0);
						LoggerInfo info(LoggerInfo::TypeNormal,
							url,
							ctime(&t));
						logger_->logMessage(info);
					}
				
					// Get query fields if any
					std::map<std::string, std::string> fields;
					char *sep = strchr(url, '?');
					if (sep)
					{
						*sep = '\0'; sep++;
						
						char *token = strtok(sep, "&");
						while(token)
						{
							char *eq = strchr(token, '=');
							if (eq)
							{
								*eq = '\0';
								fields[token] = (eq + 1);
								*eq = '=';
							}
							
							token = strtok(0, "&");
						}
					}
					
					// Process request
					ok = processRequest(message.getDestinationId(), url, fields);
				}
			}
		}

		// Disconnect the client
		netServer_.disconnectClient(message.getDestinationId(), ok);
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
	if (fields.find("name") == fields.end() ||
		fields.find("password") == fields.end()) return false;

	return ServerAdminHandler::instance()->login(
		fields["name"].c_str(),
		fields["password"].c_str());
}

bool ServerWebServer::generatePage(
	const char *url,
	std::map<std::string, std::string> &fields,
	std::string &text)
{
	text += "<html><body>Hello World!</body></html>";

	return true;
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


////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <server/ServerWebAppletHandler.h>
#include <server/ServerWebServer.h>

bool ServerWebAppletHandler::AppletFileHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	// Get file
	const char *file = getDataFile(formatString("data/html/server/binary/%s", url));
	if (!file) return false;

	// Read file contents
	std::string contents;
	FILE *in = fopen(file, "rb");
	if (!in) return false;
	int read = 0;
	char buffer[256];
	while (read = fread(buffer, 1, 256, in))
	{
		contents.append(buffer, read);
	}
	fclose(in);

	// Return file
	unsigned int dataSize = contents.size();
	text.append(formatString(
		"HTTP/1.1 200 OK\r\n"
		"Server: Scorched3D\r\n"
		"Content-Length: %u\r\n"
		"Content-Type: application/octet-stream\r\n"
		"Connection: Close\r\n"
		"\r\n", dataSize));
	text.append(contents.data(), dataSize);

	return true;
}

bool ServerWebAppletHandler::AppletHtmlHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	return ServerWebServer::getHtmlTemplate("applet.html", fields, text);
}

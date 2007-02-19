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

#include <engine/SyncCheck.h>
#include <engine/ScorchedContext.h>
#include <common/Logger.h>

SyncCheck *SyncCheck::instance()
{
	static SyncCheck instance_;
	return &instance_;
}

SyncCheck::SyncCheck() :
	clientFile("ClientSyncLog-"),
	serverFile("ServerSyncLog-")
{
}

SyncCheck::~SyncCheck()
{
}

void SyncCheck::addString(ScorchedContext &context, const char *string)
{
	if (context.serverMode)
	{
		server.push_back(string);
		LoggerInfo info(string);
		serverFile.logMessage(info);

		if (client.size() >= server.size())
		{
			const char *serverStr = server[server.size() - 1].c_str();
			const char *clientStr = client[server.size() - 1].c_str();
			if (0 != strcmp(serverStr, clientStr))
			{
				Logger::log(formatString("Synccheck error\nC:%s\nS:%s",
					clientStr, serverStr));
			}
		}
	}
	else
	{
		client.push_back(string);
		LoggerInfo info(string);
		clientFile.logMessage(info);

		if (server.size() >= client.size())
		{
			const char *serverStr = server[client.size() - 1].c_str();
			const char *clientStr = client[client.size() - 1].c_str();
			if (0 != strcmp(serverStr, clientStr))
			{
				Logger::log(formatString("Synccheck error\nC:%s\nS:%s",
					clientStr, serverStr));
			}
		}
	}
}


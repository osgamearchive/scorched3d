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

#include <common/StatsLoggerFile.h>
#include <common/StatsLoggerMySQL.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <server/ScorchedServer.h>
#include <stdlib.h>

StatsLogger *StatsLogger::instance_ = 0;

StatsLogger *StatsLogger::instance()
{
	if (!instance_ &&
		!OptionsParam::instance()->getDedicatedServer())
	{
		instance_ = new StatsLoggerNone;
	}


	if (!instance_)
	{
		const char *statsLogger =
			ScorchedServer::instance()->getOptionsGame().getStatsLogger();
		if (strcmp(statsLogger, "mysql") == 0)
		{
#ifdef HAVE_MYSQL
			instance_ = new StatsLoggerMySQL;
			Logger::log(0, "Created mysql stats logger.");
#else
			dialogExit("StatsLogger",
				"Atempted to create mysql stats logger\n"
				"but mysql support has not been compiled into this\n"
				"scorched3d binary.");
#endif
		}
		else if (strcmp(statsLogger, "file") == 0)
		{
			Logger::log(0, "Created file stats logger.");
			instance_ = new StatsLoggerFile;
		}
		else
		{
			Logger::log(0, "Created null stats logger.");
			instance_ = new StatsLoggerNone;
		}
	}
	return instance_;
}

StatsLogger::StatsLogger()
{

}

StatsLogger::~StatsLogger()
{
}


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

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include <string>
#include <list>
#include <tank/TankContainer.h>

class LoggerI
{
public:
	virtual void logMessage(
		const char *time,
		const char *message,
		unsigned int playerId) = 0;
};

// ************************************************
// NOTE: This logger is and needs to be thread safe
// ************************************************

class Logger
{
public:
	static Logger *instance();

	static void addLogger(LoggerI *logger);
	static void remLogger(LoggerI *logger);
	static void processLogEntries();
	static void log(unsigned int playerId, const char *fmt, ...);

protected:
	static Logger *instance_;
	struct LogEntry
	{
		std::string time_;
		std::string message_;
		unsigned int playerId_;
	};

	std::list<LoggerI *> loggers_;
	std::list<LogEntry> entries_;

	static void addLog(char *time, char *text, unsigned int playerId);


private:
	Logger();
	virtual ~Logger();
};

class FileLogger : public LoggerI
{
public:
	FileLogger(TankContainer &tankContainer, const char *fileName);
	virtual ~FileLogger();

	virtual void logMessage(
		const char *time,
		const char *message,
		unsigned int playerId);

protected:
	std::string fileName_;
	FILE *logFile_;
	TankContainer &tankContainer_;
	unsigned int lines_;

	void openFile(const char *fileName);

};

#endif /* _LOGGER_H_ */

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

class GLTexture;
class LoggerInfo
{
public:
	LoggerInfo(unsigned int pId = 0,
		GLTexture *icon = 0);

	unsigned int playerId;
	GLTexture *icon;
};

class LoggerI
{
public:
	virtual void logMessage(
		const char *time,
		const char *message,
		const LoggerInfo &info) = 0;
};

// ************************************************
// NOTE: This logger is and needs to be thread safe
// ************************************************

class Logger
{
public:
	static Logger *instance();

	static LoggerInfo defaultInfo;
	static void addLogger(LoggerI *logger);
	static void remLogger(LoggerI *logger);
	static void processLogEntries();
	static void log(const LoggerInfo &info, const char *fmt, ...);

protected:
	static Logger *instance_;
	struct LogEntry
	{
		std::string time_;
		std::string message_;
		LoggerInfo info_;
	};

	std::list<LoggerI *> loggers_;
	std::list<LogEntry> entries_;

	static void addLog(char *time, char *text, 
		const LoggerInfo &info);

private:
	Logger();
	virtual ~Logger();
};

class FileLogger : public LoggerI
{
public:
	FileLogger(const char *fileName);
	virtual ~FileLogger();

	virtual void logMessage(
		const char *time,
		const char *message,
		const LoggerInfo &info);

protected:
	std::string fileName_;
	FILE *logFile_;
	unsigned int lines_;

	void openFile(const char *fileName);

};

#endif /* _LOGGER_H_ */

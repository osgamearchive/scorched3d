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

#include <common/Defines.h>
#include <common/Logger.h>
#include <tank/TankContainer.h>
#include <SDL/SDL.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <algorithm>

// ************************************************
// NOTE: This logger is and needs to be thread safe
// ************************************************

static SDL_mutex *logMutex_ = 0;
Logger * Logger::instance_ = 0;

Logger * Logger::instance()
{
	if (!instance_)
	{
		instance_ = new Logger;
	}
	return instance_;
}

Logger::Logger()
{
	if (!logMutex_) logMutex_ = SDL_CreateMutex();
}

Logger::~Logger()
{
}

void Logger::addLogger(LoggerI *logger)
{
	Logger::instance();
	SDL_LockMutex(logMutex_);
	Logger::instance()->loggers_.push_back(logger);
	SDL_UnlockMutex(logMutex_);
}

void Logger::remLogger(LoggerI *logger)
{
	Logger::instance();
	std::list<LoggerI *>::iterator itor;
	SDL_LockMutex(logMutex_);
	itor = Logger::instance()->loggers_.begin();
	while(itor != Logger::instance()->loggers_.end()) {
		if (*itor == logger) {
			Logger::instance()->loggers_.erase(itor);
			break;
		}
		itor++;
	}
	SDL_UnlockMutex(logMutex_);
}

void Logger::log(unsigned int playerId, const char *fmt, ...)
{
	Logger::instance();

	SDL_LockMutex(logMutex_);
	static char text[2048];

	// Add the time to the beginning of the log message
	time_t theTime = time(0);
	char *time = ctime(&theTime); 
	char *nl = strchr(time, '\n'); 
	if (nl) *nl = '\0';
    
	// Add the actual log message
	va_list ap;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	// Add single or multiple lines
	char *found = strchr(text, '\n');
	char *start = text;
	if (found)
	{
		while (found)
		{
			*found = '\0';
			addLog(time, start, playerId);
			start = found;
			start++;

			found = strchr(start, '\n');
		}
		if (start[0] != '\0') addLog(time, start, playerId);
	}
	else
	{
		addLog(time, text, playerId);
	}
	SDL_UnlockMutex(logMutex_);
}

void Logger::addLog(char *time, char *text, unsigned int playerId)
{
	Logger::instance();

	instance_->entries_.push_back(LogEntry());
	LogEntry &lastEntry = instance_->entries_.back();

	lastEntry.time_ = time;
	lastEntry.message_ = text;
	lastEntry.playerId_ = playerId;
}

void Logger::processLogEntries()
{
	Logger::instance();

	SDL_LockMutex(logMutex_);
	std::list<LogEntry> &entries = Logger::instance()->entries_;
	while (!entries.empty())
	{
		LogEntry &firstEntry = entries.front();

		std::list<LoggerI *> &loggers = Logger::instance()->loggers_;
		std::list<LoggerI *>::iterator logItor;
		for (logItor = loggers.begin();
			logItor != loggers.end();
			logItor++)
		{
			LoggerI *log = (*logItor);
			log->logMessage(
				firstEntry.time_.c_str(), 
				firstEntry.message_.c_str(), 
				firstEntry.playerId_);
		}

		entries.pop_front();
	}
	SDL_UnlockMutex(logMutex_);
}

FileLogger::FileLogger(const char *fileName) : 
	lines_(0), logFile_(0), fileName_(fileName)
{

}

FileLogger::~FileLogger()
{
}

void FileLogger::logMessage(
		const char *time,
		const char *message,
		unsigned int playerId)
{
	const unsigned int MaxLines = 4000;
	if (!logFile_ || (lines_++>MaxLines)) openFile(fileName_.c_str());
	if (!logFile_) return;

	// Log to file and flush file
	fprintf(logFile_, "%s - %s\n", time, message);
	fflush(logFile_);
}

void FileLogger::openFile(const char *fileName)
{
	lines_ = 0;
	if (logFile_) fclose(logFile_);
	logFile_ = fopen(getLogFile("%s-%i.log", fileName, time(0)), "w");
}


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


// Logger.cpp: implementation of the Timer class.
//
//////////////////////////////////////////////////////////////////////

#include <common/Defines.h>
#include <common/Logger.h>
#include <tank/Tank.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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

}

Logger::~Logger()
{
}

void Logger::addLogger(LoggerI *logger)
{
	Logger::instance()->loggers_.push_back(logger);
}

void Logger::log(Tank *src, const char *fmt, ...)
{
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
			addLog(time, start, src);
			start = found;
			start++;

			found = strchr(start, '\n');
		}
		if (start[0] != '\0') addLog(time, start, src);
	}
	else
	{
		addLog(time, text, src);
	}
}

void Logger::addLog(char *time, char *text, Tank *src)
{
	std::list<LoggerI *> &loggers = Logger::instance()->loggers_;
	std::list<LoggerI *>::iterator itor;
	for (itor = loggers.begin();
		itor != loggers.end();
		itor++)
	{
		LoggerI *log = (*itor);
		log->logMessage(time, text, src);
	}
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
		Tank *source)
{
	const unsigned int MaxLines = 1000;
	if (!logFile_ || (lines_++>MaxLines)) openFile(fileName_.c_str());
	if (!logFile_) return;

	// Log to file and flush file
	fprintf(logFile_, "%s - %s %c%s%c\n", time, 
		message, source?'[':' ',source?source->getUniqueId():"",source?']':' ');
	fflush(logFile_);
}

void FileLogger::openFile(const char *fileName)
{
	lines_ = 0;
	if (logFile_) fclose(logFile_);
	char buffer[128];
	sprintf(buffer, "%s-%i.log", fileName, time(0));

	logFile_ = fopen(buffer, "w");
}

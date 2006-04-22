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

#include <common/FileLogger.h>
#include <common/Defines.h>
#include <time.h>

FileLogger::FileLogger(const char *fileName) : 
	lines_(0), logFile_(0), fileName_(fileName)
{

}

FileLogger::~FileLogger()
{
}

void FileLogger::logMessage(LoggerInfo &info)
{
	const unsigned int MaxLines = 4000;
	if (!logFile_ || (lines_++>MaxLines)) openFile(fileName_.c_str());
	if (!logFile_) return;

	// Log to file and flush file
	fprintf(logFile_, "%s - %s\n", info.getTime(), info.getMessage());
	fflush(logFile_);
}

void FileLogger::openFile(const char *fileName)
{
	lines_ = 0;
	if (logFile_) fclose(logFile_);

	time_t theTime = time(0);
	struct tm *newtime = localtime(&theTime); 

	logFile_ = fopen(getLogFile(formatString("%s-%02i%02i%i-%02i%02i%02i.log", fileName, 
		newtime->tm_mday, (newtime->tm_mon + 1), (newtime->tm_year + 1900),
		newtime->tm_hour, newtime->tm_min, newtime->tm_sec)), "w");
}

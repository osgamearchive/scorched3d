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
#include <server/ScorchedServer.h>
#include <common/OptionsGame.h>

StatsLoggerFile::StatsLoggerFile() : statsLogger_(0)
{

}

StatsLoggerFile::~StatsLoggerFile()
{
}

void StatsLoggerFile::createLogger()
{
	if (!statsLogger_)
	{
		char buffer[256];
		sprintf(buffer, "StatsLog-%i-", 
			ScorchedServer::instance()->getOptionsGame().getPortNo());
		statsLogger_ = new FileLogger(buffer);
	}
}

void StatsLoggerFile::gameStart(std::list<Tank *> &tanks)
{
	createLogger();
	if (!statsLogger_) return;

	time_t theTime = time(0);
	char *time = ctime(&theTime); 
	char *nl = strchr(time, '\n'); 
	if (nl) *nl = '\0';

	statsLogger_->logMessage(time, "startgame", 0);
}

void StatsLoggerFile::roundStart(std::list<Tank *> &tanks)
{
	createLogger();
	if (!statsLogger_) return;

	time_t theTime = time(0);
	char *time = ctime(&theTime); 
	char *nl = strchr(time, '\n'); 
	if (nl) *nl = '\0';

	statsLogger_->logMessage(time, "startround", 0);
}

void StatsLoggerFile::tankRank(Tank *firedTank)
{
	createLogger();
	if (!statsLogger_) return;
                                                                                                          
	time_t theTime = time(0);
	char *time = ctime(&theTime);
	char *nl = strchr(time, '\n');
	if (nl) *nl = '\0';
	
	statsLogger_->logMessage(time, "tankrank", 0);
}

void StatsLoggerFile::tankFired(Tank *firedTank, Weapon *weapon)
{
	createLogger();
	if (!statsLogger_) return;

	time_t theTime = time(0);
	char *time = ctime(&theTime); 
	char *nl = strchr(time, '\n'); 
	if (nl) *nl = '\0';

	char message[1024];
	sprintf(message, "fired \"%s\" [%s] \"%s\"", 
		firedTank->getName(), firedTank->getUniqueId(),
		weapon->getName());
	statsLogger_->logMessage(time, message, 0);
}

void StatsLoggerFile::tankResigned(Tank *resignedTank)
{
	createLogger();
	if (!statsLogger_) return;

	time_t theTime = time(0);
	char *time = ctime(&theTime); 
	char *nl = strchr(time, '\n'); 
	if (nl) *nl = '\0';

	char message[1024];
	sprintf(message, "resigined \"%s\" [%s]", 
		resignedTank->getName(), resignedTank->getUniqueId());
	statsLogger_->logMessage(time, message, 0);
}

void StatsLoggerFile::tankJoined(Tank *tank)
{
	createLogger();
	if (!statsLogger_) return;

	time_t theTime = time(0);
	char *time = ctime(&theTime); 
	char *nl = strchr(time, '\n'); 
	if (nl) *nl = '\0';

	char message[1024];
	sprintf(message, "joined \"%s\" [%s]", 
		tank->getName(), tank->getUniqueId());
	statsLogger_->logMessage(time, message, 0);
}

void StatsLoggerFile::tankLeft(Tank *tank)
{
	createLogger();
	if (!statsLogger_) return;

	time_t theTime = time(0);
	char *time = ctime(&theTime); 
	char *nl = strchr(time, '\n'); 
	if (nl) *nl = '\0';

	char message[1024];
	sprintf(message, "left \"%s\" [%s]", 
		tank->getName(), tank->getUniqueId());
	statsLogger_->logMessage(time, message, 0);
}

void StatsLoggerFile::tankKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon)
{
	createLogger();
	if (!statsLogger_) return;

	time_t theTime = time(0);
	char *time = ctime(&theTime); 
	char *nl = strchr(time, '\n'); 
	if (nl) *nl = '\0';

	char message[1024];
	sprintf(message, "killed \"%s\" [%s] \"%s\" [%s] \"%s\"", 
		firedTank->getName(), firedTank->getUniqueId(),
		deadTank->getName(), deadTank->getUniqueId(),
		weapon->getName());
	statsLogger_->logMessage(time, message, 0);
}

void StatsLoggerFile::tankTeamKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon)
{
	createLogger();
	if (!statsLogger_) return;

	time_t theTime = time(0);
	char *time = ctime(&theTime); 
	char *nl = strchr(time, '\n'); 
	if (nl) *nl = '\0';

	char message[1024];
	sprintf(message, "teamkilled \"%s\" [%s] \"%s\" [%s] \"%s\"", 
		firedTank->getName(), firedTank->getUniqueId(),
		deadTank->getName(), deadTank->getUniqueId(),
		weapon->getName());
	statsLogger_->logMessage(time, message, 0);
}

void StatsLoggerFile::tankSelfKilled(Tank *firedTank, Weapon *weapon)
{
	createLogger();
	if (!statsLogger_) return;

	time_t theTime = time(0);
	char *time = ctime(&theTime); 
	char *nl = strchr(time, '\n'); 
	if (nl) *nl = '\0';

	char message[1024];
	sprintf(message, "selfkilled \"%s\" [%s] \"%s\"", 
		firedTank->getName(), firedTank->getUniqueId(),
		weapon->getName());
	statsLogger_->logMessage(time, message, 0);
}

void StatsLoggerFile::tankWon(Tank *tank)
{
	createLogger();
	if (!statsLogger_) return;

	time_t theTime = time(0);
	char *time = ctime(&theTime); 
	char *nl = strchr(time, '\n'); 
	if (nl) *nl = '\0';

	char message[1024];
	sprintf(message, "won \"%s\" [%s]", 
		tank->getName(), tank->getUniqueId());
	statsLogger_->logMessage(time, message, 0);
}

void StatsLoggerFile::tankOverallWinner(Tank *tank)
{
	createLogger();
	if (!statsLogger_) return;

	time_t theTime = time(0);
	char *time = ctime(&theTime); 
	char *nl = strchr(time, '\n'); 
	if (nl) *nl = '\0';

	char message[1024];
	sprintf(message, "overallwinner \"%s\" [%s]", 
		tank->getName(), tank->getUniqueId());
	statsLogger_->logMessage(time, message, 0);
}

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

#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <common/Defines.h>

OptionsGame::OptionsGame() :
	teams_(options_, "Teams",
		"The number of teams (1 == no teams)", 0, 1, 1, 2),
	maxArmsLevel_(options_, "MaxArmsLevel",
		"The largest weapon type allowed", 0, 10, 0, 10),
	maxNumberWeapons_(options_, "MaxNumberWeapons",
		"The number of each weapon the player is allowed", 0, 90),
	maxLandscapeSize_(options_, "MaxLandscapeSize",
		"The largest landscape (in bytes) that will be sent to the clients", 0, 200000),
	shotTime_(options_, "ShotTime", 
		"The amount of time each player has for each shot", 0, 0),
	buyingTime_(options_, "BuyingTime", 
		"The amount of time each player has to buy weapons and use auto defense", 0, 0),
	allowedMissedMoves_(options_, "AllowedMissedMoves",
		"The number of moves a player is allowed to miss (due to the shot timer)", 0, 3),
	scoreTime_(options_, "ScoreTime", 
		"The amount of time to show the end of game scores for", 0, 15),
	idleKickTime_(options_, "IdleKickTime",
		"The amount of time to give clients to respond before kicking them", 0, 30),
	maxRoundTurns_(options_, "MaxNumberOfRoundTurns",
		"The maximum number of turns all players are allowed in each round (0 = infinite)", 0, 15),
	numberOfRounds_(options_, "NumberOfRounds", 
		"The number of rounds to play in each game", 0, 5),
	numberOfPlayers_(options_, "NumberOfPlayers", 
		"The maximum number of players to allow", 0, 2, 1, 24),
	numberOfMinPlayers_(options_, "NumberOfMinPlayers", 
		"The minimum number of players to allow", 0, 2, 2, 24),
	limitPowerByHealth_(options_, "LimitPowerByHealth", 
		"Show power is limited by the amount of health a player has", 0, true),
	autoBallanceTeams_(options_, "AutoBallanceTeams",
		"Automatically ballance teams at the start of each round", 0, true),
	moneyPerHealthPoint_(options_, "MoneyPerHealthPoint",
		"The money awarded is proportional to the amount of health removed", 0, true),
	turnType_(options_, "TurnType", 
		"The player turn mode", 0, 	int(TurnSequentialLooserFirst), int(TurnSimultaneous), int(TurnSequentialRandom)), // Data, default, min, max
	moneyBuyOnRound_(options_, "MoneyBuyOnRound", 
		"The first round players are allowed to buy on", 0 ,2),
	moneyWonForRound_(options_, "MoneyWonForRound", 
		"The money awarded for each won round", 0, 5000),
	moneyPerHitPoint_(options_, "MoneyWonPerHitPoint", 
		"The money awarded for each person hit * weapon used", 0, 100),
	moneyPerKillPoint_(options_, "MoneyWonPerKillPoint", 
		"The money awarded for each person killed * weapon used", 0, 500),
	moneyStarting_(options_, "MoneyStarting", 
		"The money each player starts with", 0, 10000, 0, 500000),
	moneyInterest_(options_, "MoneyInterest", 
		"The interest awarded at the end of each round", 0, 15, 0, 100),
	scoreType_(options_, "ScoreType",
		"How the winnder is choosen", 0, int(ScoreWins), int(ScoreWins), int(ScoreMoney)),
	windForce_(options_, "WindForce", 
		"The force of the wind", 0, int(WindRandom), int(WindRandom), int(Wind5)),
	windType_(options_, "WindType", 
		"When the wind changes", 0, int(WindOnRound), int(WindOnRound), int(WindOnMove)),
	wallType_(options_, "WallType", 
		"The type of walls allowed", 0, int(WallRandom), int(WallRandom), int(WallWrapAround)),
	weapScale_(options_, "WeaponScale", 
		"The scale of the weapons used", 0, int(ScaleMedium), int(ScaleSmall), int(ScaleLarge)),
	masterListServer_(options_, "MasterListServer",
		"The master list server for scorched3d", 0, "www.employees.org"),
	masterListServerURI_(options_, "MasterListServerURI",
		"The URI on the master list server for scorched3d", 0, "/~gcamp/scorched"),
	statsLogger_(options_, "StatsLogger",
		"The type of player stats to be logged (none, mysql, file)", FlagDontSend, "none"),
	portNo_(options_, "PortNo", 
		"The port to start the server on", 0, ScorchedPort),
	serverName_(options_, "ServerName", 
		"The name of the server to start", 0, "No Name"),
	landscapes_(options_, "Landscapes", 
		"Colon seperated list of landscape names", 0, ""),
	serverPassword_(options_, "ServerPassword", 
		"The password for this server (empty password = no password)", 0, ""),
	publishServer_(options_, "PublishServer",
		"Allow other scorched net clients to see this server.  Do not use for LAN games.", 0, false),
	publishAddress_(options_, "PublishAddress",
		"IP address to publish to scorched net clients (auto-detected if not given).", 0, "AutoDetect"),
	botNamePrefix_(options_, "BotNamePrefix", 
		"Prepend and bot name with the specified text", 0, "(Bot) "),
	computersDeathTalk_(options_, "ComputersDeathTalk",
		"The percentage chance the computers will say something when killed", 0, 100, 0, 100),
	computersAttackTalk_(options_, "ComputersAttackTalk",
		"The percentage chance the computers will say something when shooting", 0, 10, 0, 100)
{
	char buffer[128];
	for (int i=0; i<24; i++)
	{
		sprintf(buffer, "PlayerType%i", i+1);
		playerType_[i] = new OptionEntryString(playerTypeOptions_,
			buffer,
			"The type of the player e.g. human, computer etc..", 0,
			"Human");
	}
}

OptionsGame::~OptionsGame()
{
	
}

std::list<OptionEntry *> &OptionsGame::getOptions()
{
	return options_;
}

bool OptionsGame::writeToBuffer(NetBuffer &buffer)
{
        std::list<OptionEntry *> saveOptions;
        std::list<OptionEntry *>::iterator itor;
        for (itor = options_.begin();
                itor != options_.end();
                itor++)
        {
                OptionEntry *entry = *itor;
                if (!(entry->getData() & FlagDontSend))
                {
                        saveOptions.push_back(entry);
                }
        }

	if (!OptionEntryHelper::writeToBuffer(saveOptions, buffer)) return false;
	return true;
}

bool OptionsGame::readFromBuffer(NetBufferReader &reader)
{
        std::list<OptionEntry *> saveOptions;
        std::list<OptionEntry *>::iterator itor;
        for (itor = options_.begin();
                itor != options_.end();
                itor++)
        {
                OptionEntry *entry = *itor;
                if (!(entry->getData() & FlagDontSend))
                {
                        saveOptions.push_back(entry);
                }
        }

	if (!OptionEntryHelper::readFromBuffer(saveOptions, reader)) return false;
	return true;
}

bool OptionsGame::writeOptionsToFile(char *filePath)
{
	std::list<OptionEntry *> saveOptions = 
		playerTypeOptions_; // Note: The players are also saved
	std::list<OptionEntry *>::iterator itor;
	for (itor = options_.begin();
		itor != options_.end();
		itor++)
	{
		OptionEntry *entry = *itor;
		saveOptions.push_back(entry);
	} 

	if (!OptionEntryHelper::writeToFile(saveOptions, filePath)) return false;
	return true;
}

bool OptionsGame::readOptionsFromFile(char *filePath)
{
	std::list<OptionEntry *> saveOptions = 
		playerTypeOptions_; // Note: The players are also saved
        std::list<OptionEntry *>::iterator itor;
        for (itor = options_.begin();
                itor != options_.end();
                itor++)
        {
                OptionEntry *entry = *itor;
                saveOptions.push_back(entry);
        } 

	if (!OptionEntryHelper::readFromFile(saveOptions, filePath)) return false;
	return true;
}


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

OptionsGame *OptionsGame::instance_ = 0;

OptionsGame *OptionsGame::instance()
{
	if (!instance_)
	{
		instance_ = new OptionsGame;
	}

	return instance_;
}

OptionsGame::OptionsGame() :
	maxArmsLevel_(options_, "MaxArmsLevel",
		"The largest weapon type allowed", 0, 10, 0, 10),
	maxNumberWeapons_(nextOptions_, "MaxNumberWeapons",
		"The number of each weapon the player is allowed", 0, 90),
	tankStartCloseness_(options_, "TankStartCloseness",
		"How close tanks are allowed to start the level", 0, 20),
	maxLandscapeSize_(options_, "MaxLandscapeSize",
		"The largest landscape (in bytes) that will be sent to the clients", 0, 200000),
	shotTime_(options_, "ShotTime", 
		"The amount of team each player has for each shot", 0, 0),
	waitTime_(options_, "WaitTime", 
		"The amount of time to wait for new players to join", 0, 15),
	allowedMissedMoves_(options_, "AllowedMissedMoves",
		"The number of moves a player is allowed to miss (due to the shot timer)", 0, 3),
	scoreTime_(options_, "ScoreTime", 
		"The amount of time to show the end of game scores for", 0, 15),
	idleKickTime_(options_, "IdleKickTime",
		"The amount of time to give clients to respond before kicking them", 0, 30),
	maxRoundTurns_(options_, "MaxNumberOfRoundTurns",
		"The maximum number of turns all players are allowed in each round", 0, 15),
	numberOfRounds_(options_, "NumberOfRounds", 
		"The number of rounds to play in each game", 0, 5),
	numberOfPlayers_(options_, "NumberOfPlayers", 
		"The maximum number of players to allow", 0, 2, 1, 24),
	numberOfMinPlayers_(options_, "NumberOfMinPlayers", 
		"The minimum number of players to allow", 0, 2, 2, 24),
	simultaneous_(options_, "SimultaneousPlay", 
		"All players shoot at the same time", 0, false),
	moneyBuyOnRound_(options_, "MoneyBuyOnRound", 
		"The first round players are allowed to buy on", 0 ,2),
	moneyWonForRound_(options_, "MoneyWonForRound", 
		"The money awarded for each won round", 0, 5000),
	moneyPerHitPoint_(options_, "MoneyWonPerHitPoint", 
		"The money awarded for each person killed * weapon used", 0, 500),
	moneyStarting_(options_, "MoneyStarting", 
		"The money each player starts with", 0, 10000, 0, 500000),
	moneyInterest_(options_, "MoneyInterest", 
		"The interest awarded at the end of each round", 0, 15, 0, 100),
	landHills_(options_, "LandHills", 
		"The maximum number of hill", 0, 100, 10, 500),
	landHeight_(options_, "LandHeight", 
		"The maximum land height", 0, 30, 0, 100),
	windForce_(options_, "WindForce", 
		"The force of the wind", 0, int(WindRandom), int(WindRandom), int(Wind5)),
	windType_(options_, "WindType", 
		"When the wind changes", 0, int(WindOnRound), int(WindOnRound), int(WindOnMove)),
	wallType_(options_, "WallType", 
		"The type of walls allowed", 0, int(WallRandom), int(WallRandom), int(WallBouncy)),
	weapScale_(options_, "WeaponScale", 
		"The scale of the weapons used", 0, int(ScaleMedium), int(ScaleSmall), int(ScaleLarge)),
	portNo_(options_, "PortNo", 
		"The port to start the server on", 0, ScorchedPort),
	serverName_(options_, "ServerName", 
		"The name of the server to start", 0, "No Name"),
	publishServer_(options_, "PublishServer",
		"Allow other scorched net clients to see this server.  Do not use for LAN games.", 0, false),
	publishAddress_(options_, "PublishAddress",
		"IP address to publish to scorched net clients (auto-detected if not given).", 0, "AutoDetect"),
	botNamePrefix_(options_, "BotNamePrefix", 
		"Prepend and bot name with the specified text", 0, "(Bot) "),
	computersDeathTalk_(nextOptions_, "ComputersDeathTalk",
		"The percentage chance the computers will say something when killed", 0, 100, 0, 100),
	computersAttackTalk_(nextOptions_, "ComputersAttackTalk",
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
	if (!OptionEntryHelper::writeToBuffer(options_, buffer)) return false;
	return true;
}

bool OptionsGame::readFromBuffer(NetBufferReader &reader)
{
	if (!OptionEntryHelper::readFromBuffer(options_, reader)) return false;
	return true;
}

bool OptionsGame::writeOptionsToFile(char *filePath)
{
	std::list<OptionEntry *> saveOptions = 
		playerTypeOptions_;
	saveOptions.insert(
		saveOptions.begin(),
		options_.begin(),
		options_.end());

	if (!OptionEntryHelper::writeToFile(saveOptions, filePath)) return false;
	return true;
}

bool OptionsGame::readOptionsFromFile(char *filePath)
{
	std::list<OptionEntry *> saveOptions = 
		playerTypeOptions_;
	saveOptions.insert(
		saveOptions.begin(),
		options_.begin(),
		options_.end());

	if (!OptionEntryHelper::readFromFile(saveOptions, filePath)) return false;
	return true;
}

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
#include <common/Logger.h>
#include <string.h>

OptionsGame::OptionsGame() :
	teams_(options_, "Teams",
		"The number of teams (1 == no teams)", 0, 1, 1, 2),
	depricatedMaxArmsLevel_(options_, "MaxArmsLevel",
		"", OptionEntry::DataDepricated, 10, 0, 10),
	startArmsLevel_(options_, "StartArmsLevel",
		"The largest weapon type allowed (on round 1)", 0, 10, 0, 10),
	endArmsLevel_(options_, "EndArmsLevel",
		"The largest weapon type allowed (on the last round)", 0, 10, 0, 10),
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
		"The amount of time to give clients to respond after level loading before kicking them", 0, 60),
	idleShotKickTime_(options_, "IdleShotKickTime",
		"The amount of time to give clients to respond after shots before kicking them", 0, 10),
	minFallingDistance_(options_, "MinFallingDistance",
		"The minimum distance that can be made with no damage (value is divided by 10)", 0, 5),
	maxClimbingDistance_(options_, "MaxClimbingDistance",
		"The maximum distance that a tank can climb per movement square (divided by 10)", 0, 8),
	gravity_(options_, "Gravity",
		"The gravity used by the physics engine", 0, -10),
	maxRoundTurns_(options_, "MaxNumberOfRoundTurns",
		"The maximum number of turns all players are allowed in each round (0 = infinite)", 0, 15),
	numberOfRounds_(options_, "NumberOfRounds", 
		"The number of rounds to play in each game", 0, 5),
	numberOfPlayers_(options_, "NumberOfPlayers", 
		"The maximum number of players to allow", 0, 2, 0, 24),
	numberOfMinPlayers_(options_, "NumberOfMinPlayers", 
		"The minimum number of players to allow", 0, 2, 2, 24),
	removeBotsAtPlayers_(options_, "RemoveBotsAtPlayers",
		"The number of players to allow before remvoing bots", 0, 0, 0, 24),
	limitPowerByHealth_(options_, "LimitPowerByHealth", 
		"Show power is limited by the amount of health a player has", 0, true),
	residualPlayers_(options_, "ResidualPlayers",
		"Players that leave will have the same state when reconnecting", 0, true),
	resignMode_(options_, "ResignMode",
		"When does a players resign take place", 0, int(ResignEnd), int(ResignStart), int(ResignEnd)),
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
		"The money awarded for each person hit * weapon used", 0, 200),
	moneyPerKillPoint_(options_, "MoneyWonPerKillPoint", 
		"The money awarded for each person killed * weapon used", 0, 750),
	moneyPerRound_(options_, "MoneyPerRound", 
		"The money given at the end of each round", 0, 0),
	moneyStarting_(options_, "MoneyStarting", 
		"The money each player starts with", 0, 10000, 0, 500000),
	moneyInterest_(options_, "MoneyInterest", 
		"The interest awarded at the end of each round", 0, 15, 0, 100),
	freeMarketAdjustment_(options_, "FreeMarketAdjustment",
		"The scale of the adjustment changes made by the free market", 0, 100),
	scoreType_(options_, "ScoreType",
		"How the winnder is choosen", 0, int(ScoreWins), int(ScoreWins), int(ScoreMoney)),
	windForce_(options_, "WindForce", 
		"The force of the wind", 0, int(WindRandom), int(WindRandom), int(WindGale)),
	windType_(options_, "WindType", 
		"When the wind changes", 0, int(WindOnRound), int(WindOnRound), int(WindOnMove)),
	wallType_(options_, "WallType", 
		"The type of walls allowed", 0, int(WallRandom), int(WallRandom), int(WallWrapAround)),
	weapScale_(options_, "WeaponScale", 
		"The scale of the weapons used", 0, int(ScaleMedium), int(ScaleSmall), int(ScaleLarge)),
	masterListServer_(options_, "MasterListServer",
		"The master list server for scorched3d", 0, "scorched3d.sourceforge.net"),
	masterListServerURI_(options_, "MasterListServerURI",
		"The URI on the master list server for scorched3d", 0, "/scorched"),
	statsLogger_(options_, "StatsLogger",
		"The type of player stats to be logged (none, mysql, file)", 0, "none"),
	serverFileLogger_(options_, "ServerLogger",
		"The type of server events to be logged to file (none, file)", 0, "none"),
	portNo_(options_, "PortNo", 
		"The port to start the server on", 0, ScorchedPort),
	serverName_(options_, "ServerName", 
		"The name of the server to start", 0, "No Name"),
	mod_(options_, "Mod", 
		"The name of currently running Scorched3D mod", 0, ""),
	motd_(options_, "MOTD",
		"The message of the day", 0, 
		"Scorched3D : Copyright 2004 Gavin Camp\n"
		"For updates and news visit:\n"
		"http://www.scorched3d.co.uk"),
	modDownloadSpeed_(options_, "ModDownloadSpeed",
		"Max download speed of mods in bytes per second (0 == no mod download)", 0, 0),
	economy_(options_, "Economy", 
		"Speicifies the name of the economy to use", 0, "EconomyFreeMarket"),
	landscapes_(options_, "Landscapes", 
		"Colon seperated list of landscape names", 0, ""),
	serverPassword_(options_, "ServerPassword", 
		"The password for this server (empty password = no password)", OptionEntry::DataProtected, ""),
	serverAdminPassword_(options_, "ServerAdminPassword", 
		"The admin password for this server (empty password = no access)", OptionEntry::DataProtected, ""),
	allowSameIP_(options_, "AllowSameIP",
		"Allow scorched clients from same machine/NAT router to connect.", 0, true),
	publishServer_(options_, "PublishServer",
		"Allow other scorched net clients to see this server.  Do not use for LAN games.", 0, false),
	publishAddress_(options_, "PublishAddress",
		"IP address to publish to scorched net clients (auto-detected if not given).", 0, "AutoDetect"),
	botNamePrefix_(options_, "BotNamePrefix", 
		"Prepend and bot name with the specified text", 0, "(Bot) "),
	giveAllWeapons_(options_, "GiveAllWeapons",
		"Start the game will all the weapons", 0, false),
	cycleMaps_(options_, "CycleMaps",
		"Cycle through the maps instead of using the probablity", 0, false),
	randomizeBotNames_(options_, "RandomizeBotNames",
		"Choose random bot names instread of sequential names", 0, false),
	computersDeathTalk_(options_, "ComputersDeathTalk",
		"The percentage chance the computers will say something when killed", 0, 100, 0, 100),
	computersAttackTalk_(options_, "ComputersAttackTalk",
		"The percentage chance the computers will say something when shooting", 0, 25, 0, 100)
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

bool OptionsGame::writeToBuffer(NetBuffer &buffer,
	bool useProtected)
{
	std::list<OptionEntry *> saveOptions;
	std::list<OptionEntry *>::iterator itor;
	for (itor = options_.begin();
		itor != options_.end();
		itor++)
	{
		OptionEntry *entry = *itor;
		saveOptions.push_back(entry);
	}

	if (!OptionEntryHelper::writeToBuffer(
		saveOptions, buffer, useProtected)) return false;
	return true;
}

bool OptionsGame::readFromBuffer(NetBufferReader &reader,
	bool useProtected)
{
	std::list<OptionEntry *> saveOptions;
	std::list<OptionEntry *>::iterator itor;
	for (itor = options_.begin();
		itor != options_.end();
		itor++)
	{
		OptionEntry *entry = *itor;
		saveOptions.push_back(entry);
	}

	if (!OptionEntryHelper::readFromBuffer(
		saveOptions, reader, useProtected)) return false;
	return true;
}

bool OptionsGame::writeOptionsToXML(XMLNode *node)
{
	if (!OptionEntryHelper::writeToXML(options_, node)) return false;
	return true;
}

bool OptionsGame::readOptionsFromXML(XMLNode *node)
{
	if (!OptionEntryHelper::readFromXML(options_, node)) return false;
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

std::list<OptionEntry *> &OptionsGame::getOptions()
{
	return options_;
}

OptionsGameWrapper::OptionsGameWrapper()
{
}

OptionsGameWrapper::~OptionsGameWrapper()
{
}

void OptionsGameWrapper::updateChangeSet()
{
	NetBufferDefault::defaultBuffer.reset();
	writeToBuffer(NetBufferDefault::defaultBuffer, true);
	NetBufferReader reader(NetBufferDefault::defaultBuffer);
	changedOptions_.readFromBuffer(reader, true);
}

bool OptionsGameWrapper::commitChanges()
{
	// Setup buffers
	static NetBuffer testBuffer;
	testBuffer.reset();
	NetBufferDefault::defaultBuffer.reset();

	// Write to buffers
	writeToBuffer(NetBufferDefault::defaultBuffer, true);
	changedOptions_.writeToBuffer(testBuffer, true);

	// Compare buffers
	if (memcmp(testBuffer.getBuffer(), 
		NetBufferDefault::defaultBuffer.getBuffer(),
		MIN(testBuffer.getBufferUsed(),
		NetBufferDefault::defaultBuffer.getBufferUsed())) != 0)
	{
		std::list<OptionEntry *> &options = getOptions();
		std::list<OptionEntry *> &otheroptions = changedOptions_.getOptions();
		std::list<OptionEntry *>::iterator itor;
		std::list<OptionEntry *>::iterator otheritor;
		for (itor=options.begin(), otheritor=otheroptions.begin();
			itor!=options.end() && otheritor!=options.end();
			itor++, otheritor++)
		{
			OptionEntry *entry = *itor;
			OptionEntry *otherentry = *otheritor;
			std::string str = entry->getValueAsString();
			std::string otherstr = otherentry->getValueAsString();
			if (str != otherstr)
			{
				if (strlen(str.c_str()) < 20 && strlen(otherstr.c_str()) < 20)
				{
					Logger::log(0, "Option %s has been changed from %s to %s",
						entry->getName(), str.c_str(), otherstr.c_str());
				}
				else
				{
					Logger::log(0, "Option %s has been changed.",
						entry->getName());
				}
			}
		}

		NetBufferReader reader(testBuffer);
		readFromBuffer(reader, true);
		return true;
	}

	return false;
}

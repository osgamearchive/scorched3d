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

#if !defined(__INCLUDE_OptionsGameh_INCLUDE__)
#define __INCLUDE_OptionsGameh_INCLUDE__

#include <common/OptionEntry.h>
#include <common/Defines.h>

class OptionsGame
{
public:
	OptionsGame();
	virtual ~OptionsGame();

	enum ScoreType
	{
		ScoreWins = 0,
		ScoreKills = 1,
		ScoreMoney = 2
	};
	enum TurnType
	{
		TurnSimultaneous = 0,
		TurnSequentialLooserFirst = 1,
		TurnSequentialRandom = 2
	};
	enum WindForce
	{
		WindRandom = 0,
		WindNone = 1,
		Wind1 = 2,
		Wind2 = 3,
		Wind3 = 4,
		Wind4 = 5,
		Wind5 = 6,
		WindBreezy = 7,
		WindGale = 8
	};
	enum WindType
	{
		WindOnRound = 0,
		WindOnMove = 1
	};
	enum WallType
	{
		WallRandom = 0,
		WallConcrete = 1,
		WallBouncy = 2,
		WallWrapAround = 3
	};
	enum WeapScale
	{
		ScaleSmall = 0,
		ScaleMedium = 1,
		ScaleLarge = 2
	};

	int getTeams() { return teams_.getValue(); }
	void setTeams(int value) { teams_.setValue(value); }
	const char *getTeamsToolTip() { return teams_.getDescription(); }

	int getStartArmsLevel() { return startArmsLevel_.getValue(); }
	void setStartArmsLevel(int value) { startArmsLevel_.setValue(value); }
	const char *getStartArmsLevelToolTip() { return startArmsLevel_.getDescription(); }

	int getEndArmsLevel() { return endArmsLevel_.getValue(); }
	void setEndArmsLevel(int value) { endArmsLevel_.setValue(value); }
	const char *getEndArmsLevelToolTip() { return endArmsLevel_.getDescription(); }

	int getMaxNumberWeapons() { return maxNumberWeapons_.getValue(); }
	void setMaxNumberWeapons(int value) { maxNumberWeapons_.setValue(value); }
	const char *getMaxNumberWeaponsToolTip() { return maxNumberWeapons_.getDescription(); }

	int getNoMaxPlayers() { return numberOfPlayers_.getValue(); }
	void setNoMaxPlayers(int value) { numberOfPlayers_.setValue(value); }
	const char *getNoMaxPlayersToolTip() { return numberOfPlayers_.getDescription(); }

	int getNoMinPlayers() { return numberOfMinPlayers_.getValue(); }
	void setNoMinPlayers(int value) { numberOfMinPlayers_.setValue(value); }
	const char *getNoMinPlayersToolTip() { return numberOfMinPlayers_.getDescription(); }

	int getNoRounds() { return numberOfRounds_.getValue(); }
	void setNoRounds(int value) { numberOfRounds_.setValue(value); }
	const char *getNoRoundsToolTip() { return numberOfRounds_.getDescription(); }

	int getRemoveBotsAtPlayers() { return removeBotsAtPlayers_.getValue(); }
	void setRemoveBotsAtPlayers(int value) { removeBotsAtPlayers_.setValue(value); }
	const char *getRemoveBotsAtPlayersToolTip() { return removeBotsAtPlayers_.getDescription(); }

	int getNoMaxRoundTurns() { return maxRoundTurns_.getValue(); }
	void setNoMaxRoundTurns(int value) { maxRoundTurns_.setValue(value); }
	const char *getNoMaxRoundTurnsToolTip() { return maxRoundTurns_.getDescription(); }

	int getStartMoney() { return moneyStarting_.getValue(); }
	void setStartMoney(int value) { moneyStarting_.setValue(value); }
	const char *getStartMoneyToolTip() { return moneyStarting_.getDescription(); }

	int getInterest() { return moneyInterest_.getValue(); }
	void setInterest(int value) { moneyInterest_.setValue(value); }
	const char *getInterestToolTip() { return moneyInterest_.getDescription(); }

	int getFreeMarketAdjustment() { return freeMarketAdjustment_.getValue(); }
	void setFreeMarketAdjustment(int value) { freeMarketAdjustment_.setValue(value); }
	const char *getFreeMarketAdjustmentToolTip() { return freeMarketAdjustment_.getDescription(); }

	int getMaxLandscapeSize() { return maxLandscapeSize_.getValue(); }
	void setMaxLandscapeSize(int value) { maxLandscapeSize_.setValue(value); }
	const char *getMaxLandscapeSizeToolTip() { return maxLandscapeSize_.getDescription(); }

	int getShotTime() { return shotTime_.getValue(); }
	void setShotTime(int value) { shotTime_.setValue(value); }
	const char *getShotTimeToolTip() { return shotTime_.getDescription(); }

	int getBuyingTime() { return buyingTime_.getValue(); }
	void setBuyingTime(int value) { buyingTime_.setValue(value); }
	const char *getBuyingTimeToolTip() { return buyingTime_.getDescription(); }

	int getScoreTime() { return scoreTime_.getValue(); }
	void setScoreTime(int value) { scoreTime_.setValue(value); }
	const char *getScoreTimeToolTip() { return scoreTime_.getDescription(); }

	int getAllowedMissedMoves() { return allowedMissedMoves_.getValue(); }
	void setAllowedMissedMoves(int value) { allowedMissedMoves_.setValue(value); }
	const char *getAllowedMissedMovesToolTip() { return allowedMissedMoves_.getDescription(); }

	int getIdleKickTime() { return idleKickTime_.getValue(); }
	void setIdleKickTime(int value) { idleKickTime_.setValue(value); }
	const char *getIdleKickTimeToolTip() { return idleKickTime_.getDescription(); }

	int getIdleShotKickTime() { return idleShotKickTime_.getValue(); }
	void setIdleShotKickTime(int value) { idleShotKickTime_.setValue(value); }
	const char *getIdleShotKickTimeToolTip() { return idleShotKickTime_.getDescription(); }

	int getMinFallingDistance() { return minFallingDistance_.getValue(); }
	void setMinFallingDistance(int value) { minFallingDistance_.setValue(value); }
	const char *getMinFallingDistanceToolTip() { return minFallingDistance_.getDescription(); }

	int getMaxClimbingDistance() { return maxClimbingDistance_.getValue(); }
	void setMaxClimbingDistance(int value) { maxClimbingDistance_.setValue(value); }
	const char *getMaxClimbingDistanceToolTip() { return maxClimbingDistance_.getDescription(); }

	int getGravity() { return gravity_.getValue(); }
	void setGravity(int value) { gravity_.setValue(value); }
	const char *getGravityToolTip() { return gravity_.getDescription(); }

	ScoreType getScoreType() { return (ScoreType) scoreType_.getValue(); }
	void setScoreType(ScoreType value) { scoreType_.setValue((int) value); }
	const char *getScoreTypeToolTip() { return scoreType_.getDescription(); }

	WindForce getWindForce() { return (WindForce) windForce_.getValue(); }
	void setWindForce(WindForce value) { windForce_.setValue((int) value); }
	const char *getWindForceToolTip() { return windForce_.getDescription(); }

	WindType getWindType() { return (WindType) windType_.getValue(); }
	void setWindType(WindType value) { windType_.setValue((int) value); }
	const char *getWindTypeToolTip() { return windType_.getDescription(); }

	WallType getWallType() { return (WallType) wallType_.getValue(); }
	void setWallType(WallType value) { wallType_.setValue((int) value); }
	const char *getWallTypeToolTip() { return wallType_.getDescription(); }

	WeapScale getWeapScale() { return (WeapScale) weapScale_.getValue(); }
	void setWeapScale(WeapScale value) { weapScale_.setValue((int) value); }	
	const char *getWeapScaleToolTip() { return weapScale_.getDescription(); }

	TurnType getTurnType() { return (TurnType) turnType_.getValue(); }
	void setTurnType(TurnType value) { turnType_.setValue((int) value); }	
	const char *getTurnTypeToolTip() { return turnType_.getDescription(); }
	 
	int getBuyOnRound() { return moneyBuyOnRound_.getValue(); }
	void setBuyOnRound(int value) { moneyBuyOnRound_.setValue(value); }
	const char *getBuyOnRoundToolTip() { return moneyBuyOnRound_.getDescription(); }

	int getMoneyWonForRound() { return moneyWonForRound_.getValue(); }
	void setMoneyWonForRound(int value) { moneyWonForRound_.setValue(value); }
	const char *getMoneyWonForRoundToolTip() { return moneyWonForRound_.getDescription(); }

	int getMoneyWonPerHitPoint() { return moneyPerHitPoint_.getValue(); }
	void setMoneyWonPerHitPoint(int value) { moneyPerHitPoint_.setValue(value); }
	const char *getMoneyWonPerHitPointToolTip() { return moneyPerHitPoint_.getDescription(); }

	int getMoneyWonPerKillPoint() { return moneyPerKillPoint_.getValue(); }
	void setMoneyWonPerKillPoint(int value) { moneyPerKillPoint_.setValue(value); }
	const char *getMoneyWonPerKillPointToolTip() { return moneyPerKillPoint_.getDescription(); }

	int getMoneyPerRound() { return moneyPerRound_.getValue(); }
	void setMoneyPerRound(int value) { moneyPerRound_.setValue(value); }
	const char *getMoneyPerRoundToolTip() { return moneyPerRound_.getDescription(); }

	bool getMoneyPerHealthPoint() { return moneyPerHealthPoint_.getValue(); }
	void setMoneyPerHealthPoint(bool value) { moneyPerHealthPoint_.setValue(value); }
	const char *getMoneyPerHealthPointToolTip() { return moneyPerHealthPoint_.getDescription(); }

	bool getLimitPowerByHealth() { return limitPowerByHealth_.getValue(); }
	void setLimitPowerByHealth(bool value) { limitPowerByHealth_.setValue(value); }
	const char *getLimitPowerByHealthToolTip() { return limitPowerByHealth_.getDescription(); }

	bool getAutoBallanceTeams() { return autoBallanceTeams_.getValue(); }
	void setAutoBallanceTeams(bool value) { autoBallanceTeams_.setValue(value); }
	const char *getAutoBallanceTeamsToolTip() { return autoBallanceTeams_.getDescription(); }

	int getComputersDeathTalk() { return computersDeathTalk_.getValue(); }
	void setComputersDeathTalk(int value) { computersDeathTalk_.setValue(value); }
	const char *getComputersDeathTalkToolTip() { return computersDeathTalk_.getDescription(); }

	int getComputersAttackTalk() { return computersAttackTalk_.getValue(); }
	void setComputersAttackTalk(int value) { computersAttackTalk_.setValue(value); }
	const char *getComputersAttackTalkToolTip() { return computersAttackTalk_.getDescription(); }

	const char *getEconomy() { return economy_.getValue(); }
	void setEconomy(const char *value) { economy_.setValue(value); }
	const char *getEconomyToolTip() { return economy_.getDescription(); }

	const char *getLandscapes() { return landscapes_.getValue(); }
	void setLandscapes(const char *value) { landscapes_.setValue(value); }
	const char *getLandscapesToolTip() { return landscapes_.getDescription(); }

	const char *getMasterListServer() { return masterListServer_.getValue(); }
	void setMasterListServer(const char *value) { masterListServer_.setValue(value); }
	const char *getMasterListServerToolTip() { return masterListServer_.getDescription(); }

	const char *getMasterListServerURI() { return masterListServerURI_.getValue(); }
	void setMasterListServerURI(const char *value) { masterListServerURI_.setValue(value); }
	const char *getMasterListServerURIToolTip() { return masterListServerURI_.getDescription(); }

	const char *getStatsLogger() { return statsLogger_.getValue(); }
	void setStatsLogger(const char *value) { statsLogger_.setValue(value); }
	const char *getStatsLoggerToolTip() { return statsLogger_.getDescription(); }

	const char *getServerFileLogger() { return serverFileLogger_.getValue(); }
	void setServerFileLogger(const char *value) { serverFileLogger_.setValue(value); }
	const char *getServerFileLoggerToolTip() { return serverFileLogger_.getDescription(); }

	const char *getBotNamePrefix() { return botNamePrefix_.getValue(); }
	void setBotNamePrefix(const char *value) { botNamePrefix_.setValue(value); }
	const char *getBotNamePrefixToolTip() { return botNamePrefix_.getDescription(); }

	bool getGiveAllWeapons() { return giveAllWeapons_.getValue(); }
	void setGiveAllWeapons(bool value) { giveAllWeapons_.setValue(value); }
	const char *getGiveAllWeaponsToolTip() { return giveAllWeapons_.getDescription(); }

	bool getCycleMaps() { return cycleMaps_.getValue(); }
	void setCycleMaps(bool value) { cycleMaps_.setValue(value); }
	const char *getCycleMapsToolTip() { return cycleMaps_.getDescription(); }

	bool getResignEndOfRound() { return resignEndOfRound_.getValue(); }
	void setResignEndOfRound(bool value) { resignEndOfRound_.setValue(value); }
	const char *getResignEndOfRoundToolTip() { return resignEndOfRound_.getDescription(); }

	bool getRandomizeBotNames() { return randomizeBotNames_.getValue(); }
	void setRandomizeBotNames(bool value) { randomizeBotNames_.setValue(value); }
	const char *getRandomizeBotNamesToolTip() { return randomizeBotNames_.getDescription(); }

	int getPortNo() { return portNo_.getValue(); }
	void setPortNo(int value) { portNo_.setValue(value); } 
	const char *getPortNoToolTip() { return portNo_.getDescription(); }

	const char *getMod() { return mod_.getValue(); }
	void setMod(const char *value) { mod_.setValue(value); }
	const char *getModToolTip() { return mod_.getDescription(); }

	const char *getMOTD() { return motd_.getValue(); }
	void setMOTD(const char *value) { motd_.setValue(value); }
	const char *getMOTDToolTip() { return motd_.getDescription(); }

	int getModDownloadSpeed() { return modDownloadSpeed_.getValue(); }
	void setModDownloadSpeed(int value) { modDownloadSpeed_.setValue(value); }
	const char *getModDownloadSpeedToolTip() { return modDownloadSpeed_.getDescription(); }

	const char *getServerName() { return serverName_.getValue(); }
	void setServerName(const char *value) { serverName_.setValue(value); }
	const char *getServerNameToolTip() { return serverName_.getDescription(); }

	const char *getServerAdminPassword() { return serverAdminPassword_.getValue(); }
	void setServerAdminPassword(const char *value) { serverAdminPassword_.setValue(value); }
	const char *getServerAdminPasswordToolTip() { return serverAdminPassword_.getDescription(); }

	const char *getServerPassword() { return serverPassword_.getValue(); }
	void setServerPassword(const char *value) { serverPassword_.setValue(value); }
	const char *getServerPasswordToolTip() { return serverPassword_.getDescription(); }

	const char *getPlayerType(int no) { DIALOG_ASSERT(no<24); return playerType_[no]->getValue(); }
	void setPlayerType(int no, const char *value) { DIALOG_ASSERT(no<24); playerType_[no]->setValue(value); }

	const char *getPublishAddress() { return publishAddress_.getValue(); }
	void setPublishAddress(const char *value) { publishAddress_.setValue(value); }
	const char *getPublishAddressToolTip() { return publishAddress_.getDescription(); }

	bool getPublishServer() { return publishServer_.getValue(); }
	void setPublishServer(bool value) { publishServer_.setValue(value); }	
	const char *getPublishServerToolTip() { return publishServer_.getDescription(); }

	bool getResidualPlayers() { return residualPlayers_.getValue(); }
	void setResidualPlayers(bool value) { residualPlayers_.setValue(value); }	
	const char *getResidualPlayersToolTip() { return residualPlayers_.getDescription(); }

	// Fns used to save or restore the state of the options
	std::list<OptionEntry *> &getOptions();
	virtual bool writeOptionsToXML(XMLNode *xmlNode);
	virtual bool readOptionsFromXML(XMLNode *xmlNode);
	virtual bool writeOptionsToFile(char *filePath);
	virtual bool readOptionsFromFile(char *filePath);
	virtual bool writeToBuffer(NetBuffer &buffer, bool useProtected);
	virtual bool readFromBuffer(NetBufferReader &reader, bool useProtected);

protected:
	std::list<OptionEntry *> options_;
	std::list<OptionEntry *> playerTypeOptions_;

	OptionEntryBoundedInt depricatedMaxArmsLevel_;
	OptionEntryBoundedInt startArmsLevel_;
	OptionEntryBoundedInt endArmsLevel_;
	OptionEntryInt maxRoundTurns_;
	OptionEntryInt shotTime_;
	OptionEntryInt buyingTime_;
	OptionEntryInt scoreTime_;
	OptionEntryInt allowedMissedMoves_;
	OptionEntryInt numberOfRounds_;
	OptionEntryInt maxNumberWeapons_;
	OptionEntryInt gravity_;
	OptionEntryInt minFallingDistance_;
	OptionEntryInt maxClimbingDistance_;
	OptionEntryBoundedInt scoreType_;
	OptionEntryBoundedInt teams_;
	OptionEntryBoundedInt numberOfPlayers_;
	OptionEntryBoundedInt numberOfMinPlayers_;
	OptionEntryBoundedInt removeBotsAtPlayers_;
	OptionEntryBoundedInt computersDeathTalk_;
	OptionEntryBoundedInt computersAttackTalk_;
	OptionEntryInt moneyBuyOnRound_;
	OptionEntryInt moneyWonForRound_;
	OptionEntryInt maxLandscapeSize_;
	OptionEntryInt moneyPerHitPoint_;
	OptionEntryInt moneyPerKillPoint_;
	OptionEntryInt moneyPerRound_;
	OptionEntryInt freeMarketAdjustment_;
	OptionEntryBool moneyPerHealthPoint_;
	OptionEntryBoundedInt moneyStarting_;
	OptionEntryInt idleKickTime_;
	OptionEntryInt idleShotKickTime_;
	OptionEntryBoundedInt moneyInterest_;
	OptionEntryBool limitPowerByHealth_;
	OptionEntryBool autoBallanceTeams_;
	OptionEntryBool cycleMaps_;
	OptionEntryBool resignEndOfRound_;
	OptionEntryBoundedInt turnType_;
	OptionEntryBoundedInt windForce_;
	OptionEntryBoundedInt windType_;
	OptionEntryBoundedInt wallType_;
	OptionEntryBoundedInt weapScale_;
	OptionEntryInt modDownloadSpeed_;
	OptionEntryString mod_;
	OptionEntryString motd_;
	OptionEntryString economy_;
	OptionEntryString landscapes_;
	OptionEntryString masterListServer_;
	OptionEntryString masterListServerURI_;
	OptionEntryString statsLogger_;
	OptionEntryString serverFileLogger_;

	// Server only options
	OptionEntryString botNamePrefix_;
	OptionEntryBool residualPlayers_;
	OptionEntryBool randomizeBotNames_;
	OptionEntryBool giveAllWeapons_;
	OptionEntryString serverName_;
	OptionEntryString *playerType_[24];
	OptionEntryString serverPassword_;
	OptionEntryString serverAdminPassword_;
	OptionEntryInt portNo_;
	OptionEntryString publishAddress_;
	OptionEntryBool publishServer_;
};

class OptionsGameWrapper : public OptionsGame
{
public:
	OptionsGameWrapper();
	virtual ~OptionsGameWrapper();

	// Options that can be changed on the fly
	// These options then replace the main options at set times
	OptionsGame &getChangedOptions() { return changedOptions_; }
	void updateChangeSet();
	bool commitChanges();

protected:
	OptionsGame changedOptions_;

};

#endif

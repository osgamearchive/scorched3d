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
		Wind5 = 6
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
		WallBouncy = 2
	};
	enum WeapScale
	{
		ScaleSmall = 0,
		ScaleMedium = 1,
		ScaleLarge = 2
	};

	int getMaxArmsLevel() { return maxArmsLevel_.getValue(); }
	void setMaxArmsLevel(int value) { maxArmsLevel_.setValue(value); }

	int getMaxNumberWeapons() { return maxNumberWeapons_.getValue(); }
	void setMaxNumberWeapons(int value) { maxNumberWeapons_.setValue(value); }

	int getTankStartCloseness() { return tankStartCloseness_.getValue(); }
	void setTankStartCloseness(int value) { tankStartCloseness_.setValue(value); }

	int getNoMaxPlayers() { return numberOfPlayers_.getValue(); }
	void setNoMaxPlayers(int value) { numberOfPlayers_.setValue(value); }

	int getNoMinPlayers() { return numberOfMinPlayers_.getValue(); }
	void setNoMinPlayers(int value) { numberOfMinPlayers_.setValue(value); }

	int getNoRounds() { return numberOfRounds_.getValue(); }
	void setNoRounds(int value) { numberOfRounds_.setValue(value); }

	int getNoMaxRoundTurns() { return maxRoundTurns_.getValue(); }
	void setNoMaxRoundTurns(int value) { maxRoundTurns_.setValue(value); }

	int getStartMoney() { return moneyStarting_.getValue(); }
	void setStartMoney(int value) { moneyStarting_.setValue(value); }

	int getInterest() { return moneyInterest_.getValue(); }
	void setInterest(int value) { moneyInterest_.setValue(value); }

	int getLandWidthX() { return landWidthX_.getValue(); }
	void setLandWidthX(int value) { landWidthX_.setValue(value); }

	int getLandWidthY() { return landWidthY_.getValue(); }
	void setLandWidthY(int value) { landWidthY_.setValue(value); }

	int getNoHills() { return landHills_.getValue(); }
	void setNoHills(int value) { landHills_.setValue(value); }

	int getMaxHeight() { return landHeight_.getValue(); }
	void setMaxHeight(int value) { landHeight_.setValue(value); }

	int getMaxLandscapeSize() { return maxLandscapeSize_.getValue(); }
	void setMaxLandscapeSize(int value) { maxLandscapeSize_.setValue(value); }

	int getShotTime() { return shotTime_.getValue(); }
	void setShotTime(int value) { shotTime_.setValue(value); }

	int getBuyingTime() { return buyingTime_.getValue(); }
	void setBuyingTime(int value) { buyingTime_.setValue(value); }

	int getWaitTime() { return waitTime_.getValue(); }
	void setWaitTime(int value) { waitTime_.setValue(value); }

	int getScoreTime() { return scoreTime_.getValue(); }
	void setScoreTime(int value) { scoreTime_.setValue(value); }

	int getAllowedMissedMoves() { return allowedMissedMoves_.getValue(); }
	void setAllowedMissedMoves(int value) { allowedMissedMoves_.setValue(value); }

	int getIdleKickTime() { return idleKickTime_.getValue(); }
	void setIdleKickTime(int value) { idleKickTime_.setValue(value); }

	WindForce getWindForce() { return (WindForce) windForce_.getValue(); }
	void setWindForce(WindForce value) { windForce_.setValue((int) value); }

	WindType getWindType() { return (WindType) windType_.getValue(); }
	void setWindType(WindType value) { windType_.setValue((int) value); }

	WallType getWallType() { return (WallType) wallType_.getValue(); }
	void setWallType(WallType value) { wallType_.setValue((int) value); }

	WeapScale getWeapScale() { return (WeapScale) weapScale_.getValue(); }
	void setWeapScale(WeapScale value) { weapScale_.setValue((int) value); }	

	TurnType getTurnType() { return (TurnType) turnType_.getValue(); }
	void setTurnType(TurnType value) { turnType_.setValue((int) value); }	
	 
	int getBuyOnRound() { return moneyBuyOnRound_.getValue(); }
	void setBuyOnRound(int value) { moneyBuyOnRound_.setValue(value); }

	int getMoneyWonForRound() { return moneyWonForRound_.getValue(); }
	void setMoneyWonForRound(int value) { moneyWonForRound_.setValue(value); }

	int getMoneyWonPerHitPoint() { return moneyPerHitPoint_.getValue(); }
	void setMoneyWonPerHitPoint(int value) { moneyPerHitPoint_.setValue(value); }

	bool getLimitPowerByHealth() { return limitPowerByHealth_.getValue(); }
	void setLimitPowerByHealth(bool value) { limitPowerByHealth_.setValue(value); }

	int getComputersDeathTalk() { return computersDeathTalk_.getValue(); }
	void setComputersDeathTalk(int value) { computersDeathTalk_.setValue(value); }

	int getComputersAttackTalk() { return computersAttackTalk_.getValue(); }
	void setComputersAttackTalk(int value) { computersAttackTalk_.setValue(value); }

	const char *getMasterListServer() { return masterListServer_.getValue(); }
	void setMasterListServer(const char *value) { masterListServer_.setValue(value); }

	const char *getMasterListServerURI() { return masterListServerURI_.getValue(); }
	void setMasterListServerURI(const char *value) { masterListServerURI_.setValue(value); }

	// Server only options
	const char *getBotNamePrefix() { return botNamePrefix_.getValue(); }
	void setBotNamePrefix(const char *value) { botNamePrefix_.setValue(value); }

	int getPortNo() { return portNo_.getValue(); }
	void setPortNo(int value) { portNo_.setValue(value); } 

	const char *getServerName() { return serverName_.getValue(); }
	void setServerName(const char *value) { serverName_.setValue(value); }

	const char *getServerPassword() { return serverPassword_.getValue(); }
	void setServerPassword(const char *value) { serverPassword_.setValue(value); }

	const char *getPlayerType(int no) { DIALOG_ASSERT(no<24); return playerType_[no]->getValue(); }
	void setPlayerType(int no, const char *value) { DIALOG_ASSERT(no<24); playerType_[no]->setValue(value); }

	const char *getPublishAddress() { return publishAddress_.getValue(); }
	void setPublishAddress(const char *value) { publishAddress_.setValue(value); }

	bool getPublishServer() { return publishServer_.getValue(); }
	void setPublishServer(bool value) { publishServer_.setValue(value); }	

	// Fns used to save or restore the state of the options
	bool writeOptionsToFile(char *filePath);
	bool readOptionsFromFile(char *filePath);
	bool writeToBuffer(NetBuffer &buffer);
	bool readFromBuffer(NetBufferReader &reader);

	std::list<OptionEntry *> &getOptions();

protected:
	std::list<OptionEntry *> options_;
	std::list<OptionEntry *> playerTypeOptions_;

	OptionEntryBoundedInt maxArmsLevel_;
	OptionEntryInt maxRoundTurns_;
	OptionEntryInt tankStartCloseness_;
	OptionEntryInt shotTime_;
	OptionEntryInt buyingTime_;
	OptionEntryInt waitTime_;
	OptionEntryInt scoreTime_;
	OptionEntryInt allowedMissedMoves_;
	OptionEntryInt numberOfRounds_;
	OptionEntryInt maxNumberWeapons_;
	OptionEntryBoundedInt numberOfPlayers_;
	OptionEntryBoundedInt numberOfMinPlayers_;
	OptionEntryBoundedInt computersDeathTalk_;
	OptionEntryBoundedInt computersAttackTalk_;
	OptionEntryInt moneyBuyOnRound_;
	OptionEntryInt moneyWonForRound_;
	OptionEntryInt maxLandscapeSize_;
	OptionEntryInt moneyPerHitPoint_;
	OptionEntryBoundedInt moneyStarting_;
	OptionEntryInt idleKickTime_;
	OptionEntryBoundedInt moneyInterest_;
	OptionEntryBool limitPowerByHealth_;
	OptionEntryBoundedInt turnType_;
	OptionEntryBoundedInt landWidthY_;
	OptionEntryBoundedInt landWidthX_;
	OptionEntryBoundedInt landHills_;
	OptionEntryBoundedInt landHeight_;
	OptionEntryBoundedInt windForce_;
	OptionEntryBoundedInt windType_;
	OptionEntryBoundedInt wallType_;
	OptionEntryBoundedInt weapScale_;
	OptionEntryString masterListServer_;
	OptionEntryString masterListServerURI_;

	// Server only options
	OptionEntryString botNamePrefix_;
	OptionEntryString serverName_;
	OptionEntryString *playerType_[24];
	OptionEntryString serverPassword_;
	OptionEntryInt portNo_;
	OptionEntryString publishAddress_;
	OptionEntryBool publishServer_;

};


#endif

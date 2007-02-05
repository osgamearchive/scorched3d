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

#if !defined(__INCLUDE_OptionsScorchedh_INCLUDE__)
#define __INCLUDE_OptionsScorchedh_INCLUDE__

#include <common/OptionsGame.h>

#define OPTIONSTRING_GETTER(x, y) const char *get##x() { return mainOptions_.get##x(); } \
	OptionEntryString &get##x##Entry() { return mainOptions_.get##x##Entry(); }

#define OPTIONINT_GETTER(x, y) int get##x() { return mainOptions_.get##x(); } \
	OptionEntryInt &get##x##Entry() { return mainOptions_.get##x##Entry(); }

#define OPTIONENUM_GETTER(x, y) OptionEntryEnum get##x() { return mainOptions_.get##x(); } \
	OptionEntryEnum &get##x##Entry() { return mainOptions_.get##x##Entry(); }

#define OPTIONSTRINGENUM_GETTER(x, y) const char *get##x() { return mainOptions_.get##x(); } \
	OptionEntryStringEnum &get##x##Entry() { return mainOptions_.get##x##Entry(); }

#define OPTIONBOOL_GETTER(x, y) bool get##x() { return mainOptions_.get##x(); } \
	OptionEntryBool &get##x##Entry() { return mainOptions_.get##x##Entry(); }

#define OPTIONBOOL_GETTER(x, y) bool get##x() { return mainOptions_.get##x(); } \
	OptionEntryBool &get##x##Entry() { return mainOptions_.get##x##Entry(); }

class OptionsScorched
{
public:
	OptionsScorched();
	virtual ~OptionsScorched();

	OPTIONINT_GETTER(PortNo, portNo);
	OPTIONINT_GETTER(ManagementPortNo, managementPortNo);

	OPTIONSTRING_GETTER(Tutorial, tutorial);
	OPTIONINT_GETTER(PhysicsFPS, physicsFPS);
	OPTIONINT_GETTER(Teams, teams);
	OPTIONINT_GETTER(StartArmsLevel, startArmsLevel);
	OPTIONINT_GETTER(EndArmsLevel, endArmsLevel);
	OPTIONINT_GETTER(MaxNumberWeapons, maxNumberWeapons);
	OPTIONINT_GETTER(NoMaxPlayers, numberOfPlayers);
	OPTIONINT_GETTER(NoMinPlayers, numberOfMinPlayers);
	OPTIONINT_GETTER(NoRounds, numberOfRounds);
	OPTIONINT_GETTER(RemoveBotsAtPlayers, removeBotsAtPlayers);
	OPTIONINT_GETTER(NoMaxRoundTurns, maxRoundTurns);
	OPTIONINT_GETTER(MaxLandscapeSize, maxLandscapeSize);
	OPTIONINT_GETTER(AllowedMissedMoves, allowedMissedMoves);
	OPTIONINT_GETTER(IdleKickTime, idleKickTime);
	OPTIONINT_GETTER(IdleShotKickTime, idleShotKickTime);
	OPTIONINT_GETTER(MinFallingDistance, minFallingDistance);
	OPTIONINT_GETTER(MaxClimbingDistance, maxClimbingDistance);
	OPTIONINT_GETTER(PlayerLives, playerLives);
	OPTIONINT_GETTER(Gravity, gravity);

	OPTIONINT_GETTER(StartTime, startTime);
	OPTIONINT_GETTER(ShotTime, shotTime);
	OPTIONINT_GETTER(KeepAliveTime, keepAliveTime);
	OPTIONINT_GETTER(KeepAliveTimeoutTime, keepAliveTimeoutTime);
	OPTIONINT_GETTER(BuyingTime, buyingTime);
	OPTIONINT_GETTER(RoundScoreTime, roundScoreTime);
	OPTIONINT_GETTER(ScoreTime, scoreTime);

	OPTIONENUM_GETTER(WindForce, windForce);
	OPTIONENUM_GETTER(WindType, windType);
	OPTIONENUM_GETTER(WallType, wallType);
	OPTIONENUM_GETTER(WeapScale, weapScale);
	OPTIONENUM_GETTER(TurnType, turnType);
	OPTIONENUM_GETTER(TeamBallance, teamBallance);
	OPTIONENUM_GETTER(MovementRestriction, movementRestriction);

	OPTIONINT_GETTER(ScorePerMoney, scorePerMoney);
	OPTIONINT_GETTER(ScorePerAssist, scorePerAssist);
	OPTIONINT_GETTER(ScorePerKill, scorePerKill);
	OPTIONINT_GETTER(ScoreWonForRound, scoreWonForRound);
	OPTIONINT_GETTER(ScoreWonForLives, scoreWonForLives);

	OPTIONINT_GETTER(StartMoney, moneyStarting);
	OPTIONINT_GETTER(Interest, moneyInterest);
	OPTIONINT_GETTER(FreeMarketAdjustment, freeMarketAdjustment);
	OPTIONINT_GETTER(BuyOnRound, moneyBuyOnRound);
	OPTIONINT_GETTER(MoneyPerRound, moneyPerRound);
	OPTIONINT_GETTER(MoneyWonForRound, moneyWonForRound);
	OPTIONINT_GETTER(MoneyWonForLives, moneyWonForLives);
	OPTIONINT_GETTER(MoneyWonPerKillPoint, moneyPerKillPoint);
	OPTIONINT_GETTER(MoneyWonPerMultiKillPoint, moneyPerMultiKillPoint);
	OPTIONINT_GETTER(MoneyWonPerAssistPoint, moneyPerAssistPoint);
	OPTIONINT_GETTER(MoneyWonPerHitPoint, moneyPerHitPoint);
	OPTIONBOOL_GETTER(MoneyPerHealthPoint, moneyPerHealthPoint);
	OPTIONBOOL_GETTER(LimitPowerByHealth, limitPowerByHealth);
	OPTIONBOOL_GETTER(GiveAllWeapons, giveAllWeapons);
	OPTIONBOOL_GETTER(DelayedDefenseActivation, delayedDefenseActivation);
	OPTIONSTRINGENUM_GETTER(Economy, economy);
	OPTIONINT_GETTER(ResignMode, resignMode);

	OPTIONINT_GETTER(ComputersDeathTalk, computersDeathTalk);
	OPTIONINT_GETTER(ComputersAttackTalk, computersAttackTalk);
	OPTIONSTRING_GETTER(BotNamePrefix, botNamePrefix);
	OPTIONBOOL_GETTER(RandomizeBotNames, randomizeBotNames);

	OPTIONBOOL_GETTER(CycleMaps, cycleMaps);
	OPTIONSTRING_GETTER(Landscapes, landscapes);

	OPTIONSTRING_GETTER(StatsLogger, statsLogger);
	OPTIONSTRING_GETTER(ServerFileLogger, serverFileLogger);
	
	OPTIONSTRING_GETTER(Mod, mod);
	OPTIONSTRING_GETTER(MOTD, motd);
	OPTIONINT_GETTER(ModDownloadSpeed, modDownloadSpeed);
	OPTIONINT_GETTER(MaxAvatarSize, maxAvatarSize);
	OPTIONSTRING_GETTER(ServerName, serverName);
	OPTIONSTRING_GETTER(ServerPassword, serverPassword);
	OPTIONSTRING_GETTER(PublishAddress, publishAddress);

	OPTIONBOOL_GETTER(AllowSameIP, allowSameIP);
	OPTIONBOOL_GETTER(AllowSameUniqueId, allowSameUniqueId);
	OPTIONBOOL_GETTER(PublishServer, publishServer);
	OPTIONBOOL_GETTER(ResidualPlayers, residualPlayers);

	OPTIONSTRING_GETTER(AuthHandler, authHandler);
	OPTIONBOOL_GETTER(RegisteredUserNames, registeredUserNames);
	OPTIONBOOL_GETTER(DebugFeatures, debugFeatures);

	OptionEntryString &getPlayerType(int no) { DIALOG_ASSERT(no<24); return mainOptions_.getPlayerType(no); }

	OptionsGame &getChangedOptions() { return changedOptions_; }
	OptionsGame &getMainOptions() { return mainOptions_; }
	OptionsGame &getLevelOptions() { return levelOptions_; }

	void updateChangeSet();
	bool commitChanges();

protected:
	OptionsGame mainOptions_;
	OptionsGame changedOptions_;
	OptionsGame levelOptions_;

};

#endif

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


#if !defined(__INCLUDE_OptionsTransienth_INCLUDE__)
#define __INCLUDE_OptionsTransienth_INCLUDE__

#include <common/Vector.h>
#include <common/OptionsGame.h>
#include <coms/NetBuffer.h>

class TankContainer;
class OptionsTransient
{
public:
	OptionsTransient(OptionsGame &optionsGame);
	virtual ~OptionsTransient();

	const char *getGameType();
	unsigned int getLeastUsedTeam(TankContainer &container);

	enum WallType
	{
		wallConcrete = 0,
		wallBouncy = 1,
		wallWrapAround = 2
	};
	enum WallSide
	{
		LeftSide = 0,
		RightSide,
		TopSide,
		BotSide
	};

	void reset();
	void newGame();
	void nextRound();

	// Walls
	WallType &getWallType() { return settings_.wallType_; }
	Vector &getWallColor();

	// Rounds left
	int getNoRoundsLeft() { return settings_.noRoundsLeft_; }
	int getCurrentGameNo() { return settings_.currentGameNo_; }

	// Wind
	float getWindAngle() { return settings_.windAngle_; }
	float getWindSpeed() { return settings_.windSpeed_; }
	Vector &getWindDirection() { return settings_.windDirection_; }
	bool getWindOn() { return (settings_.windSpeed_ > 0.0f); }

	// Used to send this structure over coms
	bool writeToBuffer(NetBuffer &buffer);
	bool readFromBuffer(NetBufferReader &reader);

protected:
	OptionsGame &optionsGame_;
	struct Settings
	{
		Settings();

		int noRoundsLeft_;
		int currentGameNo_;
		float windAngle_, windStartAngle_;
		float windSpeed_;
		Vector windDirection_;
		WallType wallType_;
	} settings_;

	bool newGame_;
	void newGameWind();
	void newGameWall();
	void nextRoundWind();

};


#endif

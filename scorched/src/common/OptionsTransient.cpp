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

#include <common/OptionsTransient.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>
#include <time.h>
#include <math.h>

OptionsTransient::Settings::Settings()  : 
	noRoundsLeft_(5), currentGameNo_(0)
{

}

OptionsTransient::OptionsTransient(OptionsGame &optionsGame) :
	optionsGame_(optionsGame)
{
	
}

OptionsTransient::~OptionsTransient()
{
}

bool OptionsTransient::writeToBuffer(NetBuffer &buffer)
{
	buffer.addToBuffer(settings_.noRoundsLeft_);
	buffer.addToBuffer(settings_.currentGameNo_);
	buffer.addToBuffer(settings_.windAngle_);
	buffer.addToBuffer(settings_.windStartAngle_);
	buffer.addToBuffer(settings_.windSpeed_);
	buffer.addToBuffer(settings_.windDirection_[0]);
	buffer.addToBuffer(settings_.windDirection_[1]);
	buffer.addToBuffer(settings_.windDirection_[2]);
	buffer.addToBuffer((int) settings_.wallType_);
	return true;
}

bool OptionsTransient::readFromBuffer(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(settings_.noRoundsLeft_)) return false;
	if (!reader.getFromBuffer(settings_.currentGameNo_)) return false;
	if (!reader.getFromBuffer(settings_.windAngle_)) return false;
	if (!reader.getFromBuffer(settings_.windStartAngle_)) return false;
	if (!reader.getFromBuffer(settings_.windSpeed_)) return false;
	if (!reader.getFromBuffer(settings_.windDirection_[0])) return false;
	if (!reader.getFromBuffer(settings_.windDirection_[1])) return false;
	if (!reader.getFromBuffer(settings_.windDirection_[2])) return false;
	int wallType;
	if (!reader.getFromBuffer(wallType)) return false;
	settings_.wallType_ = (WallType) wallType;
	return true;
}

void OptionsTransient::reset()
{
	settings_.currentGameNo_ = 0;
	settings_.noRoundsLeft_ = optionsGame_.getNoRounds();
}

void OptionsTransient::newGame()
{
	srand(time(0));
	settings_.currentGameNo_ = 0;
	settings_.noRoundsLeft_ --;
	newGameWind();
	newGameWall();
}

void OptionsTransient::nextRound()
{
	settings_.currentGameNo_++;
	nextRoundWind();
}

void OptionsTransient::newGameWind()
{
	switch(optionsGame_.getWindForce())
	{
		case OptionsGame::WindRandom:
			settings_.windSpeed_ = float((int)(RAND * 5.9f)); // ie range 0->5
			break;
		case OptionsGame::Wind1:
		case OptionsGame::Wind2:
		case OptionsGame::Wind3:
		case OptionsGame::Wind4:
		case OptionsGame::Wind5:
			settings_.windSpeed_ = float(int(optionsGame_.getWindForce()) - 1);
			break;
		case OptionsGame::WindNone:
		default:
			settings_.windSpeed_ = 0.0f;
			break;
	}

	if (settings_.windSpeed_ > 0.0f)
	{
		settings_.windStartAngle_ = settings_.windAngle_ = RAND * 360.0f;
		settings_.windDirection_ = Vector(sinf(settings_.windAngle_ / 180.0f * 3.14f),
				cosf(settings_.windAngle_ / 180.0f * 3.14f), 0.0f);
	}
	else
	{
		settings_.windStartAngle_ = settings_.windAngle_ = 0.0f;
		settings_.windDirection_ = Vector(0.0f, 0.0f, 0.0f);
	}
}

void OptionsTransient::nextRoundWind()
{
	if (optionsGame_.getWindType() != OptionsGame::WindOnMove)
	{
		return;
	}

	if (settings_.windSpeed_ > 0.0f)
	{
		settings_.windAngle_ = settings_.windStartAngle_ + ((RAND * 40.0f) - 20.0f);
		settings_.windDirection_ = Vector(sinf(settings_.windAngle_ / 180.0f * 3.14f),
				cosf(settings_.windAngle_ / 180.0f * 3.14f), 0.0f);
	}
}

Vector &OptionsTransient::getWallColor()
{
	static Vector wallColor;
	switch (getWallType())
	{
	case wallBouncy:
		wallColor = Vector(0.0f, 0.0f, 0.5f);
		break;
	default:
		wallColor = Vector(0.5f, 0.5f, 0.5f);
		break;
	}

	return wallColor;
}

void OptionsTransient::newGameWall()
{
	switch (optionsGame_.getWallType())
	{
	case OptionsGame::WallConcrete:
		settings_.wallType_ = wallConcrete;
		break;
	case OptionsGame::WallBouncy:
		settings_.wallType_ = wallBouncy;
		break;
	default:
		float r = RAND * 2.0f;
		settings_.wallType_ = WallType(int(r));
		break;
	}
}

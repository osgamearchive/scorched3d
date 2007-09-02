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

#include <tankai/TankAIStrings.h>
#include <server/ScorchedServer.h>
#include <common/Defines.h>
#include <common/OptionsScorched.h>

TankAIStrings *TankAIStrings::instance_ = 0;

TankAIStrings *TankAIStrings::instance()
{
	if (!instance_)
	{
		instance_ = new TankAIStrings;
	}

	return instance_;
}

TankAIStrings::TankAIStrings()
{
	// Check we have init correctly
	bool s1 = deathLines_.readFile((char *) getDataFile("data/talk/talk2.txt"));
	bool s2 = attackLines_.readFile((char *) getDataFile("data/talk/talk1.txt"));
	bool s3 = aiPlayerNames_.readFile((char *) getDataFile("data/ainames.txt"));
	bool s4 = playerNames_.readFile((char *) getDataFile("data/playernames.txt"));
	DIALOG_ASSERT(s1 && s2 && s3 && s4);
}

TankAIStrings::~TankAIStrings()
{
}

const char *TankAIStrings::getPlayerName()
{
	static int counter = 0;
	const char *playerName = 
		playerNames_.getLines()[counter++ % playerNames_.getLines().size()].c_str();
	return playerName;
}

const char *TankAIStrings::getAIPlayerName()
{
	static int counter = 0;
	if (ScorchedServer::instance()->getOptionsGame().getRandomizeBotNames())
	{
		counter = (int) rand();
	}

	const char *playerName = 
		aiPlayerNames_.getLines()[counter++ % aiPlayerNames_.getLines().size()].c_str();
	return playerName;
}

const char *TankAIStrings::getDeathLine(RandomGenerator &generator)
{
	const char *deathLine = 0;
	fixed percentage = 
		fixed(ScorchedServer::instance()->getOptionsGame().getComputersDeathTalk());
	fixed talkPer = generator.getRandFixed() * 100;
	if (talkPer < percentage)
	{
		deathLine = deathLines_.getLines()
			[generator.getRandUInt() % deathLines_.getLines().size()].c_str();
	}

	return deathLine;
}

const char *TankAIStrings::getAttackLine(RandomGenerator &generator)
{
	const char *attackLine = 0;
	fixed percentage = 
		fixed(ScorchedServer::instance()->getOptionsGame().getComputersAttackTalk());
	fixed talkPer = generator.getRandFixed() * 100;
	if (talkPer < percentage)
	{
		attackLine = attackLines_.getLines()
			[generator.getRandUInt() % attackLines_.getLines().size()].c_str();
	}
	
	return attackLine;
}

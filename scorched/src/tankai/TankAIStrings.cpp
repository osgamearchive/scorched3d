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


// TankAIStrings.cpp: implementation of the TankAIStrings class.
//
//////////////////////////////////////////////////////////////////////

#include <tankai/TankAIStrings.h>
#include <common/Defines.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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
	DIALOG_ASSERT(
		deathLines_.readFile(PKGDIR "data/talk/talk2.txt") &&
		attackLines_.readFile(PKGDIR "data/talk/talk1.txt") &&
		playerNames_.readFile(PKGDIR "data/ainames.txt"));
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

const char *TankAIStrings::getDeathLine()
{
	const char *deathLine = deathLines_.getRandomLine();
	return deathLine;
}

const char *TankAIStrings::getAttackLine()
{
	const char *attackLine = attackLines_.getRandomLine();
	return attackLine;
}

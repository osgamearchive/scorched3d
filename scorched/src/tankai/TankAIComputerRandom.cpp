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

#include <common/Defines.h>
#include <tankai/TankAIComputerRandom.h>
#include <tankai/TankAIStore.h>

TankAIComputerRandom::TankAIComputerRandom() 
{
	name_ = "Random";
	description_.setText("Random",
		"A computer controlled player.\n"
		"Randomly chooses from all available computer\n"
		"controled players (except targets).");
}

TankAIComputerRandom::~TankAIComputerRandom()
{
}

bool TankAIComputerRandom::parseConfig(XMLNode *node)
{
	return true;
}

TankAI *TankAIComputerRandom::getCopy(Tank *tank, ScorchedContext *context)
{
	DIALOG_ASSERT(TankAIStore::instance()->getAis().size());

	int tankNo = int(RAND * float(TankAIStore::instance()->getAis().size()));
	std::list<TankAI *>::iterator itor = 
		TankAIStore::instance()->getAis().begin();
	for (int i=0; i<tankNo; i++) itor++;

	TankAI *result = (*itor);
	if (!result->availableForRandom()) return getCopy(tank, context);
	return result->getCopy(tank, context);
}


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

#include <XML/XMLFile.h>
#include <common/Defines.h>
#include <tankai/TankAIStore.h>
#include <tankai/TankAIComputer.h>
#include <stdlib.h>

TankAIStore::TankAIStore()
{

}

TankAIStore::~TankAIStore()
{

}

void TankAIStore::clearAIs()
{
	while (!ais_.empty())
	{
		TankAI *ai = ais_.front();
		ais_.pop_front();
		delete ai;
	}
}

bool TankAIStore::loadAIs(AccessoryStore *store)
{
	// Load key definition file
	XMLFile file;
	if (!file.readFile(getDataFile("data/tankais.xml")))
	{
		dialogMessage("TankAIStore", 
					  formatString("Failed to parse \"%s\"\n%s", 
					  "data/tankais.xml",
					  file.getParserError()));
		return false;
	}

	// Check file exists
	if (!file.getRootNode())
	{
		dialogMessage("TankAIStore",
					  formatString("Failed to find tank ai definition file \"%s\"",
					  "data/tankais.xml"));
		return false;		
	}

	// Itterate all of the keys in the file
    std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = file.getRootNode()->getChildren();
    for (childrenItor = children.begin();
		 childrenItor != children.end();
		 childrenItor++)
    {
		// Parse the ai entry
        XMLNode *currentNode = (*childrenItor);
		if (strcmp(currentNode->getName(), "ai"))
		{
			dialogMessage("TankAIStore",
						  "Failed to find ai node");
			return false;
		}

		TankAIComputer *computer = new TankAIComputer;
		if (!computer->parseConfig(store, currentNode))
		{
			return false;
		}
		if (store)
		{
			// Only check the rest of the children
			// when we are not doing a shallow load
			if (!currentNode->failChildren()) return false;
		}

		addAI(computer);
	}

	// Add the random player
	{
		bool foundRandom = false;
		std::list<TankAI *>::iterator itor;
		for (itor = ais_.begin();
			itor != ais_.end();
			itor++)
		{
			TankAI *ai = (*itor);
			if (ai->availableForRandom())
			{
				foundRandom = true;
				break;
			}
		}
		if (!foundRandom)
		{
			dialogMessage("TankAIStore",
						  "No tank ais are marked availableforrandom");
			return false;
		}

		TankAI *tankAI = new TankAIComputer;
		tankAI->getDescription().setText("Random",
			"A computer controlled player.\n"
			"Randomly chooses from all available computer\n"
			"controled players (except targets).");
		addAI(tankAI);
	}

	return true;
}

TankAI *TankAIStore::getAIByName(const char *name)
{
	// Random
	if (0 == strcmp(name, "Random"))
	{
		int tankNo = int(RAND * float(ais_.size()));
		std::list<TankAI *>::iterator itor = ais_.begin();
		for (int i=0; i<tankNo; i++) itor++;

		TankAI *result = (*itor);
		if (0 == strcmp(result->getName(), "Random")) return getAIByName(name);
		if (!result->availableForRandom()) return getAIByName(name);
		return result;
	}

	// Computers
	std::list<TankAI *>::iterator itor;
	for (itor = ais_.begin();
		itor != ais_.end();
		itor++)
	{
		if (!strcmp((*itor)->getName(), name)) return (*itor);
	}

	return 0;
}

void TankAIStore::addAI(TankAI *ai)
{
	ais_.push_back(ai);
}

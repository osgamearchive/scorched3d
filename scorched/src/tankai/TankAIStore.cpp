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
#include <tankai/TankAIHuman.h>
#include <tankai/TankAIComputerMoron.h>
#include <tankai/TankAIComputerTosser.h>
#include <tankai/TankAIComputerRandom.h>
#include <tankai/TankAIComputerPShark.h>
#include <tankai/TankAIComputerTarget.h>
#include <stdlib.h>

TankAIStore *TankAIStore::instance_ = 0;

TankAIStore *TankAIStore::instance()
{
	if (!instance_)
	{
		instance_ = new TankAIStore;
	}
	return instance_;
}

TankAIStore::TankAIStore()
{

}

TankAIStore::~TankAIStore()
{

}

bool TankAIStore::loadAIs()
{
	// Load key definition file
	XMLFile file;
	if (!file.readFile(getDataFile("data/tankais.xml")))
	{
		dialogMessage("TankAIStore", 
					  "Failed to parse \"%s\"\n%s", 
					  "data/tankais.xml",
					  file.getParserError());
		return false;
	}

	// Check file exists
	if (!file.getRootNode())
	{
		dialogMessage("TankAIStore",
					  "Failed to find tank ai definition file \"%s\"",
					  "data/tankais.xml");
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

		XMLNode *type = currentNode->getNamedParameter("type");
		if (!type)
		{
			dialogMessage("TankAIStore",
						  "Failed to find type node");
			return false;
		}
		const char *typeName = type->getContent();

		TankAIComputer *computer = 0;
		if (strcmp(typeName, "Target") == 0)
		{
			computer = new TankAIComputerTarget;
		}
		else if (strcmp(typeName, "Moron") == 0)
		{
			computer = new TankAIComputerMoron;
		} 
		else if (strcmp(typeName, "Shark") == 0)
		{
			computer = new TankAIComputerPShark;
		} 
		else if (strcmp(typeName, "Shooter") == 0)
		{
			computer = new TankAIComputerShooter;
		}
		else if (strcmp(typeName, "Tosser") == 0)
		{
			computer = new TankAIComputerTosser;
		}
		else
		{
			dialogMessage("TankAIStore",
						  "Failed to find ai type \"%s\"",
						  typeName);
			return false;
		}
		if (!computer->parseConfig(currentNode))
		{
			return false;
		}

		addAI(computer);
	}
	addAI(new TankAIComputerRandom);

	return true;
}

TankAI *TankAIStore::getAIByName(const char *name)
{
	// Human
	static TankAIHuman humanAI;
	if (0 == strcmp(name, "Human")) return &humanAI;

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

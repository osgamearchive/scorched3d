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


#include <tankai/TankAIStore.h>
#include <tankai/TankAIComputerMoron.h>
#include <tankai/TankAIComputerChooser.h>
#include <tankai/TankAIComputerRandom.h>
#include <tankai/TankAIComputerPShark.h>
#include <tankai/TankAIComputerDHTest.h>

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
	addComputerAI(new TankAIComputerRandom(0));
	addComputerAI(new TankAIComputerMoron(0));
	addComputerAI(new TankAIComputerShooter(0));
	addComputerAI(new TankAIComputerTosser(0));
	addComputerAI(new TankAIComputerChooser(0));
	addComputerAI(new TankAIComputerPShark(0));

	// Test purpose only
	 addComputerAI(new TankAIComputerDHTest(0));
}

TankAIStore::~TankAIStore()
{

}

TankAIComputer *TankAIStore::getAIByName(const char *name)
{
	std::list<TankAIComputer *>::iterator itor;
	for (itor = ais_.begin();
		itor != ais_.end();
		itor++)
	{
		if (!strcmp((*itor)->getName(), name)) return (*itor);
	}

	return 0;
}

void TankAIStore::addComputerAI(TankAIComputer *ai)
{
	ais_.push_back(ai);
}

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

#include <tank/TankSort.h>

bool TankSort::SortOnScore::operator()(const Tank *x, const Tank *y) const
{
	TankScore &scoreX = ((Tank *)x)->getScore();
	TankScore &scoreY = ((Tank *)y)->getScore();

	if (scoreX.getWins() > scoreY.getWins()) return true;
	if (scoreX.getWins() == scoreY.getWins())
	{
		if (scoreX.getMoney() > scoreY.getMoney()) return true;
		if (scoreX.getMoney() == scoreY.getMoney())
		{
			if (scoreX.getKills() > scoreY.getKills()) return true;
			if (scoreX.getKills() == scoreY.getKills())
			{
				if (strcmp(((Tank *)x)->getName(), ((Tank *)y)->getName()) < 0) return true;
			}
		}
	}

	return false;
}

void TankSort::getSortedTanks(std::list<Tank *> &list)
{
	std::list<Tank *> newList;
	while (!list.empty())
	{
		std::list<Tank *>::iterator removeItor = list.begin();
		std::list<Tank *>::iterator itor = list.begin(); itor++;
		for (;itor != list.end(); itor++)
		{
			static TankSort::SortOnScore compare;
			if (!compare(*itor, *removeItor)) removeItor = itor;
		}

		newList.push_front(*removeItor);
		list.erase(removeItor);
	}

	list.swap(newList);
}

void TankSort::getSortedTanksIds(TankContainer &container, std::list<unsigned int> &list)
{
	std::list<Tank *> sortedTanks;
	std::map<unsigned int, Tank *> &tanks = 
		container.getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		sortedTanks.push_back(tank);
	}

	getSortedTanks(sortedTanks);
	std::list<Tank *>::iterator resultitor;
	for (resultitor = sortedTanks.begin();
		resultitor != sortedTanks.end();
		resultitor++)
	{
		list.push_back((*resultitor)->getPlayerId());
	}
}

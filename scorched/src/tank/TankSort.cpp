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
#include <tank/Tank.h>
#include <tank/TankContainer.h>
#include <common/OptionsGame.h>

int TankSort::compare(ScorchedContext &context,
		int kills1, int money1, int wins1, const char *name1,
		int kills2, int money2, int wins2, const char *name2)
{
	switch (context.optionsGame->getScoreType())
	{
	case OptionsGame::ScoreWins:
		if (wins1 > wins2) return 1;
		if (wins1 == wins2)
		{
			if (kills1 > kills2) return 1;
			if (kills1 == kills2)
			{
				if (money1 > money2) return 1;
				if (money1 == money2)
				{

					return strcmp(name1, name2);
				}
			}
		}
		return -1;
	case OptionsGame::ScoreKills:
		if (kills1 > kills2) return 1;
		if (kills1 == kills2)
		{
			if (wins1 > wins2) return 1;
			if (wins1 == wins2)
			{
				if (money1 > money2) return 1;
				if (money1 == money2)
				{
					return strcmp(name1, name2);
				}
			}
		}
		return -1;
	case OptionsGame::ScoreMoney:
		if (money1 > money2) return 1;
		if (money1 == money2)
		{
			if (kills1 > kills2) return 1;
			if (kills1 == kills2)
			{
				return strcmp(name1, name2);
			}
		}
		return -1;
	}
	// Never get here
	return strcmp(name1, name2);
}

bool TankSort::SortOnScore::operator()(const Tank *x, const Tank *y, ScorchedContext &context) const
{
	Tank &tankX = *((Tank *) x);
	Tank &tankY = *((Tank *) y);
	TankScore &scoreX = tankX.getScore();
	TankScore &scoreY = tankY.getScore();

	if (tankX.getState().getSpectator() &&
		tankY.getState().getSpectator())
	{
		if (strcmp(((Tank *)x)->getName(), ((Tank *)y)->getName()) < 0) return true;
		return false;
	}
	else if (tankX.getState().getSpectator())
	{
		return false;
	}
	else if (tankY.getState().getSpectator())
	{
		return true;
	}

	int compareResult = compare(context, 
		scoreX.getKills(), scoreX.getMoney(), scoreX.getWins(), tankX.getName(),
		scoreY.getKills(), scoreY.getMoney(), scoreY.getWins(), tankY.getName());

	return (compareResult > 0);
}

int TankSort::getWinningTeam(ScorchedContext &context)
{
	struct Score
	{
		Score() : wins(0), kills(0), money(0) {}
	
		int wins;
		int kills;
		int money;
	};
	Score scores[4];

	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> &tanks = 
		context.tankContainer->getPlayingTanks();
	for (itor = tanks.begin();
		itor != tanks.end();
		itor ++)
	{
		Tank *current = (*itor).second;
		if (current->getTeam() > 0 && !current->getState().getSpectator()) 
		{
			scores[current->getTeam() - 1].wins += current->getScore().getWins();
			scores[current->getTeam() - 1].kills += current->getScore().getKills();
			scores[current->getTeam() - 1].money += current->getScore().getMoney();
		}
	}

	for (int i=0; i<context.optionsGame->getTeams(); i++)
	{
		bool won = true;
		for (int j=0; j<context.optionsGame->getTeams(); j++)
		{
			if (i != j)
			{
				if (compare(context,
					scores[i].kills, scores[i].money, scores[i].wins, "",
					scores[j].kills, scores[j].money, scores[j].wins, "") < 1)
				{
					won = false;
				}
			}
		}
		
		if (won) return i;
	}

	return 0;
}

void TankSort::getSortedTanks(std::list<Tank *> &list, ScorchedContext &context)
{
	std::list<Tank *> newList;
	while (!list.empty())
	{
		std::list<Tank *>::iterator removeItor = list.begin();
		std::list<Tank *>::iterator itor = list.begin(); itor++;
		for (;itor != list.end(); itor++)
		{
			static TankSort::SortOnScore compare;
			if (!compare(*itor, *removeItor, context)) removeItor = itor;
		}

		newList.push_front(*removeItor);
		list.erase(removeItor);
	}

	list = newList;
}

void TankSort::getSortedTanksIds(ScorchedContext &context, std::list<unsigned int> &list)
{
	std::list<Tank *> sortedTanks;
	std::map<unsigned int, Tank *> &tanks = 
		context.tankContainer->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		sortedTanks.push_back(tank);
	}

	getSortedTanks(sortedTanks, context);
	std::list<Tank *>::iterator resultitor;
	for (resultitor = sortedTanks.begin();
		resultitor != sortedTanks.end();
		resultitor++)
	{
		list.push_back((*resultitor)->getPlayerId());
	}
}

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

#if !defined(AFX_TANKCONTAINER_H__56AF98E2_E188_45EC_AA25_1865ADBBA3F1__INCLUDED_)
#define AFX_TANKCONTAINER_H__56AF98E2_E188_45EC_AA25_1865ADBBA3F1__INCLUDED_

#include <map>
#include <tank/Tank.h>

class TankContainer  
{
public:
	TankContainer();
	virtual ~TankContainer();

	void addTank(Tank *tank);
	Tank *getCurrentTank();

	unsigned int getCurrentDestinationId() { return destinationId_; }
	void setCurrentDestinationId(unsigned int did) { destinationId_ = did; }
	unsigned int getCurrentPlayerId() { return playerId_; }
	void setCurrentPlayerId(unsigned int pid) { playerId_ = pid; }

	Tank *removeTank(unsigned int playerId);
	Tank *getTankById(unsigned int id);
	Tank *getTankByPos(unsigned int pos);
	Tank *getTankByName(const char *name);
	
	void newGame();
	void clientNewGame();
	void clientNextShot();
	
	void resetTanks();
	void setAllDead();
	void removeAllTanks();
	void setAllNotReady();
	bool allReady();
	int aliveCount();
	int teamCount();

	int getNoOfTanks() { return (int) playingTanks_.size(); }
	int getNoOfNonSpectatorTanks();
	std::map<unsigned int, Tank *> &getPlayingTanks() { return playingTanks_; }

protected:
	static TankContainer *instance_;
	unsigned int playerId_;
	unsigned int destinationId_;
	std::map<unsigned int, Tank *> playingTanks_;

};

#endif // !defined(AFX_TANKCONTAINER_H__56AF98E2_E188_45EC_AA25_1865ADBBA3F1__INCLUDED_)

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


// Tank.h: interface for the Tank class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TANK_H__52F37177_46EA_49C8_9B58_E6C57ABDB78A__INCLUDED_)
#define AFX_TANK_H__52F37177_46EA_49C8_9B58_E6C57ABDB78A__INCLUDED_

#include <tank/TankAccessories.h>
#include <tank/TankScore.h>
#include <tank/TankState.h>
#include <tank/TankPhysics.h>
#include <tank/TankModelId.h>
#include <tank/TankAI.h>

class Tank  
{
public:
	// Constructor for tank
	// The name and color are copied
	Tank(unsigned int playerId, const char *name, 
		 Vector &color, TankModelId &modelId);
	virtual ~Tank();

	// Called when the state changes
	void reset();
	void newGame();
	void nextRound();

	// Serialize the tank
    bool writeMessage(NetBuffer &buffer);
    bool readMessage(NetBufferReader &reader);

	// The base attributes of the tank
	unsigned int &getPlayerId() { return playerId_; }
	const char *getName() { return name_.c_str(); }
	const char *getUniqueId() { return uniqueId_.c_str(); }
	unsigned int getNameLen() { return name_.size(); }
	Vector &getColor() { return color_; }

	// Other attributes
	TankAccessories& getAccessories() { return accessories_; }
	TankScore &getScore() { return score_; }
	TankPhysics &getPhysics() { return physics_; }
	TankState &getState() { return state_; }
	TankModelId &getModel() { return model_; }
	TankAI *getTankAI() { return tankAI_; }
	void setTankAI(TankAI *ai) { tankAI_ = ai; }
	void setTankName(const char *name);
	void setUnqiueId(const char *id);

protected:
	TankModelId model_;
	TankAccessories accessories_;
	TankScore score_;
	TankPhysics physics_;
	TankState state_;
	TankAI *tankAI_;
	Vector color_;
	std::string name_;
	std::string uniqueId_;
	unsigned int playerId_;

};

#endif // !defined(AFX_TANK_H__52F37177_46EA_49C8_9B58_E6C57ABDB78A__INCLUDED_)

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

#if !defined(AFX_TANK_H__52F37177_46EA_49C8_9B58_E6C57ABDB78A__INCLUDED_)
#define AFX_TANK_H__52F37177_46EA_49C8_9B58_E6C57ABDB78A__INCLUDED_

#include <tank/TankAccessories.h>
#include <tank/TankScore.h>
#include <tank/TankState.h>
#include <tank/TankPhysics.h>
#include <tank/TankModelId.h>
#include <tankai/TankAI.h>

class Tank  
{
public:
	// Constructor for tank
	// The name and color are copied
	Tank(ScorchedContext &context, 
		unsigned int playerId, 
		unsigned int destinationId,
		const char *name, 
		Vector &color, TankModelId &modelId);
	virtual ~Tank();

	// Called when the state changes
	void reset();
	void newGame();
	void clientNewGame();

	// Serialize the tank
	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);

	// The base attributes of the tank
	unsigned int getPlayerId() { return playerId_; }
	unsigned int getDestinationId() { return destinationId_; }
	void setDestinationId(unsigned int id) { destinationId_ = id; }
	unsigned int getTeam() { return team_; }
	void setTeam(unsigned int team) { team_ = team; }
	const char *getName() { return name_.c_str(); }
	void setName(const char *name) { name_ = name; }
	unsigned int getNameLen() { return name_.size(); }
	const char *getUniqueId() { return uniqueId_.c_str(); }
	void setUnqiueId(const char *id) { uniqueId_ = id; }
	const char *getHostDesc() { return hostDesc_.c_str(); }
	void setHostDesc(const char *id) { hostDesc_ = id; }
	TankAI *getTankAI() { return tankAI_; }
	void setTankAI(TankAI *ai);
	TankModelId &getModel() { return model_; }
	void setModel(TankModelId &model) { model_ = model; }
	Vector &getColor();
	void setColor(Vector &color) { color_ = color; }

	// Other attributes
	TankAccessories& getAccessories() { return accessories_; }
	TankScore &getScore() { return score_; }
	TankPhysics &getPhysics() { return physics_; }
	TankState &getState() { return state_; }

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
	std::string hostDesc_;
	unsigned int team_;
	unsigned int playerId_;
	unsigned int destinationId_;

};

#endif // !defined(AFX_TANK_H__52F37177_46EA_49C8_9B58_E6C57ABDB78A__INCLUDED_)

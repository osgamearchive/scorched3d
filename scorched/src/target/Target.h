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

#if !defined(AFX_TARGET_H__52F37177_46EA_49C8_9B58_E6C57ABDB78A__INCLUDED_)
#define AFX_TARGET_H__52F37177_46EA_49C8_9B58_E6C57ABDB78A__INCLUDED_

#include <target/TargetLife.h>
#include <target/TargetModelId.h>
#include <target/TargetShield.h>
#include <target/TargetParachute.h>

class Target  
{
public:
	enum TargetType
	{
		eOther = 0,
		eTank = 1
	};

	Target(unsigned int playerId, 
		TargetModelId &modelId, 
		const char *name, 
		ScorchedContext &context);
	virtual ~Target();

	virtual void newGame();

	// Position
	TargetModelId &getModel() { return model_; }
	void setModel(TargetModelId &model) { model_ = model; }
	void setTargetPosition(Vector &position);
	Vector &getTargetPosition() { return targetPosition_; }

	virtual bool getAlive();
	virtual TargetType getTargetType() { return eOther; }
	unsigned int getPlayerId() { return playerId_; }

	// Weapons
	TargetLife &getLife() { return life_; }
	TargetShield &getShield() { return shield_; }
	TargetParachute &getParachute() { return parachute_; }

	// Name
	const char *getName() { return name_.c_str(); }
	void setName(const char *name) { name_ = name; }
	unsigned int getNameLen() { return name_.size(); }

	// Serialize the target
	virtual bool writeMessage(NetBuffer &buffer, bool writeAccessories);
	virtual bool readMessage(NetBufferReader &reader);

protected:
	unsigned int playerId_;
	ScorchedContext &context_;
	Vector targetPosition_;
	TargetModelId model_;
	TargetLife life_;
	TargetShield shield_;
	TargetParachute parachute_;
	std::string name_;

};

#endif // !defined(AFX_TARGET_H__52F37177_46EA_49C8_9B58_E6C57ABDB78A__INCLUDED_)

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

#include <engine/ScorchedContext.h>
#include <common/Vector.h>
#include <string>

class NetBuffer;
class NetBufferReader;
class TargetState;
class TargetLife;
class TargetShield;
class TargetGroupEntry;
class TargetParachute;
class TargetRenderer;
class Weapon;
class Target  
{
public:
	Target(unsigned int playerId, 
		const char *name, 
		ScorchedContext &context);
	virtual ~Target();

	virtual void newGame();

	virtual bool getAlive();
	virtual bool isTarget() { return true; }
	virtual bool isTemp();
	unsigned int getPlayerId() { return playerId_; }

	// Weapons
	TargetLife &getLife() { return *life_; }
	TargetShield &getShield() { return *shield_; }
	TargetParachute &getParachute() { return *parachute_; }
	TargetGroupEntry &getGroup() { return *group_; }
	TargetState &getTargetState() { return *targetState_; }

	// Actions
	void setDeathAction(Weapon *deathAction) { deathAction_ = deathAction; }
	void setBurnAction(Weapon *burnAction) { burnAction_ = burnAction; }
	virtual Weapon *getDeathAction() { return deathAction_; }
	virtual Weapon *getBurnAction() { return burnAction_; }

	// Renderer
	float getBorder() { return border_; }
	void setBorder(float b) { border_ = b; }
	TargetRenderer *getRenderer() { return renderer_; }
	void setRenderer(TargetRenderer *renderer) { renderer_ = renderer; }

	// Name
	const char *getName() { return name_.c_str(); }
	void setName(const char *name) { name_ = name; }
	unsigned int getNameLen() { return name_.size(); }

	// Serialize the target
	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);

protected:
	unsigned int playerId_;
	float border_;
	ScorchedContext &context_;
	TargetLife *life_;
	TargetShield *shield_;
	TargetParachute *parachute_;
	TargetRenderer *renderer_;
	TargetState *targetState_;
	TargetGroupEntry *group_;
	std::string name_;
	Weapon *deathAction_, *burnAction_;

};

#endif // !defined(AFX_TARGET_H__52F37177_46EA_49C8_9B58_E6C57ABDB78A__INCLUDED_)

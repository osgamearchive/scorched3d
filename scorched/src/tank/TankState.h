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


#if !defined(__INCLUDE_TankStateh_INCLUDE__)
#define __INCLUDE_TankStateh_INCLUDE__

#include <coms/NetBuffer.h>

class ScorchedContext;
class TankState
{
public:
	enum State
	{
		sPending,
		sNormal,
		sDead
	};

	enum ReadyState
	{
		sReady,
		SNotReady
	};

	TankState(ScorchedContext &context);
	virtual ~TankState();

	// State
	void nextShot();
	void newGame();
	void reset();
	void setReady() { readyState_ = sReady; }
	void setState(State s) { state_ = s; }
	State getState() { return state_; }
	ReadyState getReadyState() { return readyState_; }
	const char *getStateString();
	const char *getSmallStateString();

	// Tank Life / Health
	float getLife() { return life_; }
	void setLife(float life);

	// Power of gun
	float getPower() { return power_; }
	float getOldPower() { return oldPower_; }
	float changePower(float power, bool diff=true);

	void setSpectator(bool s) { spectator_ = s; }
	bool getSpectator() { return spectator_; }

	const char *getPowerString();

	// Serialize the tank
    bool writeMessage(NetBuffer &buffer);
    bool readMessage(NetBufferReader &reader);

protected:
	ScorchedContext &context_;
	State state_;
	ReadyState readyState_;
	float life_;
	float power_, oldPower_;
	bool spectator_;

};


#endif

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

class Tank;
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

	TankState(ScorchedContext &context, unsigned int playerId);
	virtual ~TankState();
	
	void setTank(Tank *tank) { tank_ = tank; }

	// State Modifiers
	void newGame();
	void reset();
	
	// State
	void setReady() { readyState_ = sReady; }
	void setNotReady() { readyState_ = SNotReady; }
	void setState(State s) { state_ = s; }
	State getState() { return state_; }
	ReadyState getReadyState() { return readyState_; }
	void setSpectator(bool s) { spectator_ = s; }
	bool getSpectator() { return spectator_; }	
	void setLoading(bool l) { loading_ = l; }
	bool getLoading() { return loading_; }
	void setAdmin(bool admin) { admin_ = admin; }
	bool getAdmin() { return admin_; }
	
	const char *getStateString();
	const char *getSmallStateString();

	// Tank Life / Health
	float getLife() { return life_; }
	void setLife(float life);

	// Serialize the tank
	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);

protected:
	Tank *tank_;
	ScorchedContext &context_;
	State state_;
	ReadyState readyState_;
	float life_;
	bool admin_;
	bool spectator_;
	bool loading_;

};

#endif


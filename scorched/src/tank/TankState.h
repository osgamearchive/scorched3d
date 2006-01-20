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

class TankAdmin;
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
	void newMatch();
	void clientNewGame();
	
	// State
	void setReady() { readyState_ = sReady; }
	void setNotReady() { readyState_ = SNotReady; }
	void setState(State s);
	State getState() { return state_; }
	ReadyState getReadyState() { return readyState_; }
	void setSpectator(bool s) { spectator_ = s; }
	bool getSpectator() { return spectator_; }	
	void setLoading(bool l) { loading_ = l; }
	bool getLoading() { return loading_; }
	void setInitializing(bool i) { initializing_ = i; }
	bool getInitializing() { return initializing_; }
	void setAdmin(TankAdmin *admin);
	TankAdmin *getAdmin() { return admin_; }
	void setAdminTries(int adminTries) { adminTries_ = adminTries; }
	int getAdminTries() { return adminTries_; }
	void setMuted(bool muted) { muted_ = muted; }
	bool getMuted() { return muted_; }
	void setSkipShots(bool skip) { skipshots_ = skip; }
	bool getSkipShots() { return skipshots_; }
	
	const char *getStateString();
	const char *getSmallStateString();

	// Serialize the tank
	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);

protected:
	Tank *tank_;
	ScorchedContext &context_;
	State state_;
	ReadyState readyState_;
	TankAdmin *admin_; int adminTries_;
	bool spectator_;
	bool initializing_;
	bool loading_;
	bool muted_;
	bool skipshots_;

};

#endif


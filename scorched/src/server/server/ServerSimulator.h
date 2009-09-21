////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2009
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

#if !defined(AFX_ServerSimulator_H__86995B4A_478E_4CFE_BD4C_79128DE51904__INCLUDED_)
#define AFX_ServerSimulator_H__86995B4A_478E_4CFE_BD4C_79128DE51904__INCLUDED_

#include <engine/Simulator.h>
#include <coms/ComsMessageHandler.h>
#include <coms/ComsLoadLevelMessage.h>

class ServerSimulator : 
	public Simulator, 
	public ComsMessageHandlerI
{
public:
	ServerSimulator();
	virtual ~ServerSimulator();

	void addSimulatorAction(SimAction *action);

	bool getSendActionsEmpty() { return sendActions_.empty() && simActions_.empty(); }

	void newLevel();
	ComsLoadLevelMessage &getLevelMessage();

	virtual bool processMessage(
		NetMessage &netMessage,
		const char *messageType,
		NetBufferReader &reader);

protected:
	std::list<SimAction *> sendActions_;
	fixed nextSendTime_, nextEventTime_;
	fixed sendStepSize_;

	virtual bool continueToSimulate();
	void nextSendTime();
	fixed calcSendStepSize();
	ComsLoadLevelMessage *levelMessage_;
};

#endif // !defined(AFX_ServerSimulator_H__86995B4A_478E_4CFE_BD4C_79128DE51904__INCLUDED_)
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

#if !defined(AFX_TankStartMoveSimAction_H__2C00E711_B337_4665_AB54_C6661FD67E5D__INCLUDED_)
#define AFX_TankStartMoveSimAction_H__2C00E711_B337_4665_AB54_C6661FD67E5D__INCLUDED_

#include <simactions/SimAction.h>

class TankStartMoveSimAction : public SimAction
{
public:
	TankStartMoveSimAction();
	TankStartMoveSimAction(unsigned int playerId, unsigned int moveId, 
		float timeout, bool buying);
	virtual ~TankStartMoveSimAction();

	unsigned int getPlayerId() { return playerId_; }
	unsigned int getMoveId() { return moveId_; }
	bool getBuying() { return buying_; }

	virtual bool invokeAction(ScorchedContext &context);

	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);

REGISTER_CLASS_HEADER(TankStartMoveSimAction);
protected:
	unsigned int playerId_;
	unsigned int moveId_;
	float timeout_;
	bool buying_;
};

#endif // !defined(AFX_TankStartMoveSimAction_H__2C00E711_B337_4665_AB54_C6661FD67E5D__INCLUDED_)

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

#include <simactions/AdminSimAction.h>
#include <common/Logger.h>
#include <common/OptionsTransient.h>
#include <tank/TankContainer.h>
#include <tank/TankState.h>
#include <target/TargetLife.h>

REGISTER_CLASS_SOURCE(AdminSimAction);

AdminSimAction::AdminSimAction()
{
}

AdminSimAction::AdminSimAction(AdminType type, unsigned int playerId, fixed amount) :
	type_(type), playerId_(playerId), amount_(amount)
{
}

AdminSimAction::~AdminSimAction()
{
}

bool AdminSimAction::invokeAction(ScorchedContext &context)
{
	switch (type_)
	{
	case eKillAll:
		killAll(context);
		break;
	case eNewGame:
		newGame(context);
		break;
	case eSlap:
		slap(context);
		break;
	}

	return true;
}

bool AdminSimAction::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer((int) type_);
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(amount_);
	return true;
}

bool AdminSimAction::readMessage(NetBufferReader &reader)
{
	int type;
	if (!reader.getFromBuffer(type)) return false;
	type_ = (AdminType) type;
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(amount_)) return false;
	return true;
}

void AdminSimAction::killAll(ScorchedContext &context)
{
	Logger::log("Killing all players");

	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> &tanks = 
		context.getTankContainer().getAllTanks();
	for (itor = tanks.begin();
		 itor != tanks.end();
		 itor++)
	{
		Tank *current = (*itor).second;
		if (current->getState().getState() == TankState::sNormal)
		{
			current->getState().setState(TankState::sDead);
			current->getState().setLives(0);
		}
	}
}

void AdminSimAction::newGame(ScorchedContext &context)
{
	killAll(context);
	context.getOptionsTransient().startNewGame();	
}

void AdminSimAction::slap(ScorchedContext &context)
{
	Tank *targetTank = context.
		getTankContainer().getTankById(playerId_);
	if (!targetTank) return;

	targetTank->getLife().setLife(
		targetTank->getLife().getLife() -  amount_);
}

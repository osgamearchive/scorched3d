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

#include <client/ScorchedClient.h>
#include <client/ClientBuyAccessoryHandler.h>
#include <coms/ComsBuyAccessoryMessage.h>
#include <weapons/AccessoryStore.h>

ClientBuyAccessoryHandler *ClientBuyAccessoryHandler::instance_ = 0;

ClientBuyAccessoryHandler *ClientBuyAccessoryHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ClientBuyAccessoryHandler;
	}
	return instance_;
}

ClientBuyAccessoryHandler::ClientBuyAccessoryHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsBuyAccessoryMessage",
		this);
}

ClientBuyAccessoryHandler::~ClientBuyAccessoryHandler()
{
}

bool ClientBuyAccessoryHandler::processMessage(unsigned int id,
	const char *messageType,
	NetBufferReader &reader)
{
	// Decode the connect message
	ComsBuyAccessoryMessage message;
	if (!message.readMessage(reader)) return false;

	// Check tank exists and is alive
	Tank *tank = ScorchedClient::instance()->getTankContainer().getTankById(message.getPlayerId());
	if (!tank) return true;
	
	// Check this is not a player running on this client
	// As this message will have already been processed
	// as the stuff is actuall bought
	if (tank->getDestinationId() == 
		ScorchedClient::instance()->getTankContainer().getCurrentDestinationId())
	{
		return true;
	}

	// Check accessory exists
	Accessory *accessory = 
		AccessoryStore::instance()->findByAccessoryId(message.getAccessoryId());
	if (!accessory) return true;

	if (message.getBuy())
	{
		tank->getAccessories().add(accessory);
		tank->getScore().setMoney(tank->getScore().getMoney() - accessory->getPrice());
	}
	else
	{
		tank->getAccessories().rm(accessory);
		tank->getScore().setMoney(tank->getScore().getMoney() + accessory->getSellPrice());
	}
    
	return true;
}
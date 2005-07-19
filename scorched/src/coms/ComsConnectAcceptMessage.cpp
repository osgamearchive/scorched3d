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
#include <server/ScorchedServer.h>
#include <coms/ComsConnectAcceptMessage.h>
#include <weapons/AccessoryStore.h>
#include <common/OptionsGame.h>

ComsConnectAcceptMessage::ComsConnectAcceptMessage(unsigned int destinationId,
	const char *serverName,
	const char *publishAddress,
	const char *uniqueId) :
	ComsMessage("ComsConnectAcceptMessage"),
	destinationId_(destinationId),
	serverName_(serverName),
	publishAddress_(publishAddress),
	uniqueId_(uniqueId)
{

}

ComsConnectAcceptMessage::~ComsConnectAcceptMessage()
{

}

bool ComsConnectAcceptMessage::writeMessage(NetBuffer &buffer, unsigned int destinationId)
{
	buffer.addToBuffer(destinationId_);
	buffer.addToBuffer(serverName_);
	buffer.addToBuffer(publishAddress_);
	buffer.addToBuffer(uniqueId_);
	buffer.addToBuffer(serverGif_.getBufferUsed());
	buffer.addDataToBuffer(serverGif_.getBuffer(), serverGif_.getBufferUsed());
	if (!ScorchedServer::instance()->getOptionsGame().
		writeToBuffer(buffer, false)) return false;
	return true;
}

bool ComsConnectAcceptMessage::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(destinationId_)) return false;
	if (!reader.getFromBuffer(serverName_)) return false;
	if (!reader.getFromBuffer(publishAddress_)) return false;
	if (!reader.getFromBuffer(uniqueId_)) return false;
	unsigned int used;
	if (!reader.getFromBuffer(used)) return false;
	serverGif_.allocate(used);
	serverGif_.reset();
	serverGif_.setBufferUsed(used);
	reader.getDataFromBuffer(serverGif_.getBuffer(), used);
	if (!ScorchedClient::instance()->getOptionsGame().
		readFromBuffer(reader, false)) return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2011
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
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#include <net/NetServerTCP3Destination.h>
#include <common/Logger.h>

NetServerTCP3Destination::NetServerTCP3Destination(
	NetMessageHandler *recieveMessageHandler, 
	boost::asio::ip::tcp::socket *socket,
	unsigned int destinationId,
	unsigned int ipAddress) :
	socket_(socket), destinationId_(destinationId),
	send_(socket, destinationId, ipAddress, recieveMessageHandler),
	recv_(socket, destinationId, ipAddress, recieveMessageHandler),
	running_(true), ipAddress_(ipAddress)
{
	
}

NetServerTCP3Destination::~NetServerTCP3Destination()
{
}

void NetServerTCP3Destination::printStats()
{
	Logger::log(S3D::formatStringBuffer("TCP3 Destination %u net stats:", 
		destinationId_));
	Logger::log(S3D::formatStringBuffer("  %u messages sent, %u bytes out",
		send_.getMessagesSent(), send_.getBytesOut()));
	Logger::log(S3D::formatStringBuffer("  %u messages recieved, %u bytes in",
		recv_.getMessagesRecieved(), recv_.getBytesIn()));
}

void NetServerTCP3Destination::sendMessage(NetMessage *message) 
{
	send_.sendMessage(message);
}

void NetServerTCP3Destination::close(NetMessage *message)
{
	DIALOG_ASSERT(message->getMessageType() == NetMessage::DisconnectMessage);
	send_.sendMessage(message);
	recv_.stop();
	running_ = false;
}

bool NetServerTCP3Destination::anyFinished()
{
	if (!running_ || send_.getStopped() || recv_.getStopped())
	{
		running_ = false;
		return true;
	}
	return false;
}

bool NetServerTCP3Destination::allFinished()
{
	if (send_.getStopped() && recv_.getStopped())
	{
		send_.wait();
		recv_.wait();
		delete socket_;
		socket_ = 0;
		return true;
	}
	return false;
}

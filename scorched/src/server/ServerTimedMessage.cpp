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

#include <server/ServerTimedMessage.h>
#include <server/ServerCommon.h>
#include <common/Logger.h>
#include <time.h>

ServerTimedMessage *ServerTimedMessage::instance_ = 0;

ServerTimedMessage *ServerTimedMessage::instance()
{
	if (!instance_)
	{
		instance_ = new ServerTimedMessage;
	}
	return instance_;
}

ServerTimedMessage::ServerTimedMessage() : 
	lastTime_(0), timeStep_(0)
{
}

ServerTimedMessage::~ServerTimedMessage()
{
}

void ServerTimedMessage::simulate()
{
	if (message_.empty() || timeStep_ == 0) return;

	unsigned int currentTime = time(0);
	if (currentTime > lastTime_ + timeStep_)
	{
		lastTime_ = currentTime;
		time_t theTime = time(0);
		ServerCommon::sendString(0, message_.c_str(), ctime(&theTime));
		Logger::log(0, message_.c_str(), ctime(&theTime));
	}
}

void ServerTimedMessage::setMessage(const char *message, unsigned int step)
{
	message_ = message;
	timeStep_ = step;

	if (message_.empty() || timeStep_ == 0)
	{
		Logger::log(0, "Timed Message Turned Off");
	}
	else
	{
		Logger::log(0, "Sending message \"%s\" every %i seconds",
			message_.c_str(), timeStep_);
	}
}


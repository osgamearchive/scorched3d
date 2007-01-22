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

#include <coms/ComsSyncCheckMessage.h>
#include <common/Logger.h>
#include <client/ScorchedClient.h>
#include <server/ScorchedServer.h>
#include <target/TargetContainer.h>
#include <target/TargetState.h>
#include <tank/Tank.h>
#include <landscapemap/LandscapeMaps.h>
#include <set>

ComsSyncCheckMessage::ComsSyncCheckMessage() :
	ComsMessage("ComsSyncCheckMessage")
{

}

ComsSyncCheckMessage::~ComsSyncCheckMessage()
{

}

bool ComsSyncCheckMessage::writeMessage(NetBuffer &buffer)
{
	// Send the height map data
	HeightMap &map = ScorchedServer::instance()->getLandscapeMaps().
		getGroundMaps().getHeightMap();
	for (int y=0; y<map.getMapHeight(); y++)
	{
		for (int x=0; x<map.getMapWidth(); x++)
		{
			float height = map.getHeight(x, y);
			buffer.addToBuffer(height);
			Vector &normal = map.getNormal(x, y);
			buffer.addToBuffer(normal);
		}
	}

	// Send the target data
	std::map<unsigned int, Target *> &possibletargets =
			ScorchedServer::instance()->getTargetContainer().getTargets();
	std::map<unsigned int, Target *>::iterator itor;
	buffer.addToBuffer((int) possibletargets.size());
	for (itor = possibletargets.begin();
		itor != possibletargets.end();
		itor++)
	{
		Target *target = (*itor).second;
		buffer.addToBuffer(target->getPlayerId());

		if (target->isTarget())
		{
			if (!target->writeMessage(buffer)) return false;
		}
		else
		{
			if (!((Tank*)target)->writeMessage(buffer, true)) return false;
		}
	}

	return true;
}

bool ComsSyncCheckMessage::readMessage(NetBufferReader &reader)
{
#ifndef S3D_SERVER

	// Read the height map data
	HeightMap &map = ScorchedClient::instance()->getLandscapeMaps().
		getGroundMaps().getHeightMap();
	for (int y=0; y<map.getMapHeight(); y++)
	{
		for (int x=0; x<map.getMapWidth(); x++)
		{
			float actualheight = map.getHeight(x, y);
			Vector actualnormal = map.getNormal(x, y);
			float sentheight;
			Vector sentnormal;
			if (!reader.getFromBuffer(sentheight)) return false;
			if (!reader.getFromBuffer(sentnormal)) return false;
			
			if (actualheight != sentheight)
			{
				Logger::log(formatString("Height diff %i,%i %.2f %.2f", x, y, actualheight, sentheight));
			}
			if (actualnormal != sentnormal)
			{
				Logger::log(formatString("Normal diff %i,%i %f,%f,%f %f,%f,%f", 
					x, y,
					actualnormal[0], actualnormal[1],actualnormal[2],
					sentnormal[0], sentnormal[1],sentnormal[2]));
			}
		}
	}

	// Read the target data
	std::set<unsigned int> readTargets;
	int numberTargetsSend = 0;
	NetBuffer tmpBuffer;

	if (!reader.getFromBuffer(numberTargetsSend)) return false;
	for (int i=0; i<numberTargetsSend; i++)
	{
		unsigned int playerId = 0;
		if (!reader.getFromBuffer(playerId)) return false;
		readTargets.insert(playerId);

		Target *target = ScorchedClient::instance()->getTargetContainer().getTargetById(playerId);
		if (!target)
		{
			Logger::log(formatString("Failed to find a client target : %u", playerId));
			return true;
		}

		tmpBuffer.reset();
		target->writeMessage(tmpBuffer);
		if (memcmp(
			tmpBuffer.getBuffer(), &reader.getBuffer()[reader.getReadSize()], 
			tmpBuffer.getBufferUsed()) != 0)
		{
			if (!target->getTargetState().getMovement())
			{
				Logger::log(formatString("Targets values differ : %u", playerId));
			}
		}

		if (target->isTarget())
		{
			if (!target->readMessage(reader)) return false;
		}
		else
		{
			if (!((Tank*)target)->readMessage(reader)) return false;
		}
	}

#endif // #ifndef S3D_SERVER
	return true;
}

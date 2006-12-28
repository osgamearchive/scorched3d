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
#include <landscapemap/LandscapeMaps.h>

ComsSyncCheckMessage::ComsSyncCheckMessage() :
	ComsMessage("ComsSyncCheckMessage")
{

}

ComsSyncCheckMessage::~ComsSyncCheckMessage()
{

}

bool ComsSyncCheckMessage::writeMessage(NetBuffer &buffer, unsigned int destinationId)
{
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

	return true;
}

bool ComsSyncCheckMessage::readMessage(NetBufferReader &reader)
{
#ifndef S3D_SERVER

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

#endif // #ifndef S3D_SERVER
	return true;
}

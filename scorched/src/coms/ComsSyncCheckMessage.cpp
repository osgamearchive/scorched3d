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

#ifndef S3D_SERVER

#include <GLEXT/GLBitmap.h>
#include <landscape/Landscape.h>

bool ComsSyncCheckMessage::readMessage(NetBufferReader &reader)
{
	// Read the height map data
	HeightMap &map = ScorchedClient::instance()->getLandscapeMaps().
		getGroundMaps().getHeightMap();
	int heightDiffs = 0, normalDiffs = 0;
	bool *heightDiff = new bool[map.getMapHeight() * map.getMapWidth()];
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
			
			if (actualheight != sentheight) heightDiffs++;
			if (actualnormal != sentnormal) normalDiffs++;

			heightDiff[x + y * map.getMapWidth()] = (actualheight != sentheight);
		}
	}
	if (heightDiffs > 0 || normalDiffs > 0)
	{
		Logger::log(formatString("Height diffs %i, Normal diffs %i",
			heightDiffs, normalDiffs));

		GLBitmap newMap(
			Landscape::instance()->getMainMap().getWidth(),
			Landscape::instance()->getMainMap().getHeight());

		GLubyte *dest = newMap.getBits();
		GLubyte *src = Landscape::instance()->getMainMap().getBits();
		for (int y=0; y<newMap.getHeight(); y++)
		{
			for (int x=0; x<newMap.getWidth(); x++)
			{
				GLubyte r = src[0];
				GLubyte g = src[1];
				GLubyte b = src[2];

				int x2 = (x * map.getMapWidth()) / newMap.getWidth();
				int y2 = (y * map.getMapHeight()) / newMap.getHeight();
				if (heightDiff[x2 + y2 * map.getMapWidth()])
				{
					r = g = b = 255;
				}
		
				dest[0] = r;
				dest[1] = g;
				dest[2] = b;

				dest+=3;
				src+=3;
			}
		}
		Landscape::instance()->getMainTexture().replace(newMap, GL_RGB, false);
		Landscape::instance()->setTextureType(Landscape::eOther);
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
		if (target->isTarget())
		{
			target->writeMessage(tmpBuffer);
		}
		else
		{
			((Tank*)target)->writeMessage(tmpBuffer, true);
		}

		if (!target->getTargetState().getMovement())
		{
			for (unsigned int i=0; i<tmpBuffer.getBufferUsed(); i++)
			{
				if (tmpBuffer.getBuffer()[i] != reader.getBuffer()[reader.getReadSize() + i])
				{
					Logger::log(formatString("Targets values differ : %u:%s, position %i", 
						playerId, target->getName(), i));

					// Only used for step-through debugging to see where the
					// differences are
					tmpBuffer.setBufferUsed(i);
					NetBufferReader tmpReader(tmpBuffer);
					if (!target->readMessage(tmpReader))
					{
						Logger::log(formatString("Re-parse failed"));
					}

					break;
				}
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

	return true;
}

#else // #ifndef S3D_SERVER

bool ComsSyncCheckMessage::readMessage(NetBufferReader &reader)
{
	return true;
}

#endif // #ifndef S3D_SERVER

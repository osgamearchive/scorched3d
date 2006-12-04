////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <landscapemap/HeightMapSender.h>
#include <common/Logger.h>
#include <float.h>
#include <stdlib.h>
#include <zlib.h>

bool HeightMapSender::generateHMapDiff(
	HeightMap &hMap, 
	ComsHeightMapMessage &message,
	ProgressCounter *counter)
{
	// Buffer sizes
	unsigned long destLen = (hMap.getMapWidth() + 1) * (hMap.getMapHeight() + 1) * sizeof(float) * 2;
	unsigned long srcLen = (hMap.getMapWidth() + 1) * (hMap.getMapHeight() + 1) * sizeof(float);

	// Storage
	unsigned char *diffMap = new unsigned char[srcLen];
	unsigned char *currentDiff = diffMap;

	// Calculate the differences between the current level
	// and the landscape when the level first started
	// need to change into network byte ordering so 
	// will work on all arches
	unsigned int noDiffs = 0;
	for (int j = 0; j<(hMap.getMapHeight() + 1); j++)
	{
		for (int i = 0; i<(hMap.getMapWidth() + 1); i++)
		{
			float current = hMap.getHeight(i, j);
			float stored = hMap.getBackupHeight(i, j);
			float diff = current - stored;
			if (diff != 0.0f)
			{
				noDiffs ++;
				if (current == 0.0f) diff = FLT_MAX;
			}

			Uint32 value = 0;
			Uint32 add = 0;
			memcpy(&add, &diff, sizeof(Uint32));
			SDLNet_Write32(add, &value);
			memcpy(currentDiff, &value, 4);

			currentDiff += 4; // Move by 4 bytes (32 bits)
		}
	}

	// Compress this information
	unsigned char *compressed = new unsigned char[destLen];
	bool result = 
		(compress2(compressed, &destLen, 
				   (unsigned char *) diffMap, srcLen, 
				   6) == Z_OK);
	if (result)
	{
		// Create the coms message containing the compressed
		// level
		message.createMessage(compressed, destLen);
		//dialogMessage("hmm", "%i=%i %i", srcLen, destLen, noDiffs);
	}
	else
	{
		delete [] compressed;
	}
	
	delete [] diffMap;
	return result;
}

bool HeightMapSender::generateHMapFromDiff(
	HeightMap &hMap, 
	ComsHeightMapMessage &message,
	ProgressCounter *counter)
{
	// Buffer sizes
	unsigned long wantedDestLen = 
		(hMap.getMapWidth() + 1) * 
		(hMap.getMapHeight() + 1) * sizeof(float);
	unsigned long destLen = wantedDestLen * 2;
	
	// Storage
	unsigned char *dest = new unsigned char[destLen];
	
	// Decompress the level information
	unsigned uncompressResult = 
		uncompress(dest, &destLen, message.getLevelData(), message.getLevelLen());

	// Check for error result
	if (uncompressResult == Z_MEM_ERROR) dialogMessage(
		"LandscapeMaps::generateHMapFromDiff", "Memory error");
	else if (uncompressResult == Z_DATA_ERROR) dialogMessage(
		"LandscapeMaps::generateHMapFromDiff", "Data error");
	else if (uncompressResult == Z_BUF_ERROR) dialogMessage(
		"LandscapeMaps::generateHMapFromDiff", "Buffer error");

	bool result = (Z_OK == uncompressResult);
	if (result)
	{
		// Update this level with any changes specified in the diffs
		// need to change from network byte ordering so 
		// will work on all arches
		unsigned char *destCurrent = dest;
		float *current = hMap.getData();		
		for (int j = 0; j<(hMap.getMapHeight() + 1); j++)
		{
			for (int i = 0; i<(hMap.getMapWidth() + 1); i++)
			{
				float diff = 0.0f;
				Uint32 value = 0;
				memcpy(&value, destCurrent, 4);
				Uint32 result = SDLNet_Read32(&value);
				memcpy(&diff, &result, sizeof(Uint32));

				if (diff == FLT_MAX)
				{
					hMap.setHeight(i, j, 0.0f);
				}
				else if (diff != 0.0f)
				{
					float current = hMap.getHeight(i, j);
					hMap.setHeight(i, j, current + diff);
				}
			
				destCurrent += 4; // Move by 4 bytes (32 bits)
			}
		}

		hMap.generateNormals(0, hMap.getMapWidth(), 0, hMap.getMapHeight(), counter);
		//dialogMessage("hmm", "%i=%i", message.getLevelLen(), destLen);
	}

	delete [] dest;
	return result;
}


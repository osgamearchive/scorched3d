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


#include <landscape/GlobalHMap.h>
#include <landscape/HeightMapModifier.h>
#include <common/OptionsGame.h>
#include <common/RandomGenerator.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <zlib/zlib.h>

GlobalHMap *GlobalHMap::instance_ = 0;

GlobalHMap *GlobalHMap::instance()
{
	if (!instance_)
	{
		instance_ = new GlobalHMap;
	}
	return instance_;
}

GlobalHMap::GlobalHMap() : 
	map_(256), mmap_(256), nmap_(256), storedMap_(0), storedSeed_(0)
{
	// Allocate the stored map size to be the same
	// as the height map size
	storedMap_ = new float[(256 + 1) * (256 + 1)];
}

GlobalHMap::~GlobalHMap()
{
	delete [] storedMap_;
}

void GlobalHMap::generateHMap(unsigned long seed,
							  ProgressCounter *counter)
{
	// Seed the generator
	static RandomGenerator generator;
	generator.seed(seed);
	storedSeed_ = seed;

	// Create the new height map
	// based on the parameters in the options
	int maxHeight = OptionsGame::instance()->getMaxHeight();
	int noHills = OptionsGame::instance()->getNoHills();

	Logger::log(0, "Landscape : %i %i %i", storedSeed_, maxHeight, noHills);
	HeightMapModifier::generateTerrain(getHMap(), 
		noHills, maxHeight, generator, counter);

	// Save this height map for later
	memcpy(storedMap_, map_.getData(), 
		   (256 + 1) * (256 + 1) * sizeof(float));
}

bool GlobalHMap::generateHMapDiff(ComsLevelMessage &message)
{
	// Buffer sizes
	unsigned long destLen = (256 + 1) * (256 + 1) * sizeof(float) * 2;
	unsigned long srcLen = (256 + 1) * (256 + 1) * sizeof(float);

	// Storage
	unsigned char *diffMap = new unsigned char[srcLen];
	unsigned char *currentDiff = diffMap;
	float *currentStored = storedMap_;
	float *current = map_.getData();

	// Calculate the differences between the current level
	// and the landscape when the level first started
	// need to change into network byte ordering so 
	// will work on all arches
	unsigned int noDiffs = 0;
	for (int i = 0; i<(256 + 1) * (256 + 1); 
		 i++, current++, currentStored++)
	{
		float diff = ((*current) - (*currentStored));
		if (diff != 0.0f) noDiffs ++;

		Uint32 value = 0;
		Uint32 add = 0;
		memcpy(&add, &diff, sizeof(Uint32));
		SDLNet_Write32(add, &value);
		memcpy(currentDiff, &value, 4);

		currentDiff += 4; // Move by 4 bytes (32 bits)
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
		message.createMessage(storedSeed_,
							  compressed,
							  destLen);
		//dialogMessage("hmm", "%i=%i %i", srcLen, destLen, noDiffs);
	}
	else
	{
		delete [] compressed;
	}
	
	delete [] diffMap;
	return result;
}

bool GlobalHMap::generateHMapFromDiff(ComsLevelMessage &message,
									  ProgressCounter *counter)
{
	// Buffer sizes
	unsigned long wantedDestLen = (256 + 1) * (256 + 1) * sizeof(float);
	unsigned long destLen = wantedDestLen * 2;
	
	// Storage
	unsigned char *dest = new unsigned char[destLen];
	
	// Decompress the level information
	unsigned uncompressResult = 
		uncompress(dest, &destLen, message.getLevelData(), message.getLevelLen());

	// Check for error result
	if (uncompressResult == Z_MEM_ERROR) dialogMessage(
		"GlobalHMap::generateHMapFromDiff", "Memory error");
	else if (uncompressResult == Z_DATA_ERROR) dialogMessage(
		"GlobalHMap::generateHMapFromDiff", "Data error");
	else if (uncompressResult == Z_BUF_ERROR) dialogMessage(
		"GlobalHMap::generateHMapFromDiff", "Buffer error");

	bool result = (Z_OK == uncompressResult);
	if (result)
	{
		// Create the new base Level using the seed
		generateHMap(message.getSeed(), counter);

		// Update this level with any changes specified in the diffs
		// need to change from network byte ordering so 
		// will work on all arches
		unsigned char *destCurrent = dest;
		float *current = map_.getData();		
		for (int i = 0; i<(256 + 1) * (256 + 1); 
			 i++, current++)
		{
			float diff = 0.0f;
			Uint32 value = 0;
			memcpy(&value, destCurrent, 4);
			Uint32 result = SDLNet_Read32(&value);
			memcpy(&diff, &result, sizeof(Uint32));

			(*current) += diff;
			
			destCurrent += 4; // Move by 4 bytes (32 bits)
		}

		//dialogMessage("hmm", "%i=%i", message.getLevelLen(), destLen);
	}

	delete [] dest;
	return result;
}


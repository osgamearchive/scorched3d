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

#include <common/RandomGenerator.h>
#include <common/Defines.h>
#include <SDL/SDL_net.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

// Yes you guessed it, a really sucky way to create cross platform
// random numbers.  Store them in a file and read them in each system!

unsigned long RandomGenerator::bufferSize_ = 0;
unsigned long *RandomGenerator::buffer_ = 0;

RandomGenerator::RandomGenerator() :
	position_(0)
{
	// Cache the buffer so we only read it once
	// We can create many random generators easily now
	if (!buffer_)
	{
		FILE *in = fopen(getDataFile("data/random.no"), "rb");
		DIALOG_ASSERT(in);
		bufferSize_= 100000;
		unsigned long *tmpbuffer = new unsigned long[bufferSize_];
		int size = fread(tmpbuffer, sizeof(unsigned long), bufferSize_, in);
		fclose(in);	
		DIALOG_ASSERT(size == bufferSize_);

		buffer_ = new unsigned long[bufferSize_];
		for (unsigned long i=0; i<bufferSize_; i++)
		{
			unsigned long value = tmpbuffer[i];
			buffer_[i] = SDLNet_Read32(&value);
		}
		delete [] tmpbuffer;
	}
}

RandomGenerator::~RandomGenerator()
{
}

void RandomGenerator::seed(unsigned long seed)
{
	position_ = seed;
}

unsigned long RandomGenerator::getRandLong()
{
	unsigned long pos = position_ % bufferSize_;
	position_++;
	return buffer_[pos];
}

float RandomGenerator::getRandFloat()
{
	unsigned long y = getRandLong();
	return float(y) / float(ULONG_MAX);
}

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


#include <common/SoundStore.h>

SoundStore *SoundStore::instance_ = 0;

SoundStore *SoundStore::instance()
{
	if (!instance_)
	{
		instance_ = new SoundStore;
	}
	return instance_;
}

SoundStore::SoundStore()
{
}

SoundStore::~SoundStore()
{
}

SoundBuffer *SoundStore::fetchOrCreateBuffer(char *fn)
{
	std::string filename(fn);
	BufferMap::iterator itor = bufferMap_.find(filename);
	if (itor != bufferMap_.end())
	{
		return (*itor).second;
	}

	SoundBuffer *buffer = Sound::instance()->createBuffer(fn);
	if (!buffer)
	{
		dialogMessage("Failed to load sound",
				"\"%s\"", fn);
	}

	bufferMap_[filename] = buffer;
	return buffer;
}

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

#include <tank/TankAvatar.h>
#include <GLEXT/GLTexture.h>
#include <GLEXT/GLGif.h>
#include <common/Defines.h>
#include <zlib/zlib.h>
#include <stdio.h>

GLTexture *TankAvatar::defaultTexture_ = 0;
std::list<TankAvatar::AvatarStore> TankAvatar::storeEntries_;

TankAvatar::TankAvatar() : texture_(0)
{
	file_ = new NetBuffer();
}

TankAvatar::~TankAvatar()
{
	delete file_;
}

bool TankAvatar::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(name_);
	buffer.addToBuffer(file_->getBufferUsed());
	if (file_->getBufferUsed() > 0)
	{
		buffer.addDataToBuffer(file_->getBuffer(),
			file_->getBufferUsed());
	}
	return true;
}

bool TankAvatar::readMessage(NetBufferReader &reader)
{
	unsigned int used = 0;
	if (!reader.getFromBuffer(name_)) return false;
	if (!reader.getFromBuffer(used)) return false;
	if (used > 0)
	{
		file_->allocate(used);
		file_->reset();
		file_->setBufferUsed(used);
		reader.getDataFromBuffer(file_->getBuffer(),
			used);
	}
	return true;
}

bool TankAvatar::loadFromFile(const char *fileName)
{
	FILE *in = fopen(
		getDataFile("data/avatars/%s", fileName),
		"rb");
	if (in)
	{
		name_ = fileName;
		file_->reset();
		unsigned char readBuf[512];
		while (unsigned int size = fread(readBuf, sizeof(unsigned char), 512, in))
		{
			file_->addDataToBuffer(readBuf, size);
		}
		fclose(in);
		return true;
	}
	return false;
}

bool TankAvatar::setFromBuffer(const char *fileName,
	NetBuffer &buffer, bool createTexture)
{
	GLGif gif;
	if (!gif.loadFromBuffer(buffer)) return false;
	if (gif.getWidth() != 32 || 
		gif.getHeight() != 32) return false;

	name_ = fileName;
	file_->reset();
	file_->addDataToBuffer(buffer.getBuffer(), 
		buffer.getBufferUsed());

	if (createTexture)
	{
		texture_ = 0;
		unsigned int crc = getCrc();
		std::list<AvatarStore>::iterator itor;
		for (itor = storeEntries_.begin();
			itor != storeEntries_.end();
			itor++)
		{
			AvatarStore &store = (*itor);
			if (store.crc_ == crc &&
				0 == strcmp(store.name_.c_str(), name_.c_str()))
			{
				texture_ = store.texture_;
				break;
			}
		}
		if (!texture_)
		{
			texture_ = new GLTexture;
			texture_->create(gif);
			AvatarStore store;
			store.crc_ = crc;
			store.name_ = name_;
			store.texture_ = texture_;
			storeEntries_.push_back(store);
		}
	}

	return true;
}

GLTexture *TankAvatar::getTexture()
{
	if (!texture_)
	{
		if (!defaultTexture_)
		{
			defaultTexture_ = new GLTexture();
			GLGif gif;
			gif.loadFromFile(
				getDataFile("data/avatars/player.gif"));
			defaultTexture_->create(gif);
		}
		return defaultTexture_;
	}

	return texture_; 
}

unsigned int TankAvatar::getCrc()
{
	unsigned int crc =  crc32(0L, Z_NULL, 0);
	crc = crc32(crc, (unsigned char *) 
		file_->getBuffer(), file_->getBufferUsed());	
	return crc;
}

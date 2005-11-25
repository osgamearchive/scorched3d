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

#ifndef _NETBUFFER_H_
#define _NETBUFFER_H_

#include <string>
#include <common/Vector.h>
#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

class NetBuffer
{
public:
	NetBuffer();
	NetBuffer(unsigned startSize, void *startBuffer = 0);
	NetBuffer(const NetBuffer &other);
	virtual ~NetBuffer();

	void reset();
	void clear();
	void resize(unsigned newBufferSize);
	void allocate(unsigned size);
	void setBufferUsed(unsigned size) { usedSize_ = size; }

	void addToBuffer(Vector &vector);
	void addToBuffer(const char *add);
	void addToBuffer(std::string &string);
	void addToBuffer(const int add);
	void addToBuffer(const bool add);
	void addToBuffer(const unsigned int add);
	void addToBuffer(const float add);

	char *getBuffer() { return buffer_; }
	unsigned getTotalBufferSize() { return bufferSize_; }
	unsigned getBufferUsed() { return usedSize_; }

	// Adds raw data to into the buffer
	// NOTE: Care must be taken to ensure that the added data
	// is in network byte ordering
	void addDataToBuffer(const void *add, unsigned len);

protected:
	char *buffer_;
	unsigned usedSize_;
	unsigned bufferSize_;

};

class NetBufferDefault
{
public:
	static NetBuffer defaultBuffer;

private:
	NetBufferDefault();
};

class NetBufferReader
{
public:
	NetBufferReader(NetBuffer &buffer);
	virtual ~NetBufferReader();

	void reset();

	unsigned getBufferSize() { return bufferSize_; }
	unsigned getReadSize() { return readSize_; }
	char *getBuffer() { return buffer_; }
	void setReadSize(unsigned size) { readSize_ = size; }
	void setBufferSize(unsigned size) { bufferSize_ = size; }

	bool getFromBuffer(Vector &result);
	bool getFromBuffer(int &result);
	bool getFromBuffer(float &result);
	bool getFromBuffer(bool &result);
	bool getFromBuffer(unsigned int &result);
	bool getFromBuffer(std::string &string, bool safe = true);

	// Gets raw data from the buffer
	// NOTE: Care must be taken to ensure that the data
	// is in network byte ordering	
	bool getDataFromBuffer(void *dest, int len);

protected:
	char *buffer_;
	unsigned bufferSize_;
	unsigned readSize_;

};

#endif /* _NETBUFFER_H_ */

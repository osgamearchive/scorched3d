#pragma once

#include <sound/SoundBuffer.h>

class SoundBufferFactory
{
public:
	static SoundBuffer *createBuffer(const char *fileName);

private:
	SoundBufferFactory();
	virtual ~SoundBufferFactory();
};

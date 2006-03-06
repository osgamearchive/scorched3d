#include <sound/Sound.h>
#include <sound/SoundBufferFactory.h>
#include <sound/SoundBufferEmpty.h>
#include <sound/SoundBufferStaticWav.h>

SoundBuffer *SoundBufferFactory::createBuffer(const char *fileName)
{
	if (!Sound::instance()->getInit())
	{
		// If sound is not init return an empty buffer
		// This will happen if the user turns sound off
		return new SoundBufferEmpty();
	}

	SoundBufferStaticWav *buffer = new SoundBufferStaticWav();
	if (!buffer->createBuffer(fileName))
	{
		delete buffer;
		buffer = 0;
	}
	return buffer;
}

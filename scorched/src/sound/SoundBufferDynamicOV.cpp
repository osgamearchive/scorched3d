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

#include <sound/SoundBufferDynamicOV.h>
#include <common/Defines.h>
#include <common/Logger.h>

#ifdef HAVE_OGG

#define BUFFER_SIZE (1024 * 1024)

SoundBufferDynamicOVSourceInstance::SoundBufferDynamicOVSourceInstance(
	unsigned int source, const char *fileName) :
	SoundBufferSourceInstance(source)
{
	ov_callbacks callbacks = {
		read_func,
		seek_func,
		close_func,
		tell_func 
	};

	FILE *oggFile = fopen(fileName, "rb");
	if (!oggFile)
	{
		dialogMessage("OGG Vorbis",
			formatString("Could not open ogg file \"%s\"",
			fileName));
		return;
	}

	int result = ov_open_callbacks((void *) oggFile, &oggStream_, 0, 0, callbacks);
	if(result < 0)
	{        
		fclose(oggFile);
		dialogMessage("OGG Vorbis",
			formatString("Could not open ogg stream \"%s\" : %s",
			fileName, errorString(result)));
		return; 
	}

	vorbisInfo_ = ov_info(&oggStream_, -1);
    if(vorbisInfo_->channels == 1) format_ = AL_FORMAT_MONO16;
	else format_ = AL_FORMAT_STEREO16;

	alGenBuffers(2, buffers_);
	if (alGetError() != AL_NO_ERROR) return;
}

SoundBufferDynamicOVSourceInstance::~SoundBufferDynamicOVSourceInstance()
{
	if (buffers_[0]) alDeleteBuffers(2, buffers_);
	ov_clear(&oggStream_);
}

void SoundBufferDynamicOVSourceInstance::play(bool repeat)
{
	ov_raw_seek(&oggStream_, 0);
	
	if (!addDataToBuffer(buffers_[0], repeat)) return;
	if (!addDataToBuffer(buffers_[1], repeat)) return;

	alSourcei(source_, AL_BUFFER, 0); // Reset the source
	alSourcei(source_, AL_LOOPING, AL_FALSE);

	int error = 0;
    alSourceQueueBuffers(source_, 2, buffers_);
	if ((error = alGetError()) != AL_NO_ERROR) return;
	alSourcePlay(source_);
	if ((error = alGetError()) != AL_NO_ERROR) return;
}

void SoundBufferDynamicOVSourceInstance::stop()
{
    int queued = 2;
	while(queued--)
	{        
		ALuint buffer;            
		alSourceUnqueueBuffers(source_, 1, &buffer);
	}
	alSourceStop(source_);
}

void SoundBufferDynamicOVSourceInstance::simulate(bool repeat)
{
    int processed = 0;
	alGetSourcei(source_, AL_BUFFERS_PROCESSED, &processed);
	if (processed == 2)
	{
		Logger::log("Warning: OGG falling behind");
	}
	while(processed--)
	{
		//Logger::log(formatString("Processed %u %i", source_, processed));

		ALuint buffer;                
		alSourceUnqueueBuffers(source_, 1, &buffer);
		if (addDataToBuffer(buffer, repeat))
		{
			alSourceQueueBuffers(source_, 1, &buffer);
		}
	}
}

bool SoundBufferDynamicOVSourceInstance::addDataToBuffer(unsigned int buffer, bool loop)
{
    static char data[BUFFER_SIZE];    
	int size = 0;    
	int section;    
	int result;     
	while(size < BUFFER_SIZE)    
	{        
		result = ov_read(&oggStream_, data + size, 
			BUFFER_SIZE - size, 0, 2, 1, &section);            
		if (result > 0) size += result;        
		else if (result < 0) return false; // Error
		else break;    
	}
	if(size == 0)
	{
		if (!loop) return false;
		else
		{
			ov_raw_seek(&oggStream_, 0);
			return addDataToBuffer(buffer, false);
		}
	}

	alBufferData(buffer, format_, data, size, vorbisInfo_->rate);    
	return true;
}

const char *SoundBufferDynamicOVSourceInstance::errorString(int code)
{    
	switch(code)    
	{        
	case OV_EREAD:            
		return ("Read from media.");        
	case OV_ENOTVORBIS:            
		return ("Not Vorbis data.");        
	case OV_EVERSION:            
		return ("Vorbis version mismatch.");        
	case OV_EBADHEADER:            
		return ("Invalid Vorbis header.");        
	case OV_EFAULT:            
		return ("Internal logic fault (bug or heap/stack corruption.");        
	default:            
		return ("Unknown Ogg error.");    
	}
}

size_t SoundBufferDynamicOVSourceInstance::read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	return fread(ptr, size, nmemb, (FILE *) datasource);
}

int SoundBufferDynamicOVSourceInstance::seek_func(void *datasource, ogg_int64_t offset, int whence)
{
	return fseek((FILE*) datasource, (long) offset, whence);
}

int SoundBufferDynamicOVSourceInstance::close_func(void *datasource)
{
	return fclose((FILE *) datasource);
}

long SoundBufferDynamicOVSourceInstance::tell_func(void *datasource)
{
	return ftell((FILE *) datasource);
}

SoundBufferDynamicOV::SoundBufferDynamicOV(const char *fileName) :
	SoundBuffer(fileName)
{
}

SoundBufferDynamicOV::~SoundBufferDynamicOV()
{
}

SoundBufferSourceInstance *SoundBufferDynamicOV::createSourceInstance(unsigned int source)
{
	return new SoundBufferDynamicOVSourceInstance(source, fileName_.c_str());
}

#endif // HAVE_OGG


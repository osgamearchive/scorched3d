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

#ifndef _SOUND_H_
#define _SOUND_H_

#include <map>
#include <vector>
#include <string>
#include <sound/VirtualSoundSource.h>
#include <sound/SoundBuffer.h>
#include <sound/SoundListener.h>

#define CACHE_SOUND(var, filename) 										\
		static SoundBuffer* var = Sound::instance()->fetchOrCreateBuffer(filename);

class Sound
{
public:
	static Sound *instance();

	bool init(int channels);
	bool getInit() { return init_; }
	void destroy();

	void showSoundBuffers();

	SoundBuffer *fetchOrCreateBuffer(char *filename);
	SoundListener *getDefaultListener();
	
	void addManaged(VirtualSoundSource *source);
	void removePlaying(VirtualSoundSource *source, unsigned int priority);
	SoundSource *addPlaying(VirtualSoundSource *source, unsigned int priority);

	void simulate(float frameTime);
	int getAvailableChannels();
	int getPlayingChannels();

protected:
	static Sound *instance_;
	typedef std::map<std::string, SoundBuffer *> BufferMap;
	typedef std::vector<SoundSource *> SourceList;
	typedef std::vector<VirtualSoundSource *> VirtualSourceList;
	typedef std::multimap<unsigned int, VirtualSoundSource *> VirtualSourceMap;
	float totalTime_;
	BufferMap bufferMap_;
	SourceList totalSources_;
	SourceList availableSources_;
	SoundListener listener_;
	VirtualSourceMap playingSources_;
	VirtualSourceList managedSources_;
	VirtualSourceList loopingSources_;
	bool init_;

	void tidyBuffers();
	SoundBuffer *createBuffer(char *fileName);

private:
	Sound();
	virtual ~Sound();

};

#endif /* _SOUND_H_ */

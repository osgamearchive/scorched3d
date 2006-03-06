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

#if !defined(__INCLUDE_VirtualSoundSourceh_INCLUDE__)
#define __INCLUDE_VirtualSoundSourceh_INCLUDE__

#include <sound/SoundSource.h>
#include <sound/VirtualSoundPriority.h>

class VirtualSoundSource
{
public:
	VirtualSoundSource(
		unsigned int priority, bool looping, bool managed);
	virtual ~VirtualSoundSource();

	void play(SoundBuffer *buffer);
	void simulate();
	void stop();

	bool getPlaying();
	bool getManaged();
	bool getLooping();
	unsigned int getPriority();
	SoundBuffer *getBuffer();
	SoundSource *getSource();

	void setRelative();
	void setPosition(Vector &position);
	void setVelocity(Vector &velocity);
	void setGain(float gain);

protected:
	unsigned int priority_;
	SoundSource *actualSource_;
	SoundBuffer *buffer_;
	Vector position_, velocity_;
	float gain_;
	bool relative_;
	bool looping_;
	bool managed_;
};

#endif // __INCLUDE_VirtualSoundSourceh_INCLUDE__

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

#include <landscape/LandscapeSoundManager.h>
#include <landscape/LandscapeSound.h>
#include <common/OptionsDisplay.h>
#include <sound/Sound.h>

LandscapeSoundManager::LandscapeSoundManager() : lastTime_(0)
{
}

LandscapeSoundManager::~LandscapeSoundManager()
{
}

void LandscapeSoundManager::cleanUp()
{
	std::list<LandscapeSoundManagerEntry>::iterator itor;
	for (itor = entries_.begin();
		itor != entries_.end();
		itor++)
	{
		LandscapeSoundManagerEntry &entry = (*itor);
		delete entry.soundSource;
		entry.soundSource = 0;
	}
	entries_.clear();
}

void LandscapeSoundManager::initialize(std::list<LandscapeSound *> sounds)
{
	cleanUp();

	if (OptionsDisplay::instance()->getNoAmbientSound()) return;

	std::list<LandscapeSound *>::iterator itor;
	for (itor = sounds.begin();
		itor != sounds.end();
		itor++)
	{
		LandscapeSound *sound = (*itor);

		std::vector<LandscapeSoundType *>::iterator typeItor;
		for (typeItor = sound->objects.begin();
			typeItor != sound->objects.end();
			typeItor ++)
		{
			LandscapeSoundManagerEntry entry;
			entry.soundType = (*typeItor);
			entry.soundBuffer = 
				Sound::instance()->fetchOrCreateBuffer((char *)
					getDataFile(entry.soundType->sound.c_str()));
			entry.timeLeft = entry.soundType->timing->getNextEventTime();
			entry.soundSource = new VirtualSoundSource(
				VirtualSoundPriority::eEnvironment, (entry.timeLeft < 0.0f), false);
			entries_.push_back(entry);

			// Start any looped sounds
			if (entry.timeLeft < 0.0f)
			{
				if (entry.soundType->position->setPosition(entry.soundSource))
				{
					entry.soundSource->play(entry.soundBuffer);
				}
			}
		}
	}
}

void LandscapeSoundManager::simulate(float frameTime)
{
	lastTime_ += frameTime;
	if (lastTime_ < 0.1f) return;

	std::list<LandscapeSoundManagerEntry>::iterator itor;
	for (itor = entries_.begin();
		itor != entries_.end();
		itor++)
	{
		LandscapeSoundManagerEntry &entry = (*itor);
		if (entry.timeLeft >= 0.0f) // Not looped
		{
			entry.timeLeft -= lastTime_;

			if (entry.soundType->position->setPosition(entry.soundSource))
			{
				if (entry.timeLeft < 0.0f)
				{
					entry.timeLeft = entry.soundType->timing->getNextEventTime();
					entry.soundSource->play(entry.soundBuffer);
				}
			}
		}
		else
		{
			if (!entry.soundType->position->setPosition(entry.soundSource))
			{
				if (entry.soundSource->getPlaying())
				{
					entry.soundSource->stop();
				}
			}
		}
	}

	lastTime_ = 0.0f;
}

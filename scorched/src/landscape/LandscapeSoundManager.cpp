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
#include <landscapedef/LandscapeSound.h>
#include <graph/OptionsDisplay.h>
#include <common/Defines.h>
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
			LandscapeSoundType *soundType = (*typeItor);
			for (int i=0; i<soundType->position->getInitCount(); i++)
			{
				// Create the new sound entry
				LandscapeSoundManagerEntry entry;
				entry.soundType = soundType;
				entry.initData = soundType->position->getInitData(i);
				entry.timeLeft = soundType->timing->getNextEventTime();
				entry.soundSource = new VirtualSoundSource(
					VirtualSoundPriority::eEnvironment, 
					(entry.timeLeft < 0.0f), // Looping
					false);
				entries_.push_back(entry);

				// Start any looped sounds
				if (entry.timeLeft < 0.0f)
				{
					if (entry.soundType->position->setPosition(
						entry.soundSource, entry.initData))
					{
						entry.soundType->sound->play(entry.soundSource);
					}
					else
					{
						entry.removed = true;
					}
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

		// Check if this entry is still relevant
		if (entry.removed) continue;

		// Set this sounds position
		if (!entry.soundType->position->setPosition(
			entry.soundSource, entry.initData))
		{
			// The position set failed, stop this sound and remove it
			entry.soundSource->stop();
			entry.removed = true;
		}
		else
		{
			// Check if looped
			if (entry.timeLeft >= 0.0f) 
			{
				// Not looped
				// Check if we play again
				entry.timeLeft -= lastTime_;
				if (entry.timeLeft < 0.0f)
				{
					// Play again
					entry.timeLeft = entry.soundType->timing->getNextEventTime();
					entry.soundType->sound->play(entry.soundSource);
				}
			}
		}
	}

	lastTime_ = 0.0f;
}

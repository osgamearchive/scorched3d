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

#include <landscape/LandscapeMusicManager.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeDefn.h>
#include <landscapemap/LandscapeMaps.h>
#include <graph/OptionsDisplay.h>
#include <client/ScorchedClient.h>
#include <client/ClientState.h>
#include <common/Defines.h>
#include <sound/Sound.h>
#include <XML/XMLFile.h>

LandscapeMusicManager *LandscapeMusicManager::instance()
{
	static LandscapeMusicManager instance_;
	return &instance_;
}

LandscapeMusicManager::LandscapeMusicManager() : 
	GameStateI("LandscapeMusicManager"),
	currentSource_(0), currentGain_(0.0f)
{
	addMusics();
}

LandscapeMusicManager::~LandscapeMusicManager()
{
}

void LandscapeMusicManager::addMusics()
{
	removeAllMusic();
	
	// Read in the global music defaults
	// Do this first so that levels can override the music if required
	readGlobalMusicFile();
	addMusicTypes(globalMusics_);

	// Load in the per level music
	LandscapeDefn *defn = 
		ScorchedClient::instance()->getLandscapeMaps().getDefinitions().getDefn();
	LandscapeTex *tex = 
		ScorchedClient::instance()->getLandscapeMaps().getDefinitions().getTex();

	// Since we initialize before the landscapes are loaded these may be null
	if (tex) addMusic(tex->texDefn.includes);
	if (defn) addMusic(defn->texDefn.includes);
}

void LandscapeMusicManager::readGlobalMusicFile()
{
	const char *filePath = "data/music.xml";

	// Parse the XML file
	XMLFile file;
	if (!file.readFile(filePath))
	{
		dialogExit("Scorched3D", formatString(
			"ERROR: Failed to parse music file \"%s\"\n"
			"%s",
			filePath,
			file.getParserError()));
	}

	// Clear the last set of globalmusics
	globalMusics_.clear();

	// just return for an empty file
	if (!file.getRootNode()) return;

	XMLNode *musicNode = 0;
	while (file.getRootNode()->getNamedChild("music", musicNode, false))
	{
		LandscapeMusicType *musicType = new LandscapeMusicType();
		if (!musicType->readXML(musicNode))
		{
			dialogExit("Scorched3D", formatString(
				"ERROR: Failed to parse music file \"%s\"\n",
				filePath));
		}
		globalMusics_.push_back(musicType);
	}
}

void LandscapeMusicManager::addMusic(std::vector<LandscapeInclude *> &musics)
{
	std::vector<LandscapeInclude *>::iterator includeitor;
	for (includeitor = musics.begin();
		includeitor != musics.end(); 
		includeitor++)
	{
		LandscapeInclude *include = (*includeitor);
		addMusicTypes(include->musics);
	}
}

void LandscapeMusicManager::addMusicTypes(std::vector<LandscapeMusicType *> &musics)
{
	std::vector<LandscapeMusicType *>::iterator itor;
	for (itor = musics.begin();
		itor != musics.end();
		itor++)
	{
		LandscapeMusicType *music = (*itor);
		std::vector<LandscapeMusicType::PlayState>::iterator stateitor;
		for (stateitor = music->playstates.begin();
			stateitor != music->playstates.end();
			stateitor++)
		{
			LandscapeMusicType::PlayState playState = (*stateitor);
			stateMusic_[playState] = music;
		}
	}
}

void LandscapeMusicManager::removeAllMusic()
{
	stateMusic_.clear();
}

void LandscapeMusicManager::simulate(const unsigned state, float simTime)
{
	// Find the playstate that matches the current client state
	LandscapeMusicType::PlayState playState = LandscapeMusicType::StateNone;
	switch (state)
	{
	case ClientState::StateLoadFiles:
	case ClientState::StateGetPlayers:
	case ClientState::StateLoadPlayers:
	case ClientState::StateLoadLevel:
		playState = LandscapeMusicType::StateLoading;
		break;
	case ClientState::StateWait:
		break;
	case ClientState::StateBuyWeapons:
	case ClientState::StateAutoDefense:
		playState = LandscapeMusicType::StateBuying;
		break;
	case ClientState::StatePlaying:
		playState = LandscapeMusicType::StatePlaying;
		break;
	case ClientState::StateShot:
		playState = LandscapeMusicType::StateShot;
		break;
	case ClientState::StateScore:
		playState = LandscapeMusicType::StateScore;
		break;
	}

	// Find which music entry we should be playing in this state
	std::string wantedMusicFile;
	float wantedGain = 0.0f;
	if (!OptionsDisplay::instance()->getNoMusic())
	{
		std::map<LandscapeMusicType::PlayState, LandscapeMusicType *>::iterator findItor =
			stateMusic_.find(playState);
		if (findItor != stateMusic_.end())
		{
			wantedMusicFile = (*findItor).second->file;	
			wantedGain = (*findItor).second->gain *
				float(OptionsDisplay::instance()->getMusicVolume()) / 128.0f;
		}
	}

	// Check if this is different from the currently playing music
	// If its the same music then there is nothing to do
	if (currentMusicFile_ == wantedMusicFile)
	{
		// If we are playing the correct music, check that the
		// current volume is correct
		if (currentGain_ != wantedGain)
		{
			currentGain_ = wantedGain;
			if (currentSource_) currentSource_->setGain(wantedGain);
		}
		return;
	}

	// Stop the currently playing sound source (if any)
	// Since the source is managed it will be automatically deleted once it has stopped
	if (currentSource_)
	{
		currentSource_->stop();
	}

	// Update the state
	currentSource_ = 0;
	currentGain_ = wantedGain;
	currentMusicFile_ = wantedMusicFile;

	// Check if this state wants any music
	// If there is no music to play then there is nothing to do
	if (currentMusicFile_.empty()) return;

	// Play the next set of music
	// Load the next sound buffer
	SoundBuffer *buffer = 
		Sound::instance()->fetchOrCreateBuffer((char *)
			getDataFile(currentMusicFile_.c_str()));
	if (!buffer) return;

	// Start a new sound source
	currentSource_ = new VirtualSoundSource(
		VirtualSoundPriority::eMusic, // A high music priority
		true, // Its a looping sound
		true); // Its automatically deleted when finished
	currentSource_->setGain(currentGain_);
	currentSource_->setRelative();
	currentSource_->play(buffer);
}

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

#if !defined(__INCLUDE_ScorchedClienth_INCLUDE__)
#define __INCLUDE_ScorchedClienth_INCLUDE__

#include <engine/ScorchedContext.h>

class MainLoop;
class ParticleEngine;
class ScorchedClient
{
public:
	static ScorchedClient *instance();

	AccessoryStore &getAccessoryStore() { return *context_.accessoryStore; }
	MainLoop &getMainLoop() { return *mainLoop_; }
	GameState &getGameState() { return *context_.gameState; }
	TankContainer &getTankContainer() { return *context_.tankContainer; }
	ActionController &getActionController() { return *context_.actionController; }
	LandscapeMaps &getLandscapeMaps() { return *context_.landscapeMaps; }
	ScorchedContext &getContext() { return context_; }
	NetInterface &getNetInterface() { return *context_.netInterface; }
	OptionsGameWrapper &getOptionsGame() { return *context_.optionsGame; }
	OptionsTransient &getOptionsTransient() { return *context_.optionsTransient; }
	ComsMessageHandler &getComsMessageHandler() { return *context_.comsMessageHandler; }
	ModFiles &getModFiles() { return *context_.modFiles; }
	ParticleEngine &getParticleEngine() { return *particleEngine_; }

protected:
	static ScorchedClient *instance_;
	MainLoop *mainLoop_;
	ParticleEngine* particleEngine_;
	ScorchedContext context_;

private:
	ScorchedClient();
	virtual ~ScorchedClient();
};

#endif

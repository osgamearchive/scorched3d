#pragma once

#include <engine/GameState.h>
#include <engine/MainLoop.h>

class ScorchedClient
{
public:
	static ScorchedClient *instance();

	MainLoop &getMainLoop() { return mainLoop_; }
	GameState &getGameState() { return gameState_; }

protected:
	static ScorchedClient *instance_;
	MainLoop mainLoop_;
	GameState gameState_;

private:
	ScorchedClient();
	virtual ~ScorchedClient();
};

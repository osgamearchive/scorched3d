#pragma once

#include <engine/GameState.h>

class ScorchedServer
{
public:
	static ScorchedServer *instance();

	GameState &getGameState() { return gameState_; }

protected:
	static ScorchedServer *instance_;
	GameState gameState_;

private:
	ScorchedServer();
	virtual ~ScorchedServer();
};

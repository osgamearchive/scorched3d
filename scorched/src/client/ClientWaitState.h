#pragma once

#include <engine/GameStateStimulusI.h>
#include <engine/GameStateI.h>

class ClientWaitState : public GameStateI
{
public:
	static ClientWaitState *instance();

	virtual void enterState(const unsigned state);

protected:
	static ClientWaitState *instance_;

private:
	ClientWaitState();
	virtual ~ClientWaitState();
};

#include <client/ClientWaitState.h>

ClientWaitState *ClientWaitState::instance_ = 0;

ClientWaitState *ClientWaitState::instance()
{
	if (!instance_)
	{
		instance_ = new ClientWaitState;
	}
	return instance_;
}

ClientWaitState::ClientWaitState()
{
}

ClientWaitState::~ClientWaitState()
{
}

void ClientWaitState::enterState(const unsigned state)
{
	
}

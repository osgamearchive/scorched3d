#pragma once

#include <coms/ComsMessageHandler.h>

class ClientNextRoundHandler : 
	public ComsMessageHandlerI
{
public:
	static ClientNextRoundHandler *instance();

	virtual bool processMessage(unsigned int id,
		const char *messageType,
		NetBufferReader &reader);

protected:
	static ClientNextRoundHandler *instance_;

private:
	ClientNextRoundHandler();
	virtual ~ClientNextRoundHandler();
};

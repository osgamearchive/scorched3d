#pragma once

#include <coms/ComsMessage.h>

class ComsNextRoundMessage : public ComsMessage
{
public:
	ComsNextRoundMessage();
	virtual ~ComsNextRoundMessage();

	// Inherited from ComsMessage
    virtual bool writeMessage(NetBuffer &buffer);
    virtual bool readMessage(NetBufferReader &reader);
};

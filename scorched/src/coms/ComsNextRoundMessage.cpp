#include <coms/ComsNextRoundMessage.h>

ComsNextRoundMessage::ComsNextRoundMessage() : ComsMessage("ComsNextRoundMessage")
{
}

ComsNextRoundMessage::~ComsNextRoundMessage()
{
}

bool ComsNextRoundMessage::writeMessage(NetBuffer &buffer)
{
	return true;
}

bool ComsNextRoundMessage::readMessage(NetBufferReader &reader)
{
	return true;
}

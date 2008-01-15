#include <net/NetInterface.h>
#include <net/NetServerTCP2.h>
#include <net/NetBufferPool.h>
#include <common/Logger.h>
#include <common/DefinesString.h>
#include <SDL/SDL.h>
#include <set>

std::set<unsigned int> currentDestinations;

class TestNetMessageHandler : public NetMessageHandlerI
{
public:
	virtual void processMessage(NetMessage &message)
	{
		switch (message.getMessageType())
		{
		case NetMessage::BufferMessage:
			printf("Recieved %u ", message.getDestinationId());
			break;
		case NetMessage::ConnectMessage:
			printf("Connected %u ", message.getDestinationId());
			currentDestinations.insert(message.getDestinationId());
			break;
		case NetMessage::DisconnectMessage:
			printf("Disconnect %u ", message.getDestinationId());
			currentDestinations.erase(message.getDestinationId());
			break;
		}
	};
};

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "C");

	unsigned int initFlags = SDL_INIT_NOPARACHUTE;
	if (SDL_Init(initFlags) < 0)
	{
		dialogExit(
			"nettest",
			"This game uses the SDL library to provide graphics.\n"
			"The graphics section of this library failed to initialize.");
	}

	NetInterface *netInterface = new NetServerTCP2();
	netInterface->setMessageHandler(new TestNetMessageHandler());
	if (argc == 2)
	{
		netInterface->connect(argv[1], 27200);
	}
	else
	{
		netInterface->start(27200);
	}

	printf("Started");

	char data[100000];
	int nextSend = 0;
	NetBuffer buffer;
	while (true)
	{
		if (--nextSend <= 0)
		{
			buffer.reset();
			buffer.addDataToBuffer(data, rand() % 100000);

			if (argc == 2)
			{
				printf("Sent ");
				netInterface->sendMessageServer(buffer);
				nextSend = rand() % 100;
			}
			else
			{
				if (!currentDestinations.empty())
				{
					int dest = 0;
					int pos = rand() % int(currentDestinations.size());
					std::set<unsigned int>::iterator itor;
					for (itor = currentDestinations.begin();
						itor != currentDestinations.end();
						itor++, pos--)
					{
						dest = *itor;
						if (pos == 0) break;
					}

					printf("Sent %u ", dest);
					netInterface->sendMessageDest(buffer, dest);
				}
				nextSend = rand() % 10;
			}
		}

		netInterface->processMessages();
		SDL_Delay(10);
	}

	SDL_Quit();
	return 0; // exit(0)
}
#include <client/ScorchedClient.h>

ScorchedClient *ScorchedClient::instance_ = 0;

ScorchedClient *ScorchedClient::instance()
{
	if (!instance_)
	{
		instance_ = new ScorchedClient;
	}
	return instance_;
}

ScorchedClient::ScorchedClient()
{
	mainLoop_.clear();
	mainLoop_.addMainLoop(&gameState_);
}

ScorchedClient::~ScorchedClient()
{
}

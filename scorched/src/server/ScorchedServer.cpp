#include <server/ScorchedServer.h>

ScorchedServer *ScorchedServer::instance_ = 0;

ScorchedServer *ScorchedServer::instance()
{
	if (!instance_)
	{
		instance_ = new ScorchedServer;
	}
	return instance_;
}

ScorchedServer::ScorchedServer()
{
}

ScorchedServer::~ScorchedServer()
{
}

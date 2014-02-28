////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2013
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#if !defined(__INCLUDE_ScorchedServerh_INCLUDE__)
#define __INCLUDE_ScorchedServerh_INCLUDE__

#include <engine/ScorchedContext.h>
#include <common/fixed.h>
#include <string>

class ProgressCounter;
class TankDeadContainer;
class TankAIStore;
class ThreadCallback;
class EconomyStore;
class ServerSimulator;
class ServerDestinations;
class ServerState;
class ServerAuthHandler;
class ServerAuthHandlerStore;
class ServerTimedMessage;
class ServerBanned;
class ServerTextFilter;
class ServerHandlers;
class ServerLoadLevel;
class ServerConnectAuthHandler;
class ServerAdminSessions;
class ServerChannelManager;
class ServerSyncCheck;
class ServerMessageHandler;
class ServerFileServer;
class ScorchedServerSettings;
class EventHandlerDataBase;
class ThreadCallbackI;

class ScorchedServer : public ScorchedContext
{
public:
	static ScorchedServer *instance();

	static void startServer(ScorchedServerSettings *settings, 
		ProgressCounter *counter, ThreadCallbackI *endCallback);
	static void stopServer();

	virtual bool getServerMode() { return true; }

	virtual Simulator &getSimulator();
	virtual TargetSpace &getTargetSpace() { return *targetSpace_; }
	TankDeadContainer &getTankDeadContainer() { return *deadContainer_; }
	ScorchedContext &getContext() { return *this; }
	TankAIStore &getTankAIs() { return *tankAIStore_; }
	ServerSimulator &getServerSimulator() { return *serverSimulator_; }
	ServerDestinations &getServerDestinations() { return *serverDestinations_; }
	ServerState &getServerState() { return *serverState_; }
	ServerAuthHandler *getAuthHandler();
	ServerTimedMessage &getTimedMessage() { return *timedMessage_; }
	ServerBanned &getBannedPlayers() { return *bannedPlayers_; }
	ServerTextFilter &getTextFilter() { return *textFilter_; }
	ServerHandlers &getServerHandlers() { return *serverHandlers_; }
	ServerLoadLevel &getServerLoadLevel() { return *serverLoadLevel_; }
	ServerConnectAuthHandler &getServerConnectAuthHandler();
	ServerChannelManager &getServerChannelManager() { return *serverChannelManager_; }
	ServerAdminSessions &getServerAdminSessions() { return *serverAdminSessions_; }
	ServerSyncCheck &getServerSyncCheck() { return *serverSyncCheck_; }
	ServerMessageHandler &getServerMessageHandler() { return *serverMessageHandler_; }
	ServerFileServer &getServerFileServer() { return *serverFileServer_; }
	EconomyStore &getEconomyStore() { return *economyStore_; }
	EventHandlerDataBase *getEventHandlerDataBase() { return eventHandlerDataBase_; }
	static ThreadCallback &getThreadCallback() { return *threadCallback_; }

protected:
	static ScorchedServer *instance_;
	static TargetSpace *targetSpace_;
	static ThreadCallback *threadCallback_;
	
	TankDeadContainer *deadContainer_;
	TankAIStore *tankAIStore_;
	ServerSimulator *serverSimulator_;
	ServerDestinations *serverDestinations_;
	ServerState *serverState_;
	ServerAuthHandlerStore *authHandler_;
	ServerTimedMessage *timedMessage_;
	ServerBanned *bannedPlayers_;
	ServerTextFilter *textFilter_;
	ServerHandlers *serverHandlers_;
	ServerLoadLevel *serverLoadLevel_;
	ServerChannelManager *serverChannelManager_;	
	ServerAdminSessions *serverAdminSessions_;
	ServerSyncCheck *serverSyncCheck_;
	ServerMessageHandler *serverMessageHandler_;
	ServerFileServer *serverFileServer_;
	EconomyStore *economyStore_;
	EventHandlerDataBase *eventHandlerDataBase_;

	void checkSettings();
	static void startServerInternalStatic(ScorchedServer *instance, 
		ScorchedServerSettings *settings, 
		ProgressCounter *counter, ThreadCallbackI *endCallback);
	void startServerInternal(ScorchedServerSettings *settings, ProgressCounter *counter, ThreadCallbackI *endCallback);
	bool serverLoop(fixed timeDifference);

private:
	ScorchedServer();
	virtual ~ScorchedServer();
};

#endif

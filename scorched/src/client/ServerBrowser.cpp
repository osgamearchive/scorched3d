////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
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
//    You should have received a copy of the GNU General Public License
//    along with Scorched3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////

#include <client/ServerBrowser.h>

ServerBrowser *ServerBrowser::instance_ = 0;

ServerBrowser *ServerBrowser::instance()
{
	if (!instance_)
	{
		instance_ = new ServerBrowser;
	}
	return instance_;
}

ServerBrowser::ServerBrowser() : 
	refreshing_(false), serverList_(), serverRefresh_(serverList_)
{
}

ServerBrowser::~ServerBrowser()	
{
}

void ServerBrowser::refresh(bool lan)
{
	if (refreshing_) return;

	refreshing_ = true;
	SDL_CreateThread(ServerBrowser::threadFunc, (void *) lan);
}

int ServerBrowser::threadFunc(void *var)
{
	bool lan = (bool) var;
	bool result = false;
	if (lan) result = instance_->serverList_.fetchLANList();
	else result = instance_->serverList_.fetchServerList();
	if (result)
	{
		instance_->serverRefresh_.refreshList();
	}
	
	instance_->refreshing_ = false;
	return 0;
}

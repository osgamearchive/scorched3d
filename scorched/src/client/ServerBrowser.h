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

#if !defined(__INCLUDE_ServerBrowserh_INCLUDE__)
#define __INCLUDE_ServerBrowserh_INCLUDE__

#include <client/ServerBrowserRefresh.h>

class ServerBrowser 
{
public:
	static ServerBrowser *instance();

	void refresh();
	bool getRefreshing() { return refreshing_; }
	
	ServerBrowserServerList &getServerList() { return serverList_; }

protected:
	static ServerBrowser *instance_;
	bool refreshing_;
	unsigned int refreshId_;
	ServerBrowserServerList serverList_;
	ServerBrowserRefresh serverRefresh_;

	static int threadFunc(void *);

private:
	ServerBrowser();
	virtual ~ServerBrowser();
};

#endif

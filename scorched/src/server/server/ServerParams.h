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

#if !defined(__INCLUDE_ServerParamsh_INCLUDE__)
#define __INCLUDE_ServerParamsh_INCLUDE__

#include <common/OptionsParameters.h>

class ServerParams : public OptionsParameters
{
public:
	static ServerParams *instance();

	std::string getServerFile() { return server_.getValue(); }
	bool getHideWindow() { return hideWindow_.getValue(); }
	bool getServerCustom() { return startCustom_.getValue(); }
	bool getExitOnSyncFailure() { return exitOnSyncFailure_.getValue(); }
	int getExitTime() { return exitTime_.getValue(); }
	void setExitTime(int exitTime) { exitTime_.setValue(exitTime); }

	void setServerFile(const std::string &file) { server_.setValue(file); }

protected:
	static ServerParams *instance_;

	XMLEntryString server_;
	XMLEntryInt exitTime_;
	XMLEntryBool hideWindow_;
	XMLEntryBool startCustom_;
	XMLEntryBool exitOnSyncFailure_;

private:
	ServerParams();
	virtual ~ServerParams();

};

#endif

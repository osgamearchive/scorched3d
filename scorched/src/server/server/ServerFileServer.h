////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2009
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

#if !defined(__INCLUDE_ServerFileServerh_INCLUDE__)
#define __INCLUDE_ServerFileServerh_INCLUDE__

#include <coms/ComsFileMessage.h>

class Tank;
class ServerFileServer
{
public:
	static ServerFileServer *instance();

	void simulate(float timeDifference);

protected:
	static ServerFileServer *instance_;
	unsigned int lastTime_;
	unsigned int bytesSent_;

	void sendBytes(Tank *tank, unsigned int size);
	bool sendNextFile(ComsFileMessage &message,
		Tank *tank, unsigned int size, unsigned int &sentSize);

private:
	ServerFileServer();
	virtual ~ServerFileServer();
};

#endif // __INCLUDE_ServerFileServerh_INCLUDE__
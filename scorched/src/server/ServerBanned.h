////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#if !defined(__INCLUDE_ServerBannedh_INCLUDE__)
#define __INCLUDE_ServerBannedh_INCLUDE__

#include <set>
#include <list>

class ServerBanned
{
public:
	static ServerBanned *instance();
	struct BannedRange
	{
		unsigned int mask;
		std::set<unsigned int> ips;
	};	
	std::list<BannedRange> &getBannedIps() { return bannedIps_; }

	bool isBanned(unsigned int ip);
	void addBanned(unsigned int ip);

	bool save();
	bool load();

protected:
	static ServerBanned *instance_;
	std::list<BannedRange> bannedIps_;	

	void addBannedEntry(unsigned int ip, unsigned int mask);

private:
	ServerBanned();
	virtual ~ServerBanned();
};

#endif // __INCLUDE_ServerBannedh_INCLUDE__


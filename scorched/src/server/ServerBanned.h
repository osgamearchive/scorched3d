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
#include <map>
#include <list>
#include <string>
#include <time.h>

class ServerBanned
{
public:
	static ServerBanned *instance();
	enum BannedType
	{
		NotBanned,
		Banned,
		Muted,
		Flagged
	};
	struct BannedEntry
	{
		time_t bantime;
		std::string name;
		BannedType type;
	};
	struct BannedRange
	{
		unsigned int mask;
		std::map<unsigned int, BannedEntry> ips;
	};	
	std::list<BannedRange> &getBannedIps() { return bannedIps_; }

	BannedType getBanned(unsigned int ip);
	void addBanned(unsigned int ip, const char *name, BannedType type = Banned);

	static const char *getBannedTypeStr(BannedType type);

	bool save();
	bool load();

protected:
	static ServerBanned *instance_;
	std::list<BannedRange> bannedIps_;	

	void addBannedEntry(unsigned int ip, unsigned int mask,
		const char *name, unsigned int bantime,
		BannedType type);

private:
	ServerBanned();
	virtual ~ServerBanned();
};

#endif // __INCLUDE_ServerBannedh_INCLUDE__


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

#if !defined(__INCLUDE_TankAIComputerBuyerh_INCLUDE__)
#define __INCLUDE_TankAIComputerBuyerh_INCLUDE__

#include <weapons/AccessoryStore.h>
#include <list>
#include <vector>
#include <map>

class Tank;
class TankAIComputerBuyer
{
public:
	TankAIComputerBuyer();
	virtual ~TankAIComputerBuyer();

	bool parseConfig(AccessoryStore &store, XMLNode *node);
	bool addAccessory(AccessoryStore &store, 
		const char *accessoryName, int buyLevel);
	void buyAccessories(int maxNoBought);
	void clearAccessories();

	std::vector<Accessory *> getWeaponType(const char *type);

	std::multimap<std::string, std::string> &getTypes() { return buyTypes_; }
	void setTank(Tank *tank) { currentTank_ = tank; }

	void dumpAccessories();

protected:
	struct Entry
	{
		Entry() {}
		Entry(const Entry &other);
		Entry &operator=(const Entry &other);
		virtual ~Entry() { }

		std::list<std::string> buyAccessories;
		int level;
	};
	std::list<Entry> buyEntries_;
	std::multimap<std::string, std::string> buyTypes_;

	Tank *currentTank_;
	void buyAccessory();
};


#endif

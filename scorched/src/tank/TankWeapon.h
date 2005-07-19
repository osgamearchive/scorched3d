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

#if !defined(AFX_TANKWEAPON_H__CC20069B_6E50_49E0_B735_D739BCBF58A9__INCLUDED_)
#define AFX_TANKWEAPON_H__CC20069B_6E50_49E0_B735_D739BCBF58A9__INCLUDED_

#include <map>
#include <list>
#include <string>
#include <coms/NetBuffer.h>

class ScorchedContext;
class Accessory;
class TankWeapon  
{
public:
	TankWeapon(ScorchedContext &context);
	virtual ~TankWeapon();

	void reset();

	void addWeapon(Accessory *wp, int count); // (count < 0) == infinite
	void rmWeapon(Accessory *wp, int count);
	bool setWeapon(Accessory *wp);

	// Change the currently selected weapon
	void prevWeapon();
	void nextWeapon();
	Accessory *getCurrent();

	// Get inventry
	std::list<Accessory *> getAllWeapons(bool sort=false);
	int getWeaponCount(Accessory *weapon);

	const char *getWeaponString();

	// Serialize the tank
    bool writeMessage(NetBuffer &buffer, bool writeAccessories);
    bool readMessage(NetBufferReader &reader);

protected:
	std::map<Accessory *, int> weapons_;
	Accessory *currentWeapon_;
	ScorchedContext &context_;

	void setCurrentWeapon(Accessory *wp);

};

#endif // !defined(AFX_TANKWEAPON_H__CC20069B_6E50_49E0_B735_D739BCBF58A9__INCLUDED_)

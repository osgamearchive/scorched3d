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


// TankWeapon.h: interface for the TankWeapon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TANKWEAPON_H__CC20069B_6E50_49E0_B735_D739BCBF58A9__INCLUDED_)
#define AFX_TANKWEAPON_H__CC20069B_6E50_49E0_B735_D739BCBF58A9__INCLUDED_

#include <map>
#include <list>
#include <string>

#include <weapons/Weapon.h>

class TankWeapon  
{
public:
	TankWeapon();
	virtual ~TankWeapon();

	void reset();

	void addWeapon(Weapon *wp, int count); // (count < 0) == infinite
	void rmWeapon(Weapon *wp, int count);
	bool setWeapon(Weapon *wp);

	// Change the currently selected weapon
	void prevWeapon();
	void nextWeapon();
	Weapon *getCurrent();

	// Get inventry
	std::map<Weapon *, int> &getAllWeapons() { return weapons_; }
	int getWeaponCount(Weapon *weapon);

	const char *getWeaponString();

	// Serialize the tank
    bool writeMessage(NetBuffer &buffer);
    bool readMessage(NetBufferReader &reader);

protected:
	std::map<Weapon *, int> weapons_;
	Weapon *currentWeapon_;

	void setCurrentWeapon(Weapon *wp);

};

#endif // !defined(AFX_TANKWEAPON_H__CC20069B_6E50_49E0_B735_D739BCBF58A9__INCLUDED_)

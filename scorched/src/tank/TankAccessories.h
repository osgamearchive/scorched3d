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

#if !defined(AFX_TANKACCESSORIES_H__60850A18_DED2_4BB0_B104_CB0941EF6D1D__INCLUDED_)
#define AFX_TANKACCESSORIES_H__60850A18_DED2_4BB0_B104_CB0941EF6D1D__INCLUDED_

#include <tank/TankWeapon.h>
#include <tank/TankParachutes.h>
#include <tank/TankShields.h>
#include <tank/TankAutoDefense.h>
#include <tank/TankBatteries.h>
#include <tank/TankFuel.h>

class ScorchedContext;
class TankAccessories  
{
public:
	TankAccessories(ScorchedContext &context);
	virtual ~TankAccessories();

	void newMatch();
	void add(Accessory *accessory, int count = -1);
	void rm(Accessory *accessory);

	int getAccessoryCount(Accessory *accessory);
	std::list<Accessory *> getAllAccessories(bool sort=false);

	TankFuel &getFuel() { return tankFuel_; }
	TankWeapon &getWeapons() { return tankWeapon_; }
	TankParachutes &getParachutes() { return tankPara_; }
	TankShields &getShields() { return tankShield_; }
	TankAutoDefense &getAutoDefense() { return tankAuto_; }
	TankBatteries &getBatteries() { return tankBatteries_; }

	// Serialize the tank accessories
	bool writeMessage(NetBuffer &buffer, bool writeAccessories);
	bool readMessage(NetBufferReader &reader);

protected:
	ScorchedContext &context_;
	TankFuel tankFuel_;
	TankWeapon tankWeapon_;
	TankParachutes tankPara_;
	TankShields tankShield_;
	TankAutoDefense tankAuto_;
	TankBatteries tankBatteries_;
};

#endif // !defined(AFX_TANKACCESSORIES_H__60850A18_DED2_4BB0_B104_CB0941EF6D1D__INCLUDED_)

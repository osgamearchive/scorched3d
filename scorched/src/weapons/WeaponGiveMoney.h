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

#if !defined(__INCLUDE_WeaponGiveMoneyh_INCLUDE__)
#define __INCLUDE_WeaponGiveMoneyh_INCLUDE__

#include <actions/PowerUp.h>

class WeaponGiveMoney  : public WeaponPowerUp
{
public:
	WeaponGiveMoney();
	virtual ~WeaponGiveMoney();

	virtual bool parseXML(OptionsGame &context, 
		AccessoryStore *store, XMLNode *accessoryNode);

	// Inherited from Weapon
	void fireWeapon(ScorchedContext &context,
		unsigned int playerId, Vector &position, Vector &velocity,
		unsigned int data = 0);

	// Inherited from WeaponPowerUp
	virtual void invokePowerUp(ScorchedContext &context,
		unsigned int playerId, Vector &position, Vector &velocity,
		unsigned int data);

	REGISTER_ACCESSORY_HEADER(WeaponGiveMoney, AccessoryPart::AccessoryWeapon);

protected:
	int money_;

};

#endif // __INCLUDE_WeaponGiveMoneyh_INCLUDE__

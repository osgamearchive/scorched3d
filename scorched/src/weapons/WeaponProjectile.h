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

#if !defined(AFX_WEAPONPROJECTILE_H__70119A64_2064_4066_8EE5_FD6A3E24D5FC__INCLUDED_)
#define AFX_WEAPONPROJECTILE_H__70119A64_2064_4066_8EE5_FD6A3E24D5FC__INCLUDED_

#include <weapons/Weapon.h>

class WeaponProjectile : public Weapon
{
public:
	WeaponProjectile();
	virtual ~WeaponProjectile();

	virtual bool parseXML(XMLNode *accessoryNode);
	virtual bool writeAccessory(NetBuffer &buffer);
	virtual bool readAccessory(NetBufferReader &reader);

	Weapon *getCollisionAction() { return collisionAction_; }

	// Inherited from Weapon
	virtual void fireWeapon(ScorchedContext &context,
		unsigned int playerId, Vector &position, Vector &velocity,
		unsigned int data = 0);

	REGISTER_ACCESSORY_HEADER(WeaponProjectile, Accessory::AccessoryWeapon);

	bool getUnder() { return under_; }
	bool getShowShotPath() { return showShotPath_; }
	bool getShowEndPoint() { return showEndPoint_; }
	bool getApexCollision() { return apexCollision_; }
	bool getCreateSmoke() { return createSmoke_; }
	bool getCreateFlame() { return createFlame_; }

protected:
	bool under_;
	bool showShotPath_;
	bool showEndPoint_;
	bool apexCollision_;
	bool createSmoke_;
	bool createFlame_;
	Weapon *collisionAction_;

};

#endif // !defined(AFX_WEAPONPROJECTILE_H__70119A64_2064_4066_8EE5_FD6A3E24D5FC__INCLUDED_)

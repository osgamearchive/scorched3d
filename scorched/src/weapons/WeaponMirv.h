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

#if !defined(AFX_WEAPONMIRV_H__B5C043F0_7DC6_4198_AE5B_E19002234FCE__INCLUDED_)
#define AFX_WEAPONMIRV_H__B5C043F0_7DC6_4198_AE5B_E19002234FCE__INCLUDED_

#include <weapons/Weapon.h>

class WeaponMirv  : public Weapon
{
public:
	WeaponMirv();
	virtual ~WeaponMirv();

	virtual bool parseXML(XMLNode *accessoryNode);
	virtual bool writeAccessory(NetBuffer &buffer);
	virtual bool readAccessory(NetBufferReader &reader);

	// Inherited from Weapon
	Action *fireWeapon(unsigned int playerId, Vector &position, Vector &velocity);
	virtual const char *getFiredSound();
	virtual const char *getExplosionSound();
	virtual const char *getExplosionTexture();

	REGISTER_ACCESSORY_HEADER(WeaponMirv, Accessory::AccessoryWeapon);

protected:
	int noWarheads_;
	int size_;
	bool spread_;

};

#endif // !defined(AFX_WEAPONMIRV_H__B5C043F0_7DC6_4198_AE5B_E19002234FCE__INCLUDED_)

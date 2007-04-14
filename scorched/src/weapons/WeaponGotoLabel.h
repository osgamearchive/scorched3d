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

#if !defined(AFX_WeaponGotoLabel_H__B5C043F0_7DC6_4198_AE5B_E19002234FCE__INCLUDED_)
#define AFX_WeaponGotoLabel_H__B5C043F0_7DC6_4198_AE5B_E19002234FCE__INCLUDED_

#include <weapons/WeaponLabel.h>

class WeaponGotoLabel  : public Weapon
{
public:
	WeaponGotoLabel();
	virtual ~WeaponGotoLabel();

	virtual bool parseXML(AccessoryCreateContext &context,
		XMLNode *accessoryNode);

	// Inherited from Weapon
	void fireWeapon(ScorchedContext &context,
		WeaponFireContext &weaponContext, Vector &position, Vector &velocity);

	REGISTER_ACCESSORY_HEADER(WeaponGotoLabel, AccessoryPart::AccessoryWeapon);

protected:
	Weapon *weaponLabel_;
	int count_;

};

#endif // !defined(AFX_WeaponGotoLabel_H__B5C043F0_7DC6_4198_AE5B_E19002234FCE__INCLUDED_)
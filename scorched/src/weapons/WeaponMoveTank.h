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

#if !defined(AFX_WeaponMoveTank_H__B5C043F0_7DC6_4198_AE5B_E19002234FCE__INCLUDED_)
#define AFX_WeaponMoveTank_H__B5C043F0_7DC6_4198_AE5B_E19002234FCE__INCLUDED_

#include <weapons/Weapon.h>

class WeaponMoveTank  : public Weapon
{
public:
	WeaponMoveTank();
	virtual ~WeaponMoveTank();

	virtual bool parseXML(OptionsGame &context, 
		AccessoryStore *store, XMLNode *accessoryNode);

	int getMaximumRange() { return maximumRange_; }
	float getStepTime() { return stepTime_; }
	bool getUseFuel() { return useFuel_; }

	// Inherited from Weapon
	void fireWeapon(ScorchedContext &context,
		unsigned int playerId, Vector &position, Vector &velocity,
		unsigned int data = 0);

	REGISTER_ACCESSORY_HEADER(WeaponMoveTank, AccessoryPart::AccessoryWeapon);

protected:
	int maximumRange_;
	float stepTime_;
	bool useFuel_;

};

#endif // !defined(AFX_WeaponMoveTank_H__B5C043F0_7DC6_4198_AE5B_E19002234FCE__INCLUDED_)
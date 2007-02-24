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

#if !defined(__INCLUDE_WeaponRollerh_INCLUDE__)
#define __INCLUDE_WeaponRollerh_INCLUDE__

#include <weapons/Weapon.h>
#include <common/ModelID.h>

class ScorchedContext;
class WeaponRoller : public Weapon
{
public:
	WeaponRoller();
	virtual ~WeaponRoller();

	virtual bool parseXML(AccessoryCreateContext &context,
		XMLNode *accessoryNode);

	Weapon *getCollisionAction() { return collisionAction_; }
	ModelID &getRollerModelID() { return rollerModelId_; }
	float getShieldHurtFactor() { return shieldHurtFactor_; }
	float getTime(ScorchedContext &context);
	float getTime() { return time_; }
	float getWindFactor() { return windFactor_; }

	// Inherited from Weapon
	virtual void fireWeapon(ScorchedContext &context,
		WeaponFireContext &weaponContext, Vector &position, Vector &velocity);

	REGISTER_ACCESSORY_HEADER(WeaponRoller, AccessoryPart::AccessoryWeapon);

protected:
	NumberParser numberRollers_;
	Weapon *collisionAction_;
	ModelID rollerModelId_;
	float shieldHurtFactor_;
	float windFactor_;
	float time_;
	bool maintainVelocity_;

	void addRoller(ScorchedContext &context, 
		WeaponFireContext &weaponContext,
		Vector &position, Vector &velocity);

};

#endif

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

class WeaponRoller : public Weapon
{
public:
	WeaponRoller();
	virtual ~WeaponRoller();

	virtual bool parseXML(XMLNode *accessoryNode);
	virtual bool writeAccessory(NetBuffer &buffer);
	virtual bool readAccessory(NetBufferReader &reader);

	const float getRollerTime() { return rollerTime_; }
	const float getRollerHeight() { return rollerHeight_; }
	const float getStepTime() { return stepTime_; }
	const int getNumberRollers() { return numberRollers_; }
	const int getSize() { return size_; }

	// Inherited from Weapon
	virtual void fireWeapon(ScorchedContext &context,
		unsigned int playerId, Vector &position, Vector &velocity);

	REGISTER_ACCESSORY_HEADER(WeaponRoller, Accessory::AccessoryWeapon);

protected:
	float rollerTime_;   // The time to generate Roller
	float rollerHeight_; // The height of a roller point
	float stepTime_;     // Move roller every StepTime secs
	int numberRollers_;  // The number of rollers
	int size_;           // Explosion size

	void addRoller(ScorchedContext &context, unsigned int playerId,
		int x, int y);

};


#endif

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


#if !defined(__INCLUDE_WeaponNapalmh_INCLUDE__)
#define __INCLUDE_WeaponNapalmh_INCLUDE__

#include <weapons/Weapon.h>

class WeaponNapalm : public Weapon
{
public:
	WeaponNapalm();
	virtual ~WeaponNapalm();

	virtual bool parseXML(XMLNode *accessoryNode);
	virtual bool writeAccessory(NetBuffer &buffer);
	virtual bool readAccessory(NetBufferReader &reader);

	const float getNapalmTime() { return napalmTime_; }
	const float getNaplamHeight() { return napalmHeight_; }
	const float getStepTime() { return stepTime_; }
	const float getHurtStepTime() { return hurtStepTime_; }
	const float getHurtPerSecond() { return hurtPerSecond_; }
	const int getEffectRadius() { return effectRadius_; }
	const int getNumberStreams() { return numberStreams_; }

	// Inherited from Weapon
	virtual void fireWeapon(ScorchedContext &context,
		unsigned int playerId, Vector &position, Vector &velocity);

	REGISTER_ACCESSORY_HEADER(WeaponNapalm, Accessory::AccessoryWeapon);

protected:
	float napalmTime_;   // The time to generate napalm
	float napalmHeight_; // The height of a napalm point
	float stepTime_;     // Add/rm napalm every StepTime secs
	float hurtStepTime_; // Calculate damage every HurtStepTime secs
	float hurtPerSecond_;// Damage per second
	int numberStreams_;  // The number of napalm paths
	int effectRadius_;   // How close do tanks take damage

	void addNapalm(ScorchedContext &context, unsigned int playerId,
		int x, int y);
};

#endif

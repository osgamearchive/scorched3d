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

#if !defined(__INCLUDE_WeaponLightningh_INCLUDE__)
#define __INCLUDE_WeaponLightningh_INCLUDE__

#include <weapons/Weapon.h>

class WeaponLightning : public Weapon
{
public:
	WeaponLightning();
	virtual ~WeaponLightning();

	virtual bool parseXML(OptionsGame &context, 
		AccessoryStore *store, XMLNode *accessoryNode);

	float getConeLength() { return coneLength_; }
	float getSegLength() { return segLength_; }
	float getSegVar() { return segVar_; }
	float getSize() { return size_; }
	float getSizeVar() { return sizeVar_; }
	float getMinSize() { return minSize_; }
	float getSplitProb() { return splitProb_; }
	float getSplitVar() { return splitVar_; }
	float getDeathProb() { return deathProb_; }
	float getDerivAngle() { return derivAngle_; }
	float getAngleVar() { return angleVar_; }
	float getTotalTime() { return totalTime_; }
	float getSegHurt() { return segHurt_; }
	float getSegHurtRadius() { return segHurtRadius_; }
	const char *getSound() { return sound_.c_str(); }

	// Inherited from Weapon
	void fireWeapon(ScorchedContext &context,
		unsigned int playerId, Vector &position, Vector &velocity,
		unsigned int data = 0);

	REGISTER_ACCESSORY_HEADER(WeaponLightning, AccessoryPart::AccessoryWeapon);

protected:
	float coneLength_;
	float segLength_;
	float segVar_;
	float size_;
	float sizeVar_;
	float minSize_;
	float splitProb_;
	float splitVar_;
	float deathProb_;
	float derivAngle_;
	float angleVar_;
	float totalTime_;
	float segHurt_;
	float segHurtRadius_;
	std::string sound_;

};

#endif // __INCLUDE_WeaponLightningh_INCLUDE__

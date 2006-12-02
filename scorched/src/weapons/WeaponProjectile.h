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
#include <common/ModelID.h>

class WeaponProjectile : public Weapon
{
public:
	WeaponProjectile();
	virtual ~WeaponProjectile();

	virtual bool parseXML(OptionsGame &context, 
		AccessoryStore *store, XMLNode *accessoryNode);

	Weapon *getCollisionAction() { return collisionAction_; }

	// Inherited from Weapon
	virtual void fireWeapon(ScorchedContext &context,
		unsigned int playerId, Vector &position, Vector &velocity,
		unsigned int data = 0);

	REGISTER_ACCESSORY_HEADER(WeaponProjectile, AccessoryPart::AccessoryWeapon);

	bool getUnder() { return under_; }
	bool getShowShotPath() { return showShotPath_; }
	bool getShowEndPoint() { return showEndPoint_; }
	bool getApexCollision() { return apexCollision_; }
	bool getApexNoDud() { return apexNoDud_; }
	bool getCreateSmoke() { return createSmoke_; }
	bool getCreateFlame() { return createFlame_; }
	bool getTimedDud() { return timedDud_; }
	float getWindFactor() { return windFactor_; }
	float getShieldHurtFactor() { return shieldHurtFactor_; }
	float getTimedCollision() { return timedCollision_; }
	float getSpinSpeed() { return spinSpeed_; }
	float getFlameLife() { return flameLife_; }
	float getFlameStartSize() { return flameStartSize_; }
	float getFlameEndSize() { return flameEndSize_; }
	float getSmokeLife() { return smokeLife_; }
	float getSmokeStartSize() { return smokeStartSize_; }
	float getSmokeEndSize() { return smokeEndSize_; }
	Vector &getFlameStartColor1() { return flameStartColor1_; }
	Vector &getFlameStartColor2() { return flameStartColor2_; }
	Vector &getFlameEndColor1() { return flameEndColor1_; }
	Vector &getFlameEndColor2() { return flameEndColor2_; }
	const char *getEngineSound() { return engineSound_.c_str(); }
	float getScale() { return scale_; }
	ModelID &getModelID() { return modelId_; }

protected:
	bool under_;
	bool showShotPath_;
	bool showEndPoint_;
	bool apexCollision_;
	bool apexNoDud_, timedDud_;
	float spinSpeed_;
	bool createSmoke_, createFlame_;
	float flameLife_, smokeLife_;
	float flameStartSize_, flameEndSize_;
	float smokeStartSize_, smokeEndSize_;
	float timedCollision_;
	float shieldHurtFactor_;
	float scale_;
	float windFactor_;
	Vector flameStartColor1_, flameStartColor2_;
	Vector flameEndColor1_, flameEndColor2_;
	std::string engineSound_;
	Weapon *collisionAction_;
	ModelID modelId_;
	int flareType_;

};

#endif // !defined(AFX_WEAPONPROJECTILE_H__70119A64_2064_4066_8EE5_FD6A3E24D5FC__INCLUDED_)

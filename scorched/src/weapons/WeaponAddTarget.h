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

#if !defined(__INCLUDE_WeaponAddTargeth_INCLUDE__)
#define __INCLUDE_WeaponAddTargeth_INCLUDE__

#include <weapons/Weapon.h>
#include <3dsparse/ModelID.h>

class Shield;
class Parachute;
class WeaponAddTarget : public Weapon
{
public:
	WeaponAddTarget();
	virtual ~WeaponAddTarget();

	ModelID &getTargetModel() { return targetModelId_; }
	Shield *getShield() { return shield_; }
	Parachute *getParachute() { return parachute_; }
	Weapon *getDeathAction() { return deathAction_; }

	virtual bool parseXML(OptionsGame &context, 
		AccessoryStore *store, XMLNode *accessoryNode);
	void fireWeapon(ScorchedContext &context, 
		unsigned int playerId, Vector &position, Vector &velocity,
		unsigned int data = 0);

	REGISTER_ACCESSORY_HEADER(WeaponAddTarget, AccessoryPart::AccessoryWeapon);

protected:
	ModelID targetModelId_;
	Shield *shield_;
	Parachute *parachute_;
	Weapon *deathAction_;
};

#endif // __INCLUDE_WeaponAddTargeth_INCLUDE__

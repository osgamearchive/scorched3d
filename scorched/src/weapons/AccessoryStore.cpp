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

#include <weapons/AccessoryStore.h>
#include <weapons/Parachute.h>
#include <weapons/Battery.h>
#include <weapons/AutoDefense.h>
#include <weapons/Shield.h>
#include <weapons/Fuel.h>
#include <weapons/ShieldReflective.h>
#include <weapons/ShieldReflectiveMag.h>
#include <weapons/Weapon.h>
#include <weapons/WeaponProjectile.h>
#include <weapons/WeaponTracer.h>
#include <weapons/WeaponClod.h>
#include <weapons/WeaponFunky.h>
#include <weapons/WeaponMirv.h>
#include <weapons/WeaponRiotBomb.h>
#include <weapons/WeaponNapalm.h>

#define ADD_NEW_ACCESSORY(x) { Accessory *acc = x; accessories_.push_back(acc); }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AccessoryStore *AccessoryStore::instance_ = 0;

AccessoryStore *AccessoryStore::instance()
{
	if (!instance_)
	{
		instance_ = new AccessoryStore;
	}

	return instance_;
}

AccessoryStore::AccessoryStore()
{
	// Add all accessories that can be bought and sold
	// Parameters are usually

	// Armslevel goes from 10->1 (actualy equals hit points)
	// 10 = Small weapon (large hit points)
	// 1  = Large weapon (small hit points)

	// Parachutes
	// char *name, int price, int bundle, int armsLevel
	ADD_NEW_ACCESSORY(new Parachute("Parachute", 10000, 8, 7));

	// Batteries
	ADD_NEW_ACCESSORY(new Battery("Battery", 5000, 10, 7));

	// Fuel
	ADD_NEW_ACCESSORY(new Fuel("Fuel", 10000, 20, 7));

	// Auto Defense
	// char *name, int price, int bundle, int armsLevel
	ADD_NEW_ACCESSORY(new AutoDefense("Auto Defense", 10000, 1, 7));

	// Napalm
	// char *name, int price, int bundle, int armsLevel, bool hot
	ADD_NEW_ACCESSORY(new WeaponNapalm("Napalm", 10000, 10, 7, false));
	ADD_NEW_ACCESSORY(new WeaponNapalm("Hot Napalm", 20000, 2, 5, true));

	// Pojectile weapons
	// char *name, int price, int bundle, int armsLevel, int size
	ADD_NEW_ACCESSORY(new WeaponProjectile("Baby Missile", 0, 0, 10, 4));
	ADD_NEW_ACCESSORY(new WeaponProjectile("Missile", 1875, 5, 9, 6));
	ADD_NEW_ACCESSORY(new WeaponProjectile("Baby Nuke", 10000, 3, 6, 10));
	ADD_NEW_ACCESSORY(new WeaponProjectile("Nuke", 12000, 1, 4, 18));

	// Earth clod weapons
	// char *name, int price, int bundle, int armsLevel, int size
	ADD_NEW_ACCESSORY(new WeaponClod("Dirt Clod", 5000, 10, 10, 6));
	ADD_NEW_ACCESSORY(new WeaponClod("Dirt Ball", 5000, 5, 9, 10));
	ADD_NEW_ACCESSORY(new WeaponClod("Ton Of Dirt", 6750, 2, 7, 18));

	// Earth remove weapons
	// char *name, int price, int bundle, int armsLevel, int size
	ADD_NEW_ACCESSORY(new WeaponRiotBomb("Riot Bomb", 5000, 5, 10, 6));
	ADD_NEW_ACCESSORY(new WeaponRiotBomb("Lrg Riot Bomb", 4750, 2, 8, 12));

	// Multiple warhead weapons
	// char *name, int price, int bundle, int armsLevel, int size, int numberWarheads, bool spread
	ADD_NEW_ACCESSORY(new WeaponMirv("MIRV", 10000, 3, 6, 6, 5, false));
	ADD_NEW_ACCESSORY(new WeaponMirv("Spread MIRV", 10000, 2, 5, 6, 5, true));
	ADD_NEW_ACCESSORY(new WeaponMirv("Death's Head", 20000, 1, 2, 18, 20, true));
	ADD_NEW_ACCESSORY(new WeaponFunky("Funky Bomb", 7000, 2, 3, 10, 8, 12));

	// Tracer
	// char *name, int price, int bundle, int armsLevel, bool showShotPath
	ADD_NEW_ACCESSORY(new WeaponTracer("Tracer", 10, 20, 10, false));
	ADD_NEW_ACCESSORY(new WeaponTracer("Smoke Tracer", 500, 10, 9, true));

	// Shields
	// char *name, int price, int bundle, int armsLevel, float radius, Vector color
	ADD_NEW_ACCESSORY(new Shield("Shield", 20000, 3, 8, Shield::ShieldSizeSmall, Vector(1.0f, 1.0f, 1.0f)));
	ADD_NEW_ACCESSORY(new Shield("Heavy Shield", 30000, 2, 7, Shield::ShieldSizeLarge, Vector(1.0f, 1.0f, 1.0f)));
	ADD_NEW_ACCESSORY(new ShieldReflective("Force Shield", 25000, 3, 6, Shield::ShieldSizeSmall, Vector(0.5f, 1.0f, 1.0f)));
	ADD_NEW_ACCESSORY(new ShieldReflectiveMag("Mag Deflect", 10000, 2, 8, Shield::ShieldSizeSmall, Vector(0.5f, 1.0f, 1.0f)));
	ADD_NEW_ACCESSORY(new ShieldReflectiveMag("Heavy Mag", 25000, 2, 6, Shield::ShieldSizeLarge, Vector(0.5f, 1.0f, 1.0f)));
}

AccessoryStore::~AccessoryStore()
{

}

std::list<Accessory *> AccessoryStore::getAllWeapons()
{
	std::list<Accessory *> result;
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++)
	{
		if ((*itor)->getType() == Accessory::AccessoryWeapon)
		{
			result.push_back(*itor);
		}
	}

	return result;
}

std::list<Accessory *> AccessoryStore::getAllOthers()
{
	std::list<Accessory *> result;
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++)
	{
		if ((*itor)->getType() != Accessory::AccessoryWeapon)
		{
			result.push_back(*itor);
		}
	}

	return result;
}

std::list<Accessory *> &AccessoryStore::getAllAccessories()
{
	return accessories_;
}

Accessory *AccessoryStore::findByName(const char *name)
{
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++)
	{
		if (0 == stricmp((*itor)->getName(), name))
		{
			return *itor;
		}
	}

	return 0;
}

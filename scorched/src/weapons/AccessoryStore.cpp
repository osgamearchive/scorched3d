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

#include <XML/XMLFile.h>
#include <common/Defines.h>
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
#include <weapons/WeaponLeapFrog.h>
#include <weapons/WeaponSandHog.h>
#include <weapons/WeaponDigger.h>
#include <math.h>

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
	parseFile();
}

AccessoryStore::~AccessoryStore()
{

}

Accessory *AccessoryStore::createAccessoryType(const char *type)
{
	// Crude, but it works !! :)
	if (stricmp(type, "Parachute") == 0) return new Parachute;
	else if (stricmp(type, "Battery") == 0) return new Battery;
	else if (stricmp(type, "Fuel") == 0) return new Fuel;
	else if (stricmp(type, "AutoDefense") == 0) return new AutoDefense;
	else if (stricmp(type, "WeaponNapalm") == 0) return new WeaponNapalm;
	else if (stricmp(type, "WeaponProjectile") == 0) return new WeaponProjectile;
	else if (stricmp(type, "WeaponLeapFrog") == 0) return new WeaponLeapFrog;
	else if (stricmp(type, "WeaponClod") == 0) return new WeaponClod;
	else if (stricmp(type, "WeaponRiotBomb") == 0) return new WeaponRiotBomb;
	else if (stricmp(type, "WeaponDigger") == 0) return new WeaponDigger;
	else if (stricmp(type, "WeaponSandHog") == 0) return new WeaponSandHog;
	else if (stricmp(type, "WeaponMirv") == 0) return new WeaponMirv;
	else if (stricmp(type, "WeaponFunky") == 0) return new WeaponFunky;
	else if (stricmp(type, "WeaponTracer") == 0) return new WeaponTracer;
	else if (stricmp(type, "Shield") == 0) return new Shield;
	else if (stricmp(type, "ShieldReflective") == 0) return new ShieldReflective;
	else if (stricmp(type, "ShieldReflectiveMag") == 0) return new ShieldReflectiveMag;

	return 0;
}

bool AccessoryStore::parseFile()
{
	XMLFile file;
	if (!file.readFile(PKGDIR "data/weapons.xml"))
	{
		dialogMessage("AccessoryStore", 
					  "Failed to parse \"data/weapons.xml\"\n%s", 
					  file.getParserError());
		return false;
	}

	// Check file exists
	if (!file.getRootNode())
	{
		dialogMessage("AccessoryStore",
					"Failed to find tank definition file \"data/weapons.xml\"");
		return false;		
	}

	// Itterate all of the tanks in the file
    std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = file.getRootNode()->getChildren();
    for (childrenItor = children.begin();
        childrenItor != children.end();
        childrenItor++)
    {
		// Parse the tank entry
        XMLNode *currentNode = (*childrenItor);
		if (stricmp(currentNode->getName(), "weapon"))
		{
			dialogMessage("AccessoryStore",
						  "Failed to find weapon node");
			return false;
		}

		XMLNode *typeNode = currentNode->getNamedParameter("type");
		if (!typeNode)
		{
			dialogMessage("AccessoryStore",
						  "Failed to find type node");
			return false;
		}

		Accessory *accessory = createAccessoryType(typeNode->getContent());
		if (!accessory)
		{
			dialogMessage("AccessoryStore",
						  "Failed to find a weapon type \"%s\"",
						  typeNode->getContent());
			return false;
		}
		
		if (!accessory->parseXML(currentNode)) return false;
		accessories_.push_back(accessory);

		// Add weapons to death animations, weighted by arms level
		if (accessory->getType() == Accessory::AccessoryWeapon)
		{
			Weapon *weapon = (Weapon *) accessory;
			for (int i=0; i<weapon->getArmsLevel(); i++)
			{
				deathAnimations_.push_back(weapon);
			}
		}
	}

	return true;
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

Weapon *AccessoryStore::getDeathAnimation()
{
	int pos = int(float(deathAnimations_.size()) * RAND);
	if (pos < (int) deathAnimations_.size()) return deathAnimations_[pos];
	return 0;
}

Accessory *AccessoryStore::findByAccessoryType(Accessory::AccessoryType type)
{
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		if (accessory->getType() == type) return accessory;
	}
	return 0;
}

Accessory *AccessoryStore::findByAccessoryName(const char *name)
{
	std::list<Accessory *>::iterator itor;
	for (itor = accessories_.begin();
		itor != accessories_.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		if (strcmp(accessory->getName(), name)==0) return accessory;
	}
	return 0;
}

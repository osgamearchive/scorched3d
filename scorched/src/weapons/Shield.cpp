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


// Shield.cpp: implementation of the Shield class.
//
//////////////////////////////////////////////////////////////////////

#include <weapons/Shield.h>
#include <common/VectorLib.h>
#include <common/Defines.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Shield::Shield(char *name, int price, int bundle, 
			   int armsLevel, ShieldSize radius, 
			   Vector color) :
	Accessory(name, price, bundle, armsLevel), 
		color_(color), radius_(radius)
{

}

Shield::~Shield()
{

}

Accessory::AccessoryType Shield::getType()
{
	return Accessory::AccessoryShield;
}

const char *Shield::getActivatedSound()
{
	return  PKGDIR "data/wav/shield/activate.wav";
}

const char *Shield::getCollisionSound()
{
	return PKGDIR "data/wav/shield/hit.wav";
}

float Shield::getHitRemovePower()
{
	if (radius_ == ShieldSizeSmall) return 20;
	return 15;
}

Shield::ShieldType Shield::getShieldType()
{
	return ShieldTypeNormal;
}

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


// Battery.cpp: implementation of the Battery class.
//
//////////////////////////////////////////////////////////////////////

#include <weapons/Battery.h>
#include <common/Defines.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Battery::Battery(char *name, int price, 
				 int bundle, int armsLevel) :
	Accessory(name, price, bundle, armsLevel)
{

}

Battery::~Battery()
{

}

Accessory::AccessoryType Battery::getType()
{
	return Accessory::AccessoryBattery;
}

const char *Battery::getActivatedSound()
{
	return PKGDIR "data/wav/battery/battery.wav";
}

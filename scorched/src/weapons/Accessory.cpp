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


// Accessory.cpp: implementation of the Accessory class.
//
//////////////////////////////////////////////////////////////////////

#include <weapons/Accessory.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Accessory::Accessory(char *name, int price, 
					 int bundle, int armsLevel) :
	name_(name), price_(price), 
	bundle_(bundle), armsLevel_(armsLevel),
	noBought_(0), noSold_(0)
{
	sellPrice_ = 0;
	if (price_ > 0 && bundle_ > 0) sellPrice_ = int((price_ / bundle_) * 0.8f);
}

Accessory::~Accessory()
{

}

const char *Accessory::getName()
{ 
	return name_.c_str(); 
}

const int Accessory::getPrice() 
{ 
	return price_; 
}

const int Accessory::getSellPrice() 
{ 
	return sellPrice_; 
}

const int Accessory::getBundle() 
{ 
	return bundle_; 
}

const int Accessory::getArmsLevel() 
{ 
	return armsLevel_; 
}

void Accessory::bought()
{
	noBought_++;
}

void Accessory::sold()
{
	noSold_++;
}

bool Accessory::singular()
{
	return false;
}

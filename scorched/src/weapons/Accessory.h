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


// Accessory.h: interface for the Accessory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCESSORY_H__21765D5B_DB45_4275_AB63_BAD1E84C1790__INCLUDED_)
#define AFX_ACCESSORY_H__21765D5B_DB45_4275_AB63_BAD1E84C1790__INCLUDED_


#include <string>

class Accessory  
{
public:
	enum AccessoryType
	{
		AccessoryWeapon,
		AccessoryParachute,
		AccessoryShield,
		AccessoryAutoDefense,
		AccessoryBattery,
		AccessoryFuel
	};

	Accessory(char *name, int price, 
		int bundle, int armsLevel);
	virtual ~Accessory();

	virtual const char *getName();
	virtual const int getPrice();
	virtual const int getSellPrice();
	virtual const int getBundle();
	virtual const int getArmsLevel();
	virtual void bought();
	virtual void sold();
	virtual bool singular();

	virtual AccessoryType getType() = 0;

protected:
	std::string name_;
	int price_;
	int bundle_;
	int armsLevel_;
	int sellPrice_;

	int noBought_;
	int noSold_;
};

#endif // !defined(AFX_ACCESSORY_H__21765D5B_DB45_4275_AB63_BAD1E84C1790__INCLUDED_)

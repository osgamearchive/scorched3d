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

#if !defined(AFX_WEAPON_H__65439E20_84A6_406A_8FD0_045A3E7555D3__INCLUDED_)
#define AFX_WEAPON_H__65439E20_84A6_406A_8FD0_045A3E7555D3__INCLUDED_

#pragma warning(disable: 4786)

#include <weapons/Accessory.h>
#include <coms/NetBuffer.h>
#include <common/Defines.h>
#include <common/Vector.h>

class Action;
class Weapon : public Accessory
{
public:
	Weapon();
	virtual ~Weapon();

	virtual bool parseXML(XMLNode *accessoryNode);
	virtual bool writeAccessory(NetBuffer &buffer);
	virtual bool readAccessory(NetBufferReader &reader);
	
	virtual Action *fireWeapon(unsigned int playerId, 
		Vector &position, Vector &velocity) = 0;

	virtual int getDeathAnimationWeight() { return deathAnimationWeight_; }
	virtual const char *getExplosionTexture();
	virtual const char *getFiredSound();
	virtual const char *getExplosionSound();

	static bool write(NetBuffer &buffer, Weapon *weapon);
	static Weapon *read(NetBufferReader &reader);

protected:
	int deathAnimationWeight_;
};

#endif // !defined(AFX_WEAPON_H__65439E20_84A6_406A_8FD0_045A3E7555D3__INCLUDED_)

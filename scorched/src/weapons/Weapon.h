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

#include <weapons/AccessoryPart.h>
#include <3dsparse/ModelID.h>
#include <coms/NetBuffer.h>
#include <common/Defines.h>
#include <common/Vector.h>
#include <engine/ScorchedContext.h>

class Action;
class Weapon : public AccessoryPart
{
public:
	enum DataEnum
	{
		eDataDeathAnimation = 1
	};

	Weapon();
	virtual ~Weapon();

	virtual bool parseXML(OptionsGame &context, 
		AccessoryStore *store, XMLNode *accessoryNode);
	
	virtual void fireWeapon(ScorchedContext &context,
		unsigned int playerId, 
		Vector &position, Vector &velocity,
		unsigned int data = 0) = 0;

	int getArmsLevel();
	float getScale() { return scale_; }
	bool getMuzzleFlash() { return muzzleFlash_; }
	ModelID &getModelID();

protected:
	int armsLevel_;
	float scale_;
	bool muzzleFlash_;
	ModelID modelId_;

};

#endif // !defined(AFX_WEAPON_H__65439E20_84A6_406A_8FD0_045A3E7555D3__INCLUDED_)

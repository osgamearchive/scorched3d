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


// Shield.h: interface for the Shield class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHIELD_H__F9BCDF39_FB62_4BB4_9D64_C70215669F9C__INCLUDED_)
#define AFX_SHIELD_H__F9BCDF39_FB62_4BB4_9D64_C70215669F9C__INCLUDED_

#include <weapons/Accessory.h>
#include <common/Vector.h>

class Shield : public Accessory
{
public:
	enum ShieldSize
	{
		ShieldSizeSmall,
		ShieldSizeLarge
	};
	enum ShieldType
	{
		ShieldTypeNormal,
		ShieldTypeReflective,
		ShieldTypeReflectiveMag
	};

	Shield();
	virtual ~Shield();

	virtual bool parseXML(XMLNode *accessoryNode);
	virtual bool writeAccessory(NetBuffer &buffer);
	virtual bool readAccessory(NetBufferReader &reader);

	const char *getCollisionSound();

	// Shield attributes
	float getHitRemovePower();
	ShieldSize getRadius() { return radius_; }
	Vector &getColor() { return color_; }
	virtual ShieldType getShieldType();

	REGISTER_ACCESSORY_HEADER(Shield, Accessory::AccessoryShield);

protected:
	std::string collisionSound_;
	ShieldSize radius_;
	Vector color_;
	float removePower_;
};

#endif // !defined(AFX_SHIELD_H__F9BCDF39_FB62_4BB4_9D64_C70215669F9C__INCLUDED_)

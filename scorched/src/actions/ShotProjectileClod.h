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


// ShotClod.h: interface for the ShotClod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOTCLOD_H__F4084AD2_0254_4D77_B43F_3C887111A46F__INCLUDED_)
#define AFX_SHOTCLOD_H__F4084AD2_0254_4D77_B43F_3C887111A46F__INCLUDED_

#include <actions/ShotProjectile.h>
#include <weapons/Weapon.h>

class ShotProjectileClod : public ShotProjectile 
{
public:
	ShotProjectileClod();
	ShotProjectileClod(
		Vector &startPosition, Vector &velocity,
		Weapon *weapon, unsigned int playerId, float width);
	virtual ~ShotProjectileClod();

	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);
	virtual void collision(Vector &position);

	REGISTER_ACTION_HEADER(ShotProjectileClod);

protected:
	float width_;

};

#endif // !defined(AFX_SHOTCLOD_H__F4084AD2_0254_4D77_B43F_3C887111A46F__INCLUDED_)

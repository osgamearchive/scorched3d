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


// ShotFunky.h: interface for the ShotFunky class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOTFUNKY_H__B3E18145_BFA2_469C_85D5_73C0E03445FC__INCLUDED_)
#define AFX_SHOTFUNKY_H__B3E18145_BFA2_469C_85D5_73C0E03445FC__INCLUDED_

#include <actions/ShotProjectile.h>

class ShotProjectileAimedOver : public ShotProjectile
{
public:
	ShotProjectileAimedOver();
	ShotProjectileAimedOver(
		Vector &startPosition, Vector &velocity,
		Weapon *weapon, unsigned int playerId);
	virtual ~ShotProjectileAimedOver();

	virtual void collision(Vector &position);

	REGISTER_ACTION_HEADER(ShotProjectileAimedOver);
};

#endif // !defined(AFX_SHOTFUNKY_H__B3E18145_BFA2_469C_85D5_73C0E03445FC__INCLUDED_)

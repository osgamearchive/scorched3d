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

#if !defined(AFX_TANKCONTROLLER_H__3B055B6D_C8E7_42C7_9D29_FBBFD88457D1__INCLUDED_)
#define AFX_TANKCONTROLLER_H__3B055B6D_C8E7_42C7_9D29_FBBFD88457D1__INCLUDED_

#include <engine/ScorchedContext.h>
#include <common/Vector.h>

class Tank;
class Weapon;
class TankController
{
public:
	static void explosion(ScorchedContext &context,
		Weapon *weapon, unsigned int firer, 
		Vector &position, float radius,
		float damageAmount);
	static void damageTank(ScorchedContext &context,
		Tank *tank, 
		Weapon *weapon,
		unsigned int firer,
		float damage,
		bool useShieldDamage = true);

private:
	TankController();
	virtual ~TankController();

};

#endif // !defined(AFX_TANKCONTROLLER_H__3B055B6D_C8E7_42C7_9D29_FBBFD88457D1__INCLUDED_)

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


#if !defined(__INCLUDE_TankDamageh_INCLUDE__)
#define __INCLUDE_TankDamageh_INCLUDE__

#include <engine/ActionMeta.h>
#include <weapons/Weapon.h>

class TankDamage : public ActionMeta
{
public:
	TankDamage();
	TankDamage(Weapon *weapon, 
		unsigned int damagedPlayerId, unsigned int firedPlayerId,
		float damage, bool useShieldDamage,
		unsigned int data);
	virtual ~TankDamage();

	virtual void init();
	virtual void simulate(float frameTime, bool &remove);
	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);

	REGISTER_ACTION_HEADER(TankDamage);

protected:
	bool firstTime_;
	
	Weapon *weapon_;
	unsigned int damagedPlayerId_;
	unsigned int firedPlayerId_;
	float damage_;
	bool useShieldDamage_;
	unsigned int data_;
};


#endif

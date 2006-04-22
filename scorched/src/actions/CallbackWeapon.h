////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#if !defined(__INCLUDE_CallbackWeaponh_INCLUDE__)
#define __INCLUDE_CallbackWeaponh_INCLUDE__

#include <weapons/Weapon.h>
#include <engine/ActionMeta.h>

class WeaponCallback : public Weapon
{
public:
	virtual void weaponCallback(
		ScorchedContext &context,
		unsigned int playerId, Vector &position, Vector &velocity,
		unsigned int data,
		unsigned int userData) = 0;
};

class CallbackWeapon : public ActionMeta
{
public:
	CallbackWeapon();
	CallbackWeapon(WeaponCallback *callback,
		float delay, unsigned int callbackData,
		unsigned int playerId, Vector &position, Vector &velocity,
    	unsigned int data);
	virtual ~CallbackWeapon();

	virtual void init();
	virtual void simulate(float frameTime, bool &remove);
	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);

	REGISTER_ACTION_HEADER(CallbackWeapon);

protected:
	float totalTime_;

	Vector position_;
	Vector velocity_;
	unsigned int playerId_;
	unsigned int data_;
	float delay_;
	unsigned int callbackData_;
	WeaponCallback *callback_;

};

#endif // __INCLUDE_CallbackWeaponh_INCLUDE__

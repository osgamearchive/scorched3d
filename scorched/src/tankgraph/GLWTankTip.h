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

#ifndef _gLWTankTip_h
#define _gLWTankTip_h

#include <GLW/GLWToolTip.h>
#include <tank/Tank.h>

class GLWTankWeapon : public GLWTip
{
public:
	GLWTankWeapon(Tank *tank);
	virtual ~GLWTankWeapon();

	virtual void populate();
protected:
	Tank *tank_;
};

class GLWTankPower : public GLWTip
{
public:
	GLWTankPower(Tank *tank);
	virtual ~GLWTankPower();

	virtual void populate();
protected:
	Tank *tank_;
};

class GLWTankRotation : public GLWTip
{
public:
	GLWTankRotation(Tank *tank);
	virtual ~GLWTankRotation();

	virtual void populate();
protected:
	Tank *tank_;
};

class GLWTankElevation : public GLWTip
{
public:
	GLWTankElevation(Tank *tank);
	virtual ~GLWTankElevation();

	virtual void populate();
protected:
	Tank *tank_;
};

class GLWTankTip : public GLWTip
{
public:
	GLWTankTip(Tank *tank);
	virtual ~GLWTankTip();

	virtual void populate();
protected:
	Tank *tank_;
};

class GLWTankTips 
{
public:
	GLWTankTips(Tank *tank);
	virtual ~GLWTankTips();

	GLWTankTip tankTip;
	GLWTip nameTip;
	GLWTankWeapon weaponTip;
	GLWTankPower powerTip;
	GLWTankRotation rotationTip;
	GLWTankElevation elevationTip;

private:
	GLWTankTips(const GLWTankTips &);
	const GLWTankTips & operator=(const GLWTankTips &);
};

#endif // _gLWTankTip_h


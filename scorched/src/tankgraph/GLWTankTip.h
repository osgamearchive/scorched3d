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

class TankFuelTip : public GLWTip
{
public:
	TankFuelTip(Tank *tank);
	virtual ~TankFuelTip();

	virtual void populate();
protected:
	Tank *tank_;
};

class TankBatteryTip : public GLWTip
{
public:
	TankBatteryTip(Tank *tank);
	virtual ~TankBatteryTip();

	virtual void populate();
protected:
	Tank *tank_;
};

class TankShieldTip : public GLWTip
{
public:
	TankShieldTip(Tank *tank);
	virtual ~TankShieldTip();

	virtual void populate();
protected:
	Tank *tank_;
};

class TankHealthTip : public GLWTip
{
public:
	TankHealthTip(Tank *tank);
	virtual ~TankHealthTip();

	virtual void populate();
protected:
	Tank *tank_;
};

class TankParachutesTip : public GLWTip
{
public:
	TankParachutesTip(Tank *tank);
	virtual ~TankParachutesTip();

	virtual void populate();
protected:
	Tank *tank_;
};

class TankAutoDefenseTip : public GLWTip
{
public:
	TankAutoDefenseTip(Tank *tank);
	virtual ~TankAutoDefenseTip();

	virtual void populate();
protected:
	Tank *tank_;
};

class TankWeaponTip : public GLWTip
{
public:
	TankWeaponTip(Tank *tank);
	virtual ~TankWeaponTip();

	virtual void populate();
protected:
	Tank *tank_;
};

class TankPowerTip : public GLWTip
{
public:
	TankPowerTip(Tank *tank);
	virtual ~TankPowerTip();

	virtual void populate();
protected:
	Tank *tank_;
};

class TankRotationTip : public GLWTip
{
public:
	TankRotationTip(Tank *tank);
	virtual ~TankRotationTip();

	virtual void populate();
protected:
	Tank *tank_;
};

class TankElevationTip : public GLWTip
{
public:
	TankElevationTip(Tank *tank);
	virtual ~TankElevationTip();

	virtual void populate();
protected:
	Tank *tank_;
};

class TankTip : public GLWTip
{
public:
	TankTip(Tank *tank);
	virtual ~TankTip();

	virtual void populate();
protected:
	Tank *tank_;
};

class GLWTankTips 
{
public:
	GLWTankTips(Tank *tank);
	virtual ~GLWTankTips();

	TankTip tankTip;
	GLWTip nameTip;
	TankWeaponTip weaponTip;
	TankPowerTip powerTip;
	TankRotationTip rotationTip;
	TankElevationTip elevationTip;
	TankAutoDefenseTip autodTip;
	TankParachutesTip paraTip;
	TankHealthTip healthTip;
	TankShieldTip shieldTip;
	TankBatteryTip batteryTip;
	TankFuelTip fuelTip;

private:
	GLWTankTips(const GLWTankTips &);
	const GLWTankTips & operator=(const GLWTankTips &);
};

#endif // _gLWTankTip_h


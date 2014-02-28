////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2013
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
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#if !defined(__INCLUDE_UITankRendererh_INCLUDE__)
#define __INCLUDE_UITankRendererh_INCLUDE__

#include <uiactions/UITargetRenderer.h>
#include <uiactions/UITankShotHistory.h>
#include <uiactions/UITankWeapon.h>
#include <tank/Tank.h>

class UITankActiveModel;
class UITankRenderer : public UITargetRenderer
{
public:
	UITankRenderer(Tank *tank);
	virtual ~UITankRenderer();

	UITankShotHistory &getShotHistory();
	UITankWeapon &getTankWeapon();
	UITankActiveModel *getActiveModel();
	void updateRotation();
	void setActive();
	void setInactive();

protected:
	UITankActiveModel *activeModel_;
	UITankShotHistory shotHistory_;
	UITankWeapon tankWeapon_;
	
	virtual UITargetModel *createModel();
};

#endif

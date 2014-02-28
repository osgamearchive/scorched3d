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

#if !defined(__INCLUDE_UITargetRendererh_INCLUDE__)
#define __INCLUDE_UITargetRendererh_INCLUDE__

#include <client/ClientUISync.h>
#include <target/TargetRenderer.h>
#include <target/Target.h>

class UITargetRenderer : public ClientUISyncAction, public TargetRenderer
{
public:
	UITargetRenderer(Target *target);
	virtual ~UITargetRenderer();

	// ClientUISyncAction (UI and Client Thread)
	virtual void performUIAction();

	// TargetRenderer (Client Thread)
	virtual void moved();
	virtual void targetBurnt();
	virtual void shieldHit();
	virtual void fired();

protected:
	Ogre::SceneNode* targetNode_;
	Ogre::Entity* targetEntity_;
	int registered_;
	Target *target_;

	void create();
};

#endif
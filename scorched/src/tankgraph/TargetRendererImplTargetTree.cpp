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

#include <tankgraph/TargetRendererImplTargetTree.h>
#include <landscape/Landscape.h>
#include <landscape/ShadowMap.h>
#include <3dsparse/ModelStore.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLViewPort.h>
#include <GLEXT/GLState.h>
#include <GLW/GLWToolTip.h>
#include <dialogs/TutorialDialog.h>

TargetRendererImplTargetTree::TargetRendererImplTargetTree(Target *target, 
	const char *tree, float size, float color, bool useSnow) :
	target_(target),
	burnt_(false)
{
}

TargetRendererImplTargetTree::~TargetRendererImplTargetTree()
{
}

void TargetRendererImplTargetTree::simulate(float frameTime)
{
}

void TargetRendererImplTargetTree::draw(float distance)
{
}

void TargetRendererImplTargetTree::draw2d()
{
}

void TargetRendererImplTargetTree::drawSecond(float distance)
{
}

void TargetRendererImplTargetTree::shieldHit()
{
}

void TargetRendererImplTargetTree::fired()
{
}

void TargetRendererImplTargetTree::targetBurnt()
{
	burnt_ = true;
}
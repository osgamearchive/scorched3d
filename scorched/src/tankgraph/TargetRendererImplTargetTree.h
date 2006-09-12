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

#if !defined(__INCLUDE_TargetRendererImplTargetTreeh_INCLUDE__)
#define __INCLUDE_TargetRendererImplTargetTreeh_INCLUDE__

#include <tankgraph/TargetRendererImplTarget.h>

class TargetRendererImplTargetTree : 
	public TargetRendererImplTarget
{
public:
	TargetRendererImplTargetTree(Target *target, 
		const char *tree, float size, float color, bool useSnow);
	virtual ~TargetRendererImplTargetTree();

	virtual void simulate(float frameTime);
	virtual void draw(float distance);
	virtual void drawSecond(float distance);
	virtual void draw2d();
	virtual void shieldHit();
	virtual void fired();
	virtual void targetBurnt();

	virtual Vector &getPosition() { return target_->getTargetPosition(); }

protected:
	Target *target_;
	bool burnt_;
};

#endif // __INCLUDE_TargetRendererImplTargetTreeh_INCLUDE__

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

#if !defined(__INCLUDE_TargetModelRendererh_INCLUDE__)
#define __INCLUDE_TargetModelRendererh_INCLUDE__

#include <tankgraph/TargetModelIdRenderer.h>
#include <3dsparse/ModelRenderer.h>
#include <target/Target.h>

class TargetModelRenderer : public TargetModelIdRenderer
{
public:
	TargetModelRenderer(Target *target);
	virtual ~TargetModelRenderer();

	virtual void simulate(float frameTime);
	virtual void draw();
	virtual void drawSecond();
	virtual void draw2d();

protected:
	Target *target_;
	ModelRenderer *modelRenderer_;
	bool canSeeTank_;
};

#endif // __INCLUDE_TargetModelRendererh_INCLUDE__

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

#if !defined(__INCLUDE_TargetRendererImplTargeth_INCLUDE__)
#define __INCLUDE_TargetRendererImplTargeth_INCLUDE__

#include <tankgraph/TargetRendererImpl.h>
#include <tankgraph/GLWTankTip.h>
#include <3dsparse/ModelRenderer.h>
#include <3dsparse/ModelID.h>
#include <landscape/LandscapeObjectsEntry.h>

class TargetRendererImplTarget : 
	public TargetRendererImpl,
	public LandscapeObjectEntryBase
{
public:
	TargetRendererImplTarget(Target *target, 
		ModelID model, ModelID burntModel,
		float scale);
	virtual ~TargetRendererImplTarget();

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
	ModelRenderer *modelRenderer_;
	ModelRenderer *burntModelRenderer_;
	bool canSeeTank_;
	bool burnt_;
	float shieldHit_, totalTime_;
	float scale_;
	double posX_, posY_, posZ_;
	GLWTargetTips targetTips_;

	void storeTank2DPos();
};

#endif // __INCLUDE_TargetRendererImplTargeth_INCLUDE__

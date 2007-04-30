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

#if !defined(__INCLUDE_TargetRendererImplTargetModelh_INCLUDE__)
#define __INCLUDE_TargetRendererImplTargetModelh_INCLUDE__

#include <tankgraph/TargetRendererImplTarget.h>
#include <GLW/GLWTankTip.h>
#include <common/ModelID.h>
#include <graph/ModelRendererSimulator.h>

class TargetRendererImplTargetModel : 
	public TargetRendererImplTarget
{
public:
	TargetRendererImplTargetModel(Target *target, 
		ModelID model, ModelID burntModel,
		float scale, float color);
	virtual ~TargetRendererImplTargetModel();

	virtual void simulate(float frameTime);
	virtual void draw(float distance);
	virtual void drawSecond(float distance);
	virtual void draw2d();
	virtual void shieldHit();
	virtual void fired();
	virtual void targetBurnt();

	GLWTargetTips &getTips() { return targetTips_; }

protected:
	Target *target_;
	ModelRendererSimulator *modelRenderer_;
	ModelRendererSimulator *burntModelRenderer_;
	bool canSeeTank_;
	bool burnt_;
	float shieldHit_, totalTime_;
	float scale_, color_;
	double posX_, posY_, posZ_;
	GLWTargetTips targetTips_;

	void storeTank2DPos();
};

#endif // __INCLUDE_TargetRendererImplTargetModelh_INCLUDE__

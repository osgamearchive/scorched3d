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

#if !defined(__INCLUDE_TreeModelRendererh_INCLUDE__)
#define __INCLUDE_TreeModelRendererh_INCLUDE__

#include <tankgraph/TargetModelIdRenderer.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLTexture.h>
#include <target/Target.h>

class TreeModelRenderer : public TargetModelIdRenderer
{
public:
	TreeModelRenderer(Target *target);
	virtual ~TreeModelRenderer();

	virtual void simulate(float frameTime);
	virtual void draw(float distance);
	virtual void drawSecond(float distance);
	virtual void draw2d();
	virtual void shieldHit();

protected:
	Target *target_;
	static GLuint treePine, treePineBurnt, treePineSnow;
	static GLuint treePineSmall, treePineBurntSmall, treePineSnowSmall;
	static GLuint treePalm, treePalmBurnt;
	static GLuint treePalmSmall, treePalmBurntSmall;
	static GLTexture texture_;

	bool pine;
	bool snow;
	bool burnt;
	float rotation;
	float size;
	Vector color;
};

#endif // __INCLUDE_TreeModelRendererh_INCLUDE__

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

#if !defined(__INCLUDE_ExplosionRendererh_INCLUDE__)
#define __INCLUDE_ExplosionRendererh_INCLUDE__

#include <GLEXT/GLTextureSet.h>
#include <GLEXT/GLBilboardRenderer.h>
#include <common/Vector.h>
#include <engine/Action.h>

class ExplosionRenderer : public ActionRenderer
{
public:
	ExplosionRenderer(Vector &position, GLTextureSet &textureSet, 
		Vector &color,
		float width, bool weapon);
	virtual ~ExplosionRenderer();

	virtual void simulate(Action *action, float frameTime, bool &remove);
	virtual void draw(Action *action);

protected:
	class ExplosionSubPart
	{
	public:
		ExplosionSubPart();
		virtual ~ExplosionSubPart();

		void simulate(float frametime);
		void draw(	Vector &center, Vector &color, float w, float opacity1, 
			float opacity2, GLTexture *t1, GLTexture *t2);

		GLBilboardRenderer::GLBilboardOrderedEntry *graphicEntry1_;
		GLBilboardRenderer::GLBilboardOrderedEntry *graphicEntry2_;
	protected:
		Vector rotation;
		Vector position;
		Vector positionMove;
	};

	class ExplosionMainPart
	{
	public:
		ExplosionMainPart();
		~ExplosionMainPart();

		void simulate(float frametime);
		void draw(Vector &center, Vector &color, float w, float opacity1, 
			float opacity2, GLTexture *t1, GLTexture *t2);

	protected:
		Vector position;
		Vector positionMove;

		ExplosionSubPart *subParts;
	};

	Vector centrePosition_;
	ExplosionMainPart *mainParts;

	bool weapon_;
	float width_;
	float currentWidth_;
	GLTextureSet &textureSet_;
	Vector color_;

	void drawExplosion();

};

#endif

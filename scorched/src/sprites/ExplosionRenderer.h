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
#include <common/Vector.h>
#include <engine/Action.h>

class ExplosionRenderer : public ActionRenderer
{
public:
	ExplosionRenderer(Vector &position, GLTextureSet &textureSet, 
		float width, bool weapon);
	virtual ~ExplosionRenderer();

	virtual void simulate(Action *action, float frameTime, bool &remove);
	virtual void draw(Action *action);

protected:
	class ExplosionSubPart
	{
	public:
		ExplosionSubPart();

		void simulate(float frametime);
		void draw(Vector &bilX, Vector &bilY);

	protected:
		enum ExplosionTypeEnum
		{
			texOne = 0,
			texTwo = 1,
			texThree = 2,
			texFour = 3
		};

		Vector rotation;
		Vector position;
		Vector positionMove;
		ExplosionTypeEnum type;
	};

	class ExplosionMainPart
	{
	public:
		ExplosionMainPart();
		~ExplosionMainPart();

		void simulate(float frametime);
		void draw(Vector &center, Vector &bilX, Vector &bilY);

	protected:
		Vector position;
		Vector positionMove;

		ExplosionSubPart *subParts;
	};

	Vector centrePosition_;
	ExplosionMainPart *mainParts;

	bool firstTime_; 
	bool weapon_;
	float width_;
	float currentWidth_;
	GLTextureSet &textureSet_;

	void drawExplosion();

};

#endif

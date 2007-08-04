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

#if !defined(__INCLUDE_Lightningh_INCLUDE__)
#define __INCLUDE_Lightningh_INCLUDE__

#include <engine/ActionReferenced.h>
#include <weapons/WeaponLightning.h>
#include <list>

class GLTexture;
class Target;
class Lightning : public ActionReferenced
{
public:
	Lightning(WeaponLightning *weapon,
		WeaponFireContext &weaponContext,
		Vector &position, Vector &velocity);
	virtual ~Lightning();

	virtual void draw();
	virtual void init();
	virtual void simulate(float frameTime, bool &remove);

protected:
	struct Segment
	{
		Vector start;
		Vector end;
		Vector direction;
		bool endsegment;
		float size;
	};
	bool firstTime_;
	float totalTime_;
	std::list<Segment> segments_;

	GLTexture *texture_;
	WeaponLightning *weapon_;
	Vector position_;
	Vector velocity_;
	WeaponFireContext weaponContext_;

	void damageTargets(Vector &position, 
		std::map<unsigned int, float> &hurtMap);
	void dispaceDirection(Vector &direction, 
		Vector &originalDirection, float angle);
	void generateLightning(int id, int depth, float size, 
		Vector &originalPosition, Vector &originalDirection,
		Vector &position, Vector &direction,
		std::map<unsigned int, float> &hurtMap);
};

#endif // __INCLUDE_Lightningh_INCLUDE__

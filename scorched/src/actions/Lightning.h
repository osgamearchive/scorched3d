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

#include <engine/ActionMeta.h>
#include <weapons/WeaponLightning.h>
#include <common/RandomGenerator.h>
#include <list>

class Tank;
class Lightning : public ActionMeta
{
public:
	Lightning();
	Lightning(WeaponLightning *weapon,
		unsigned int playerId, 
		Vector &position, Vector &velocity,
		unsigned int data);
	virtual ~Lightning();

	virtual void draw();
	virtual void init();
	virtual void simulate(float frameTime, bool &remove);
	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);

	REGISTER_ACTION_HEADER(Lightning);

protected:
	struct Segment
	{
		Vector start;
		Vector end;
		Vector direction;
		bool endsegment;
		float size;
	};
	float totalTime_;
	std::list<Segment> segments_;
	RandomGenerator generator_;

	WeaponLightning *weapon_;
	Vector position_;
	Vector velocity_;
	unsigned int seed_;
	unsigned int playerId_;
	unsigned int data_;

	void damageTanks(Vector &position, 
		std::map<Tank *, float> &hurtMap);
	void dispaceDirection(Vector &direction, 
		Vector &originalDirection, float angle);
	void generateLightning(int id, int depth, float size, 
		Vector &originalPosition, Vector &originalDirection,
		Vector &position, Vector &direction,
		std::map<Tank *, float> &hurtMap);
};

#endif // __INCLUDE_Lightningh_INCLUDE__

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

#if !defined(__INCLUDE_ScorchedCollisionHandlerh_INCLUDE__)
#define __INCLUDE_ScorchedCollisionHandlerh_INCLUDE__

#include <engine/ScorchedContext.h>
#include <engine/ScorchedCollisionIds.h>
#include <common/Vector.h>
#include <tank/Tank.h>
#include <weapons/Shield.h>

class PhysicsParticleMeta;
class ScorchedCollisionHandler : public PhysicsEngineCollision
{
public:
	ScorchedCollisionHandler(ScorchedContext *context);
	virtual ~ScorchedCollisionHandler();

	virtual void collision(dGeomID o1, dGeomID o2, 
		dContactGeom *contacts, int noContacts);

protected:
	static ScorchedCollisionHandler *instance_;

	enum ParticleAction
	{
		ParticleActionBounce,
		ParticleActionFinished,
		ParticalActionWarp,
		ParticleActionNone
	};
	ScorchedContext *context_;

	void groundCollision(dGeomID o1, dGeomID o2, 
		dContactGeom *contacts, int noContacts,
		bool metaAction);
	void bounceCollision(dGeomID o1, dGeomID o2, 
		dContactGeom *contacts, int noContacts);
	void shotCollision(dGeomID o1, dGeomID o2, 
		dContactGeom *contacts, int noContacts);
	void collisionBounce(dGeomID o1, dGeomID o2, 
		dContactGeom *contacts, int noContacts, 
		double bvel = 0.0, double bfactor = 1.0);

	ScorchedCollisionHandler::ParticleAction collisionShield(
		unsigned int shotId,
		unsigned int hitId,
		Vector &collisionPos,
		Shield::ShieldSize size,
		PhysicsParticleMeta *shot,
		float hitPercentage,
		float &deflectPower);

};

#endif

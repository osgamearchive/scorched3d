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


// PhysicsParticle.h: interface for the PhysicsParticle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PhysicsParticle_H__53A44445_C8C9_4108_B5E1_744C0AF7A1D5__INCLUDED_)
#define AFX_PhysicsParticle_H__53A44445_C8C9_4108_B5E1_744C0AF7A1D5__INCLUDED_

// Both these classes are the same except for the inheritance
// Need to find a better way to do this

#include <engine/ActionMeta.h>
#include <common/Vector.h>
#include <engine/PhysicsParticleObject.h>

class PhysicsParticle : public Action
{
public:
	PhysicsParticle();
	virtual ~PhysicsParticle();

	virtual void setPhysics(Vector &position, Vector &velocity);
	virtual void collision(Vector &position);

	Vector &getCurrentPosition();
	Vector &getCurrentVelocity();
	void setData(void *data);
	void setCurrentPosition(Vector &position);

	// Inherited from action
	virtual void simulate(float timepassed, bool &remove);

protected:
	PhysicsParticleObject physicsObject_;
	bool collision_;
	float totalActionTime_;

};

class PhysicsParticleMeta : public ActionMeta
{
public:
	PhysicsParticleMeta();
	virtual ~PhysicsParticleMeta();

	virtual void setPhysics(Vector &position, Vector &velocity,
		float sphereSize = 0.0f, float sphereDensity = 0.0f);
	virtual void collision(Vector &position);

	Vector &getCurrentPosition();
	Vector &getCurrentVelocity();
	void setData(void *data);
	void setCurrentPosition(Vector &position);

	void applyForce(Vector &force);

	// Inherited from action
	virtual void simulate(float timepassed, bool &remove);

protected:
	PhysicsParticleObject physicsObject_;
	bool collision_;
	float totalActionTime_;

};

#endif // !defined(AFX_PhysicsParticle_H__53A44445_C8C9_4108_B5E1_744C0AF7A1D5__INCLUDED_)

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

#include <common/Defines.h>
#include <engine/ParticleEmitter.h>
#include <math.h>
#include <stdlib.h>

static void randomVector(Vector &result, Vector &start, Vector &end)
{
	result[0] = start[0] + ((end[0] - start[0]) * RAND);
	result[1] = start[1] + ((end[1] - start[1]) * RAND);
	result[2] = start[2] + ((end[2] - start[2]) * RAND);
}

static void randomCounterVector(Vector &result, Vector &current, 
								Vector &start, Vector &end,
								float life)
{
	randomVector(result, start, end);
	result -= current;
	result /= life;
}

static float randomScalar(float start, float end)
{
	return start + ((end - start) * RAND);
}

static float randomCounterScalar(float current, 
								float start, float end,
								float life)
{
	float result = randomScalar(start, end);
	result -= current;
	result /= life;
	return result;
}

ParticleEmitter::ParticleEmitter()
{
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::setLife(float life1, float life2)
{
	life1_ = life1; life2_ = life2;
}

void ParticleEmitter::setMass(float mass1, float mass2)
{
	mass1_ = mass1; mass2_ = mass2;
}

void ParticleEmitter::setFriction(float friction1, float friction2)
{
	friction1_ = friction1; friction2_ = friction2;
}
	
void ParticleEmitter::setStartColor(
		Vector startColor1, float startAlpha1,
		Vector startColor2, float startAlpha2)
{
	startAlpha1_ = startAlpha1; endAlpha1_ = startAlpha1; 
	startAlpha2_ = startAlpha2; endAlpha2_ = startAlpha2;
	startColor1_ = startColor1; endColor1_ = startColor1;
	startColor2_ = startColor2; endColor2_ = startColor2;
}

void ParticleEmitter::setEndColor(
		Vector endColor1, float endAlpha1,
		Vector endColor2, float endAlpha2)
{
	endColor1_ = endColor1; endAlpha1_ = endAlpha1;
	endColor2_ = endColor2; endAlpha2_ = endAlpha2;
}

void ParticleEmitter::setVelocity(Vector velocity1, Vector velocity2)
{
	velocity1_ = velocity1; velocity2_ = velocity2;
}

void ParticleEmitter::setStartSize(
		float startX1, float startY1,
		float startX2, float startY2)
{
	startSize1_[0] = startX1; startSize1_[1] = startY1;
	startSize2_[0] = startX2; startSize2_[1] = startY2;
}

void ParticleEmitter::setEndSize(
		float endX1, float endY1,
		float endX2, float endY2)
{
	endSize1_[0] = endX1; endSize1_[1] = endY1;
	endSize2_[0] = endX2; endSize2_[1] = endY2;
}

void ParticleEmitter::setGravity(Vector gravity)
{
	gravity_ = gravity;
}

void ParticleEmitter::setAttributes(
	float life1, float life2,
	float mass1, float mass2,
	float friction1, float friction2,
	Vector velocity1, Vector velocity2,
	Vector startColor1, float startAlpha1,
	Vector startColor2, float startAlpha2,
	Vector endColor1, float endAlpha1,
	Vector endColor2, float endAlpha2,
	float startX1, float startY1,
	float startX2, float startY2,
	float endX1, float endY1,
	float endX2, float endY2,
	Vector gravity)
{
	setLife(life1, life2);
	setMass(mass1, mass2);
	setFriction(friction1, friction2);
	setVelocity(velocity1, velocity2);
	setStartColor(startColor1, startAlpha1,
		startColor2, startAlpha2);
	setEndColor(endColor1, endAlpha1,
		endColor2, endAlpha2);
	setStartSize(startX1, startY1,
		startX2, startY2);
	setEndSize(endX1, endY1,
		endX2, endY2);
	setGravity(gravity);
}

void ParticleEmitter::createDefaultParticle(Particle &particle)
{
	float life = randomScalar(life1_, life2_);
	float mass = randomScalar(mass1_, mass2_);
	float friction = randomScalar(friction1_, friction2_);
	float alpha = randomScalar(startAlpha1_, startAlpha2_);
	float alphac = randomCounterScalar(alpha, endAlpha1_, endAlpha2_, life);

	Vector velocity;
	randomVector(velocity, velocity1_, velocity2_);
	Vector color, colorc;
	randomVector(color, startColor1_, startColor2_);
	randomCounterVector(colorc, color, endColor1_, endColor2_, life);
	Vector size, sizec;
	randomVector(size, startSize1_, startSize2_);
	randomCounterVector(sizec, size, endSize1_, endSize2_, life);

	particle.setParticle(life, mass, 
		friction, velocity, gravity_,
		color, colorc, 
		size, sizec, 
		alpha, alphac);
}

void ParticleEmitter::emitLinear(int number, 
	Vector &position1, Vector &position2,
	ParticleEngine &engine,
	ParticleRenderer *renderer)
{
	for (int i=0; i<number; i++)
	{
		Particle *particle = engine.getNextAliveParticle();
		if (!particle) return;

		Vector position;
		randomVector(position, position1, position2);

		createDefaultParticle(*particle);
		particle->position_ = position;
		particle->renderer_ = renderer;
	}
}

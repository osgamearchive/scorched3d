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

#include <engine/ParticleEngine.h>
#include <GLEXT/GLState.h>

ParticleEngine::ParticleEngine(unsigned int maxParticles) :
	particlesOnScreen_(0), particles_(0), 
	freeParticles_(0), 
	speed_(1.0f)
{
	setMaxParticles(maxParticles);
}

ParticleEngine::~ParticleEngine()
{
	delete [] particles_;
	delete [] freeParticles_;
	particles_ = 0;
	freeParticles_ = 0;
}

void ParticleEngine::setMaxParticles(unsigned int maxParticles)
{
	maxParticles_ = maxParticles;
	delete [] particles_;
	delete [] freeParticles_;
	freeParticles_ = new Particle*[maxParticles];
	particles_  = new Particle[maxParticles];

	killAll();
}

unsigned int ParticleEngine::getMaxParticles()
{
	return maxParticles_;
}

unsigned int ParticleEngine::getParticlesOnScreen()
{
	return particlesOnScreen_;
}

void ParticleEngine::killAll()
{
	for (unsigned int i=0; i<maxParticles_; i++)
	{
		Particle &particle = particles_[i];
		if (particle.life_ > 0.0f) particle.life_ = -1.0f;
		particle.next_ = 0;

		freeParticles_[i] = &particles_[i];
	}
	particlesOnScreen_ = 0;
	start_ = 0;
	end_ = 0;
}

void ParticleEngine::draw(const unsigned state)
{
	GLState glstate(GLState::TEXTURE_ON | GLState::BLEND_ON | GLState::DEPTH_ON);
	glDepthMask(GL_FALSE);
	
	Particle *particle = start_;
	while (particle)
	{
		if (particle->renderer_)
		{
			particle->renderer_->renderParticle(*particle);
		}
		particle = particle->next_;
	}

	glDepthMask(GL_TRUE);
}

void ParticleEngine::simulate(const unsigned state, float time)
{
	if (speed_ != 1.0f) time *= speed_;

	Vector momentum;
	Particle *particle = start_;
	Particle *lastParticle = 0;
	while (particle)
	{
		Particle *next = particle->next_;
		particle->life_ -= time;
		if (particle->life_ > 0.0f)
		{
			momentum = particle->velocity_ * particle->mass_;

			//update the particle's position
			particle->position_ += momentum * time;
			
			//interpolate the color, alpha value, and size
			particle->color_ += particle->colorCounter_ * time;
			particle->alpha_ += particle->alphaCounter_ * time;
			particle->size_ += particle->sizeCounter_ * time;

			//now its time for the external forces to take their toll
			particle->velocity_ *= 1.0f - (particle->friction_ * time);
			particle->velocity_ += particle->gravity_ * time * time;

			// Simulate the particle
			if (particle->renderer_)
			{
				particle->renderer_->simulateParticle(*particle, time);
			}

			lastParticle = particle;
		}
		else
		{
			particlesOnScreen_--;
			freeParticles_[particlesOnScreen_] = particle;
			delete particle->userData_;

			if (end_ == particle) end_ = lastParticle;
			if (start_ == particle) start_ = particle->next_;
			if (lastParticle) lastParticle->next_ = particle->next_;
			particle->next_ = 0;
			particle->userData_ = 0;
		}
		particle = next;
	}
}

Particle *ParticleEngine::getNextAliveParticle()
{
	Particle *particle = 0;
	if (particlesOnScreen_ < maxParticles_)
	{
		particle = freeParticles_[particlesOnScreen_];
		if (end_) end_->next_ = particle;
		end_ = particle;
		if (!start_) start_ = particle;
		particlesOnScreen_ ++;	
	}

	return particle;
}


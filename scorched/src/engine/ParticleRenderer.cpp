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

#include <engine/ParticleRenderer.h>
#include <engine/Particle.h>
#include <sprites/ExplosionTextures.h>
#include <sprites/DebrisActionRenderer.h>
#include <sprites/SmokeActionRenderer.h>
#include <sprites/NapalmRenderer.h>
#include <sprites/ExplosionNukeRenderer.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <client/ScorchedClient.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLInfo.h>

void ParticleRendererPoints::simulateParticle(Particle &particle, float time)
{
}

void ParticleRendererPoints::renderParticle(Particle &particle)
{
	glColor4f(
		particle.color_[0],
		particle.color_[1],
		particle.color_[2], 
		particle.alpha_);
	glBegin(GL_POINTS);
		glVertex3fv(particle.position_);
	glEnd();
}

ParticleRendererQuads *ParticleRendererQuads::getInstance()
{
	static ParticleRendererQuads instance_;
	return &instance_;
}

void ParticleRendererQuads::simulateParticle(Particle &particle, float time)
{
}

void ParticleRendererQuads::renderParticle(Particle &particle)
{
	if (!particle.texture_) return;
	particle.texture_->draw();

	if (particle.additiveTexture_)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	Vector &bilX = GLCameraFrustum::instance()->getBilboardVectorX();
	Vector &bilY = GLCameraFrustum::instance()->getBilboardVectorY();

	float bilXX = bilX[0] * particle.size_[0];
	float bilXY = bilX[1] * particle.size_[0];
	float bilXZ = bilX[2] * particle.size_[0];
	float bilYX = bilY[0] * particle.size_[1];
	float bilYY = bilY[1] * particle.size_[1];
	float bilYZ = bilY[2] * particle.size_[1];

	glColor4f(
		particle.color_[0],
		particle.color_[1],
		particle.color_[2], 
		particle.alpha_);
	glBegin(GL_QUADS);
	switch(particle.textureCoord_)
	{
	default: glTexCoord2d(1.0f, 1.0f); break;
	case 1:  glTexCoord2d(0.0f, 1.0f); break;
	case 2:  glTexCoord2d(0.0f, 0.0f); break;
	case 3:  glTexCoord2d(1.0f, 0.0f); break;
	}
	glVertex3f(
		particle.position_[0] + bilXX + bilYX, 
		particle.position_[1] + bilXY + bilYY, 
		particle.position_[2] + bilXZ + bilYZ);
	switch(particle.textureCoord_)
	{
	default: glTexCoord2d(0.0f, 1.0f); break;
	case 1:  glTexCoord2d(0.0f, 0.0f); break;
	case 2:  glTexCoord2d(1.0f, 0.0f); break;
	case 3:  glTexCoord2d(1.0f, 1.0f); break;
	}
	glVertex3f(
		particle.position_[0] - bilXX + bilYX, 
		particle.position_[1] - bilXY + bilYY, 
		particle.position_[2] - bilXZ + bilYZ);
	switch(particle.textureCoord_)
	{
	default: glTexCoord2d(0.0f, 0.0f); break;
	case 1:  glTexCoord2d(1.0f, 0.0f); break;
	case 2:  glTexCoord2d(1.0f, 1.0f); break;
	case 3:  glTexCoord2d(0.0f, 1.0f); break;
	}
	glVertex3f(
		particle.position_[0] - bilXX - bilYX, 
		particle.position_[1] - bilXY - bilYY, 
		particle.position_[2] - bilXZ - bilYZ);
	switch(particle.textureCoord_)
	{
	default: glTexCoord2d(1.0f, 0.0f); break;
	case 1:  glTexCoord2d(1.0f, 1.0f); break;
	case 2:  glTexCoord2d(0.0f, 1.0f); break;
	case 3:  glTexCoord2d(0.0f, 0.0f); break;
	}
	glVertex3f(
		particle.position_[0] + bilXX - bilYX, 
		particle.position_[1] + bilXY - bilYY, 
		particle.position_[2] + bilXZ - bilYZ);
	
	glEnd();

	if (particle.additiveTexture_)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	// Add a shadow of the smoke on the ground
	float posX = particle.position_[0];
	float posY = particle.position_[1];
	float posZ = particle.position_[2];
	if (particle.shadow_ &&
		posX < ScorchedClient::instance()->getLandscapeMaps().getHMap().getWidth() &&
		posX > 0.0f &&
		posY < ScorchedClient::instance()->getLandscapeMaps().getHMap().getWidth() &&
		posY > 0.0f)
	{
		float aboveGround =
			posZ - ScorchedClient::instance()->getLandscapeMaps().
			getHMap().getHeight(
			int (posX), int(posY));
		float smokeAlpha = particle.alpha_ + .2f; 
		if (smokeAlpha > 1.0f) smokeAlpha = 1.0f;
		Landscape::instance()->getShadowMap().
			addCircle(posX, posY, 
			(particle.size_[0] * aboveGround) / 10.0f, smokeAlpha);
	}

	GLInfo::addNoTriangles(2);
}

ParticleRendererDebris *ParticleRendererDebris::getInstance()
{
	static ParticleRendererDebris instance_;
	return &instance_;
}

void ParticleRendererDebris::renderParticle(Particle &particle)
{
	DebrisActionRenderer *renderer = (DebrisActionRenderer *)
		particle.userData_;
	renderer->draw(particle.position_);
}

void ParticleRendererDebris::simulateParticle(Particle &particle, float time)
{
	DebrisActionRenderer *renderer = (DebrisActionRenderer *)
		particle.userData_;
	renderer->simulate(time);	
}

ParticleRendererSmoke *ParticleRendererSmoke::getInstance()
{
	static ParticleRendererSmoke instance_;
	return &instance_;
}

void ParticleRendererSmoke::renderParticle(Particle &particle)
{
}

void ParticleRendererSmoke::simulateParticle(Particle &particle, float time)
{
	SmokeActionRenderer *renderer = (SmokeActionRenderer *)
		particle.userData_;
}

ParticleRendererNapalm *ParticleRendererNapalm::getInstance()
{
	static ParticleRendererNapalm instance_;
	return &instance_;
}

void ParticleRendererNapalm::renderParticle(Particle &particle)
{
	NapalmRenderer *renderer = (NapalmRenderer *)
		particle.userData_;
	renderer->draw(&particle);
	ParticleRendererQuads::getInstance()->renderParticle(particle);
}

void ParticleRendererNapalm::simulateParticle(Particle &particle, float time)
{
	NapalmRenderer *renderer = (NapalmRenderer *)
		particle.userData_;
	renderer->simulate(&particle, time);
}

ParticleRendererMushroom *ParticleRendererMushroom::getInstance()
{
	static ParticleRendererMushroom instance_;
	return &instance_;
}

void ParticleRendererMushroom::renderParticle(Particle &particle)
{
	ParticleRendererQuads::getInstance()->renderParticle(particle);
}

void ParticleRendererMushroom::simulateParticle(Particle &particle, float time)
{
	ExplosionNukeRendererEntry *renderer = (ExplosionNukeRendererEntry *)
		particle.userData_;
	renderer->simulate(&particle, time);
}


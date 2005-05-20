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

#include <sprites/MissileActionRenderer.h>
#include <sprites/ExplosionTextures.h>
#include <GLEXT/GLCameraFrustum.h>
#include <weapons/Accessory.h>
#include <actions/ShotProjectile.h>
#include <landscape/Landscape.h>
#include <common/OptionsDisplay.h>
#include <tankgraph/TankModelRenderer.h>
#include <tankgraph/TankRenderer.h>
#include <tank/TankContainer.h>
#include <client/ScorchedClient.h>
#include <engine/ScorchedContext.h>
#include <engine/ParticleEngine.h>
#include <sound/Sound.h>

MissileActionRenderer::MissileActionRenderer(int flareType, float scale) : 
	flareType_(flareType), counter_(0.05f, 0.05f), 
	mesh_(0), scale_(scale), rotation_(180.0f),
	flameemitter_(0), smokeemitter_(0), sound_(0)
{
}

MissileActionRenderer::~MissileActionRenderer()
{
	delete flameemitter_;
	delete smokeemitter_;
	delete sound_;
}

void MissileActionRenderer::simulate(Action *action, float timepassed, bool &remove)
{
	ShotProjectile *shot = (ShotProjectile *) action;
	if (!flameemitter_)
	{
		flameemitter_ = new ParticleEmitter;
		flameemitter_->setAttributes(
			shot->getWeapon()->getFlameLife() / 2.0f, shot->getWeapon()->getFlameLife(), // Life
			0.5f, 1.0f, // Mass
			0.01f, 0.02f, // Friction
			Vector(-0.05f, -0.1f, 0.3f), Vector(0.05f, 0.1f, 0.9f), // Velocity
			shot->getWeapon()->getFlameStartColor1(), 0.9f, // StartColor1
			shot->getWeapon()->getFlameStartColor2(), 1.0f, // StartColor2
			shot->getWeapon()->getFlameEndColor1(), 0.0f, // EndColor1
			shot->getWeapon()->getFlameEndColor2(), 0.1f, // EndColor2
			0.2f, 0.2f, 0.5f, 0.5f, // Start Size
			1.5f, 1.5f, 3.0f, 3.0f, // EndSize
			Vector(0.0f, 0.0f, 10.0f), // Gravity
			true,
			true);
	}
	if (!smokeemitter_)
	{
		smokeemitter_ = new ParticleEmitter;
		smokeemitter_->setAttributes(
			shot->getWeapon()->getSmokeLife() / 2.0f, shot->getWeapon()->getSmokeLife(), // Life
			0.2f, 0.5f, // Mass
			0.01f, 0.02f, // Friction
			Vector(-0.05f, -0.1f, 0.3f), Vector(0.05f, 0.1f, 0.9f), // Velocity
			Vector(0.7f, 0.7f, 0.7f), 0.3f, // StartColor1
			Vector(0.7f, 0.7f, 0.7f), 0.3f, // StartColor2
			Vector(0.7f, 0.7f, 0.7f), 0.0f, // EndColor1
			Vector(0.8f, 0.8f, 0.8f), 0.1f, // EndColor2
			0.2f, 0.2f, 0.5f, 0.5f, // Start Size
			2.2f, 2.2f, 4.0f, 4.0f, // EndSize
			Vector(0.0f, 0.0f, 100.0f), // Gravity
			false,
			true);
	}
	if (!sound_)
	{
		const char *engineSound = shot->getWeapon()->getEngineSound();
		if (0 != strcmp("none", engineSound))
		{
			SoundBuffer *rocket = Sound::instance()->fetchOrCreateBuffer(
				(char *) getDataFile(engineSound));
			sound_ = Sound::instance()->createSource();
			sound_->setPosition(shot->getCurrentPosition());
			sound_->setGain(0.25f);
			sound_->play(rocket, true);
		}
	}
	if (sound_)
	{
		sound_->setPosition(shot->getCurrentPosition());
		sound_->setVelocity(shot->getCurrentVelocity());
	}

	Vector &actualPos = shot->getCurrentPosition();
	Vector actualPos1;
	actualPos1[0] = actualPos[0] - 0.25f;
	actualPos1[1] = actualPos[1] - 0.25f;
	actualPos1[2] = actualPos[2] - 0.25f;
	Vector actualPos2;
	actualPos2[0] = actualPos[0] + 0.25f;
	actualPos2[1] = actualPos[1] + 0.25f;
	actualPos2[2] = actualPos[2] + 0.25f;

	// Rotate the shot
	rotation_ += shot->getCurrentVelocity().Magnitude() * 
		shot->getWeapon()->getSpinSpeed();

	// Add flame trail
	if (shot->getWeapon()->getCreateFlame())
	{
		flameemitter_->emitLinear(2, actualPos1, actualPos2, 
			ScorchedClient::instance()->getParticleEngine(), 
			ParticleRendererQuads::getInstance());
	}

	// Add the smoke trail
	if (shot->getWeapon()->getCreateSmoke())
	{
		if (counter_.nextDraw(timepassed))
		{
			Vector vel1 = shot->getCurrentVelocity();
			Vector vel2;
			vel1 *= -0.4f;
			vel2 = vel1 * 0.7f;

			actualPos1 -= shot->getCurrentVelocity() * 0.2f;
			actualPos2 -= shot->getCurrentVelocity() * 0.2f;

			smokeemitter_->setVelocity(vel1, vel2);
			smokeemitter_->emitLinear(3, actualPos1, actualPos2, 
				ScorchedClient::instance()->getParticleEngine(), 
				ParticleRendererQuads::getInstance());
		}
	}
}

void MissileActionRenderer::draw(Action *action)
{
	ShotProjectile *shot = (ShotProjectile *) action;
	Vector &actualPos = shot->getCurrentPosition();
	Vector &actualdir = shot->getCurrentVelocity();

	if (shot->getWeapon()->getShowShotPath())
	{
		Tank *current = 
			action->getScorchedContext()->tankContainer->
			getTankById(shot->getPlayerId());
		if (current)
		{
			glColor3fv(current->getColor());
			TankRenderer::instance()->getTracerStore().drawSmokeTracer(
				shot->getPositions());
		}
	}

	// Check we can see the missile
	if (!GLCameraFrustum::instance()->pointInFrustum(actualPos))
	{
		return;
	}

	// Do we have a loaded mesh
	if (!mesh_)
	{
		Tank *currentPlayer = action->
			getScorchedContext()->tankContainer->getTankById(
			shot->getPlayerId());
		mesh_ = Accessory::getWeaponMesh(
			shot->getWeapon()->getModelID(), currentPlayer);
	}

	mesh_->setScale(scale_);
	mesh_->draw(actualPos, actualdir, flareType_, rotation_);
}

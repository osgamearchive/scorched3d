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

#include <client/MainCamera.h>
#include <client/ScorchedClient.h>
#include <target/TargetDamageCalc.h>
#include <sound/SoundUtils.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <common/OptionsDisplay.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <weapons/AccessoryStore.h>
#include <weapons/WeaponExplosion.h>
#include <actions/Explosion.h>
#include <actions/CameraPositionAction.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <engine/ParticleEmitter.h>
#include <landscape/DeformLandscape.h>
#include <landscape/DeformTextures.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/Landscape.h>
#include <landscape/Water.h>
#include <landscape/Smoke.h>
#include <sprites/ExplosionNukeRenderer.h>
#include <sprites/ExplosionTextures.h>
#include <math.h>

REGISTER_ACTION_SOURCE(Explosion);

Explosion::Explosion() :
	firstTime_(true), totalTime_(0.0f)
{
}

Explosion::Explosion(Vector &position,
	WeaponExplosion *weapon, unsigned int fired,
	unsigned int data) :
	firstTime_(true), totalTime_(0.0f),
	weapon_(weapon), playerId_(fired), 
	position_(position), data_(data)
{
}

Explosion::~Explosion()
{
}

void Explosion::init()
{
	float multiplier = float(((int) context_->optionsGame->getWeapScale()) - 
							 OptionsGame::ScaleMedium);
	multiplier *= 0.5f;
	multiplier += 1.0f;
	float explosionSize = weapon_->getSize() * multiplier;	

	if (!context_->serverMode) 
	{
		float height = context_->landscapeMaps->getGroundMaps().getInterpHeight(
			position_[0], position_[1]);
		float aboveGround = position_[2] - height;

		// If there is a weapon play a splash sound when in water
		bool waterSplash = false;
		if (weapon_->getCreateSplash())
		{
			waterSplash = 
				Landscape::instance()->getWater().explosion(
					position_, weapon_->getSize());
		}

		// Create particles from the center of the explosion
		// These particles will render smoke trails or bits of debris
		if (weapon_->getCreateDebris())
		{
			// If we are below the ground create the spray of dirt (or water)
			if (aboveGround < 1.0f && !waterSplash)
			{
				ParticleEmitter sprayemitter;
				sprayemitter.setAttributes(
					3.0f, 4.0f, // Life
					0.5f, 1.0f, // Mass
					0.01f, 0.02f, // Friction
					Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), // Velocity
					Vector(0.9f, 0.9f, 0.9f), 0.5f, // StartColor1
					Vector(1.0f, 1.0f, 1.0f), 0.7f, // StartColor2
					Vector(0.9f, 0.9f, 0.9f), 0.0f, // EndColor1
					Vector(1.0f, 1.0f, 1.0f), 0.1f, // EndColor2
					3.0f, 3.0f, 4.0f, 4.0f, // Start Size
					3.0f, 3.0f, 4.0f, 4.0f, // EndSize
					Vector(0.0f, 0.0f, -800.0f), // Gravity
					false,
					true);

				sprayemitter.emitSpray(position_, 
					ScorchedClient::instance()->getParticleEngine(),
					weapon_->getSize() - 2.0f,
					&Landscape::instance()->getLandscapeTexture1());
			}

			ParticleEmitter emitter;
			emitter.setAttributes(
				2.5f, 4.0f, // Life
				0.2f, 0.5f, // Mass
				0.01f, 0.02f, // Friction
				Vector(-0.05f, -0.1f, 0.3f), Vector(0.05f, 0.1f, 0.9f), // Velocity
				Vector(0.7f, 0.7f, 0.7f), 0.3f, // StartColor1
				Vector(0.7f, 0.7f, 0.7f), 0.3f, // StartColor2
				Vector(0.7f, 0.7f, 0.7f), 0.0f, // EndColor1
				Vector(0.8f, 0.8f, 0.8f), 0.1f, // EndColor2
				0.2f, 0.2f, 0.5f, 0.5f, // Start Size
				2.2f, 2.2f, 4.0f, 4.0f, // EndSize
				Vector(0.0f, 0.0f, -800.0f), // Gravity
				false,
				true);

			// Create debris
			float mult = float(
				OptionsDisplay::instance()->getExplosionParticlesMult()) / 40.0f;
			int debris = 5 + int(weapon_->getSize() * mult);
			emitter.emitDebris(debris,
				position_, 
				ScorchedClient::instance()->getParticleEngine());
		}

		if (0 == strcmp(weapon_->getAccessoryTypeName(), "WeaponMuzzle"))
		{
			// Add initial smoke clouds
			for (int a=0; a<OptionsDisplay::instance()->getNumberExplosionSubParts() * 2; a++)
			{
				float posXY = (RAND * 4.0f) - 2.0f;
				float posYZ = (RAND * 4.0f) - 2.0f;

				Landscape::instance()->getSmoke().addSmoke(
					position_[0] + posXY, position_[1] + posYZ, position_[2] + 2.0f);
			}
		}
		else
		{
			context_->viewPoints->explosion(playerId_);
		}

		{
			GLTextureSet *texture = 0;
			if (0 != strcmp(weapon_->getExplosionTexture(), "none"))
			{
				texture = ExplosionTextures::instance()->getTextureSetByName(
					weapon_->getExplosionTexture());
			}

			if (texture)
			{
				Vector expColor = weapon_->getExplosionColor();

				ParticleEmitter exploemitter;
				exploemitter.setAttributes(
					weapon_->getMinLife(), weapon_->getMaxLife(), // Life
					0.2f, 0.5f, // Mass
					0.01f, 0.02f, // Friction
					Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), // Velocity
					expColor, 0.8f, // StartColor1
					expColor, 0.9f, // StartColor2
					expColor, 0.0f, // EndColor1
					expColor, 0.1f, // EndColor2
					0.2f, 0.2f, 0.5f, 0.5f, // Start Size
					2.2f, 2.2f, 4.0f, 4.0f, // EndSize
					Vector(0.0f, 0.0f, 0.0f), // Gravity
					weapon_->getLuminance(),
					weapon_->getWindAffected());
				exploemitter.emitExplosion(position_,
					ScorchedClient::instance()->getParticleEngine(),
					explosionSize,
					texture,
					weapon_->getAnimate());
			}
		}

		if (weapon_->getCreateMushroomAmount() > 0.0f &&
			aboveGround < 2.0f)
		{
			if (RAND <= weapon_->getCreateMushroomAmount())
			{
				context_->actionController->addAction(
					new SpriteAction(
					new ExplosionNukeRenderer(position_, weapon_->getSize() - 2.0f)));	
			}
		}

		// Make the camera shake
		MainCamera::instance()->getCamera().addShake(weapon_->getShake());
	}
}

void Explosion::simulate(float frameTime, bool &remove)
{
	totalTime_ += frameTime;
	if (firstTime_)
	{
		firstTime_ = false;
		if (!context_->serverMode) 
		{
			if (weapon_->getExplosionSound() &&
				0 != strcmp("none", weapon_->getExplosionSound()))
			{
				SoundBuffer *expSound = 
					Sound::instance()->fetchOrCreateBuffer(
						(char *) getDataFile("data/wav/%s", weapon_->getExplosionSound()));
				SoundUtils::playAbsoluteSound(VirtualSoundPriority::eAction,
					expSound, position_);
			}
		}

		// Dirt should only form along the ground
		Vector newPosition = position_;
		if (weapon_->getDeformType() == DeformUp)
		{
			newPosition[2] = context_->landscapeMaps->getGroundMaps().
				getInterpHeight(newPosition[0], newPosition[1]);			
		}

		if (weapon_->getDeformType() != DeformNone)
		{
			// Get the actual explosion size
			float multiplier = 
				float(((int) context_->optionsGame->getWeapScale()) - 
					OptionsGame::ScaleMedium);
			multiplier *= 0.5f;
			multiplier += 1.0f;
			float explosionSize = weapon_->getSize() * multiplier;	

			// Remove areas from the height map
			static DeformLandscape::DeformPoints map;
			if (DeformLandscape::deformLandscape(
				*context_,
				newPosition, explosionSize, 
				(weapon_->getDeformType() == DeformDown), map,
				playerId_))
			{
				if (!context_->serverMode) 
				{
					DeformTextures::deformLandscape(newPosition, explosionSize, 
						(weapon_->getDeformType() == DeformDown), 
						ExplosionTextures::instance()->getScorchBitmap(weapon_->getDeformTexture()),
						map);
				}
			}
		}

		if (weapon_->getHurtAmount() != 0.0f ||
			weapon_->getDeformType() != DeformNone)
		{
			// Check the tanks for damage
			TargetDamageCalc::explosion(
				*context_,
				weapon_, playerId_, 
				newPosition, 
				weapon_->getSize() , 
				weapon_->getHurtAmount(),
				(weapon_->getDeformType() != DeformNone),
				weapon_->getOnlyHurtShield(),
				data_);
		}
	}

	if (!renderer_) remove = true;
	Action::simulate(frameTime, remove);
}

bool Explosion::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(position_[0]);
	buffer.addToBuffer(position_[1]);
	buffer.addToBuffer(position_[2]);
	context_->accessoryStore->writeWeapon(buffer, weapon_);
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(data_);
	return true;
}

bool Explosion::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(position_[0])) return false;
	if (!reader.getFromBuffer(position_[1])) return false;
	if (!reader.getFromBuffer(position_[2])) return false;
	weapon_ = (WeaponExplosion *) context_->accessoryStore->readWeapon(reader); if (!weapon_) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(data_)) return false;

	if (0 != strcmp(weapon_->getAccessoryTypeName(), "WeaponMuzzle"))
	{
		const float ShowTime = 4.0f;
		ActionMeta *pos = new CameraPositionAction(
			position_, ShowTime, 10);
		context_->actionController->getBuffer().clientAdd(-3.0f, pos);
	}

	return true;
}

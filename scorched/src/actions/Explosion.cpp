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
#include <tank/TankController.h>
#include <common/OptionsGame.h>
#include <common/SoundStore.h>
#include <common/OptionsParam.h>
#include <common/OptionsDisplay.h>
#include <common/Defines.h>
#include <weapons/AccessoryStore.h>
#include <weapons/WeaponExplosion.h>
#include <actions/Explosion.h>
#include <actions/CameraPositionAction.h>
#include <actions/SpriteProjectile.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <landscape/DeformLandscape.h>
#include <landscape/DeformTextures.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/Landscape.h>
#include <sprites/ExplosionRenderer.h>
#include <sprites/ExplosionNukeRenderer.h>
#include <sprites/SprayActionRenderer.h>
#include <sprites/SmokeActionRenderer.h>
#include <sprites/DebrisActionRenderer.h>
#include <sprites/ExplosionTextures.h>
#include <math.h>

REGISTER_ACTION_SOURCE(Explosion);

Explosion::Explosion() :
	firstTime_(true), totalTime_(0.0f)
{

}

Explosion::Explosion(Vector &position,
	Weapon *weapon, unsigned int fired) :
	firstTime_(true), totalTime_(0.0f),
	weapon_(weapon), playerId_(fired), 
	position_(position)
{

}

Explosion::~Explosion()
{

}

void Explosion::init()
{
	WeaponExplosion *explosion = (WeaponExplosion *) weapon_;

	float multiplier = float(((int) context_->optionsGame->getWeapScale()) - 
							 OptionsGame::ScaleMedium);
	multiplier *= 0.5f;
	multiplier += 1.0f;
	float explosionSize = explosion->getSize() * multiplier;	

	if (!context_->serverMode) 
	{
		float height = context_->landscapeMaps->getHMap().getInterpHeight(
			position_[0], position_[1]);
		float aboveGround = position_[2] - height;

		// Create particles from the center of the explosion
		// These particles will render smoke trails or bits of debris
		if (explosion->getCreateDebris())
		{
			// If there is a weapon play a splash sound when in water
			if (position_[2] < 5.0f)
			{
				float mult = Landscape::instance()->getWater().getWidthMult();
				int posX = int((position_[0] + 64.0f) / mult);
				int posY = int((position_[1] + 64.0f) / mult);

				Landscape::instance()->getWater().addWave(posX, posY, 5.0f);

				CACHE_SOUND(sound, (char *) getDataFile("data/wav/misc/splash.wav"));
				sound->play();
			}

			// If we are below the ground create the spray of dirt (or water)
			if (aboveGround < 0.0f)
			{
				context_->actionController->addAction(
					new SpriteAction(
						new SprayActionRenderer(position_, explosion->getSize() - 2.0f)));
			}

			// Create debris
			float mult = float(
				OptionsDisplay::instance()->getExplosionParticlesMult()) / 10.0f;
			for (int a=0; a<int(explosion->getSize() * mult); a++)
			{
				int numberOfSprites = 40;
				if (OptionsDisplay::instance()->getEffectsDetail() == 0) 
					numberOfSprites = 10;
				else if (OptionsDisplay::instance()->getEffectsDetail() == 2) 
					numberOfSprites = 100;

				if ((int) SpriteProjectile::getNoSpriteProjectiles() <
					numberOfSprites)
				{
					float direction = RAND * 3.14f * 2.0f;
					float speed = RAND * 5.0f + 5.0f;
					float height = RAND * 10.0f + 5.0f;
					Vector velocity(sinf(direction) * speed, 
						cosf(direction) * speed, height);

					SpriteProjectile *particle = new SpriteProjectile;
					particle->setScorchedContext(context_);
					particle->setPhysics(position_, velocity);
					particle->setData(&particle->collisionInfo);

					if (RAND > 0.5)
					{
						SmokeActionRenderer *render = new SmokeActionRenderer;
						particle->setActionRender(render);
					}
					else
					{
						DebrisActionRenderer *render = new DebrisActionRenderer;
						particle->setActionRender(render);
					}

					context_->actionController->addAction(particle);
				}
			}
		}

		if (0 == strcmp(explosion->getAccessoryTypeName(), "WeaponMuzzle"))
		{
			// Add initial smoke clouds
			for (int a=0; a<OptionsDisplay::instance()->getNumberExplosionSubParts() * 2; a++)
			{
				float posXY = (RAND * 4.0f) - 2.0f;
				float posYZ = (RAND * 4.0f) - 2.0f;

				Landscape::instance()->getSmoke().addSmoke(
					position_[0] + posXY, position_[1] + posYZ, position_[2] + 2.0f,
					0.0f, 0.0f, 0.0f,
					0.5f);
			}
		}

		{
			Vector expColor(1.0f, 1.0f, 1.0f);
			GLTextureSet *texture = 0;
			if (0 != strcmp(weapon_->getExplosionTexture(), "none"))
			{
				texture = ExplosionTextures::instance()->getTextureSetByName(
					weapon_->getExplosionTexture());
				expColor = weapon_->getExplosionColor();
			}

			if (texture)
			{
				setActionRender(
					new ExplosionRenderer(
						position_, 
						*texture,
						expColor,
						explosionSize, 
						(explosion->getHurtAmount() > 0.0f)));
			}
		}

		if (explosion->getCreateMushroom() &&
			aboveGround < 2.0f)
		{
			context_->actionController->addAction(
				new SpriteAction(
				new ExplosionNukeRenderer(position_, explosion->getSize() - 2.0f)));
		}

		// Make the camera shake
		MainCamera::instance()->getCamera().addShake(weapon_->getShake());
	}
	else 
	{
		if (0 != strcmp(explosion->getAccessoryTypeName(), "WeaponMuzzle"))
		{
			const float ShowTime = 4.0f;
			ActionMeta *pos = new CameraPositionAction(
				position_, ShowTime, 10);
			context_->actionController->getBuffer().serverAdd(
				context_->actionController->getActionTime() - 3.0f,
				pos);
			delete pos;
		}
	}
}

void Explosion::simulate(float frameTime, bool &remove)
{
	totalTime_ += frameTime;
	if (firstTime_)
	{
		firstTime_ = false;
		WeaponExplosion *explosion = (WeaponExplosion *) weapon_;

		if (!context_->serverMode) 
		{
			if (weapon_->getExplosionSound() &&
				0 != strcmp("none", weapon_->getExplosionSound()))
			{
				SoundBuffer *expSound = 
					SoundStore::instance()->fetchOrCreateBuffer(
						(char *) getDataFile("data/wav/%s", weapon_->getExplosionSound()));
				expSound->play();
			}
		}

		if (explosion->getDeformType() != DeformNone)
		{
			// Get the actual explosion size
			float multiplier = 
				float(((int) context_->optionsGame->getWeapScale()) - 
					OptionsGame::ScaleMedium);
			multiplier *= 0.5f;
			multiplier += 1.0f;
			float explosionSize = explosion->getSize() * multiplier;	

			// Remove areas from the height map
			static DeformLandscape::DeformPoints map;
			if (DeformLandscape::deformLandscape(
				*context_,
				position_, explosionSize, 
				(explosion->getDeformType() == DeformDown), map))
			{
				if (!context_->serverMode) 
				{
					DeformTextures::deformLandscape(position_, explosionSize, 
						(explosion->getDeformType() == DeformDown), map);
				}
			}
		}

		// Check the tanks for damage
		TankController::explosion(
			*context_,
			weapon_, playerId_, 
			position_, 
			explosion->getSize() , 
			explosion->getHurtAmount());
	}

	if (!renderer_) remove = true;
	Action::simulate(frameTime, remove);
}

bool Explosion::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(position_[0]);
	buffer.addToBuffer(position_[1]);
	buffer.addToBuffer(position_[2]);
	Weapon::write(buffer, weapon_);
	buffer.addToBuffer(playerId_);
	return true;
}

bool Explosion::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(position_[0])) return false;
	if (!reader.getFromBuffer(position_[1])) return false;
	if (!reader.getFromBuffer(position_[2])) return false;
	weapon_ = Weapon::read(reader); if (!weapon_) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	return true;
}

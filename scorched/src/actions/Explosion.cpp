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
#include <common/Defines.h>
#include <weapons/AccessoryStore.h>
#include <weapons/WeaponProjectile.h>
#include <actions/Explosion.h>
#include <actions/ShotProjectile.h>
#include <actions/CameraPositionAction.h>
#include <engine/ScorchedContext.h>
#include <landscape/DeformLandscape.h>
#include <landscape/DeformTextures.h>
#include <sprites/ExplosionRenderer.h>
#include <sprites/ExplosionNukeRenderer.h>
#include <sprites/SprayActionRenderer.h>
#include <sprites/ExplosionTextures.h>
#include <math.h>

REGISTER_ACTION_SOURCE(Explosion);

Explosion::Explosion() :
	firstTime_(true), vPoint_(0), totalTime_(0.0f)
{

}

Explosion::Explosion(Vector &position, float width, 
					 Weapon *weapon, unsigned int fired,
					 bool explosionHurts,
					 DeformType deformType) :
	firstTime_(true),
	weapon_(weapon), playerId_(fired), 
	position_(position), width_(width), deformType_(deformType),
	explosionHurts_(explosionHurts), vPoint_(0), totalTime_(0.0f)
{

}

Explosion::~Explosion()
{
	if (vPoint_) context_->viewPoints.releaseViewPoint(vPoint_);
}

void Explosion::init()
{
	float multiplier = float(((int) context_->optionsGame.getWeapScale()) - 
							 OptionsGame::ScaleMedium);
	multiplier *= 0.5f;
	multiplier += 1.0f;
	float explosionSize = width_ * multiplier;	

	if (deformType_ != DeformNone)
	{
		vPoint_ = context_->viewPoints.getNewViewPoint(playerId_);
	}

	if (!context_->serverMode) 
	{
		GLTextureSet *texture = &ExplosionTextures::instance()->exp00;
		Vector expColor(1.0f, 1.0f, 1.0f);
		if (deformType_ != DeformNone)
		{
			texture = ExplosionTextures::instance()->getTextureSetByName(
				weapon_->getExplosionTexture());
			expColor = weapon_->getExplosionColor();
		}

		float height = context_->landscapeMaps.getHMap().getInterpHeight(
			position_[0], position_[1]);
		float aboveGround = position_[2] - height;

		setActionRender(
			new ExplosionRenderer(
				position_, 
				*texture,
				expColor,
				explosionSize, 
				explosionHurts_));
		if (width_ >=11 && deformType_==DeformDown && explosionHurts_ &&
			aboveGround < 2.0f)
		{
			context_->actionController.addAction(
				new SpriteActionReferenced(
					new ExplosionNukeRenderer(position_, float(width_ - 2))));
		}
		if (deformType_==DeformDown && explosionHurts_)
		{
			context_->actionController.addAction(
				new SpriteActionReferenced(
					new SprayActionRenderer(position_, width_ - 2)));
		}

		if (weapon_ && explosionHurts_)
		{
			// Make the camera shake
			MainCamera::instance()->getCamera().addShake(weapon_->getShake());
		}
	}
	else 
	{
		if (deformType_ != DeformNone)
		{
			const float ShowTime = 4.0f;
			ActionMeta *pos = new CameraPositionAction(
				position_, ShowTime, 10);
			context_->actionController.getBuffer().serverAdd(
				context_->actionController.getActionTime() - 3.0f,
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

		if (!context_->serverMode) 
		{
			if (deformType_ != DeformNone)
			{
				if (weapon_->getExplosionSound())
				{
					static char soundBuffer[256];
					sprintf(soundBuffer, PKGDIR "data/wav/%s", weapon_->getExplosionSound());
					SoundBuffer *expSound = 
						SoundStore::instance()->fetchOrCreateBuffer(soundBuffer);
					expSound->play();
				}
			}
		}

		if (deformType_ != DeformNone)
		{
			// Get the actual explosion size
			float multiplier = 
				float(((int) context_->optionsGame.getWeapScale()) - 
					OptionsGame::ScaleMedium);
			multiplier *= 0.5f;
			multiplier += 1.0f;
			float explosionSize = width_ * multiplier;	

			// Remove areas from the height map
			static DeformLandscape::DeformPoints map;
			if (DeformLandscape::deformLandscape(
				*context_,
				position_, explosionSize, 
				(deformType_ == DeformDown), map))
			{
				if (!context_->serverMode) 
				{
					DeformTextures::deformLandscape(position_, explosionSize, 
						(deformType_ == DeformDown), map);
				}
			}
		}

		// Check the tanks for damage
		TankController::explosion(
			*context_,
			weapon_, playerId_, 
			position_, width_ , !explosionHurts_);
	}

	if (deformType_ != DeformNone)
	{
		if (vPoint_)
		{
			vPoint_->setPosition(position_);
			Vector velocity(
				sinf(0.0f / 10.0f * 3.14f), 
				cosf(0.0f / 10.0f * 3.14f), 1.0f);
			vPoint_->setLookFrom(velocity);
			vPoint_->setRadius(10.0f + width_ / 3.0f);
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
	buffer.addToBuffer(width_);
	Weapon::write(buffer, weapon_);
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(explosionHurts_);
	buffer.addToBuffer((int) deformType_);
	return true;
}

bool Explosion::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(position_[0])) return false;
	if (!reader.getFromBuffer(position_[1])) return false;
	if (!reader.getFromBuffer(position_[2])) return false;
	if (!reader.getFromBuffer(width_)) return false;
	weapon_ = Weapon::read(reader); if (!weapon_) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(explosionHurts_)) return false;
	int deform;
	if (!reader.getFromBuffer(deform)) return false;
	deformType_ = (DeformType) deform;

	return true;
}

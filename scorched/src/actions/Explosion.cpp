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

#include <tank/TankController.h>
#include <common/OptionsGame.h>
#include <common/SoundStore.h>
#include <common/OptionsParam.h>
#include <weapons/AccessoryStore.h>
#include <weapons/WeaponProjectile.h>
#include <actions/Explosion.h>
#include <actions/ShotProjectile.h>
#include <engine/ScorchedContext.h>
#include <landscape/DeformLandscape.h>
#include <landscape/DeformTextures.h>
#include <sprites/ExplosionRenderer.h>
#include <sprites/ExplosionNukeRenderer.h>
#include <sprites/ExplosionTextures.h>

REGISTER_ACTION_SOURCE(Explosion);

Explosion::Explosion() :
	firstTime_(true)
{

}

Explosion::Explosion(Vector &position, float width, 
					 Weapon *weapon, unsigned int fired,
					 bool explosionHurts,
					 DeformType deformType) :
	firstTime_(true),
	weapon_(weapon), playerId_(fired), 
	position_(position), width_(width), deformType_(deformType),
	explosionHurts_(explosionHurts)
{

}

Explosion::~Explosion()
{
}

void Explosion::draw()
{
	if (context_->tankContainer.getCurrentPlayerId() == playerId_)
	{
		ShotProjectile::getLookAtPosition() += position_;
		ShotProjectile::getLookAtCount()++;
	}

	ActionMeta::draw();
}

void Explosion::init()
{
	float multiplier = float(((int) context_->optionsGame.getWeapScale()) - 
							 OptionsGame::ScaleMedium);
	multiplier *= 0.5f;
	multiplier += 1.0f;
	float explosionSize = width_ * multiplier;	

	if (!OptionsParam::instance()->getOnServer()) 
	{
		GLTextureSet *texture = &ExplosionTextures::instance()->exp00;
		std::map<const char *, GLTextureSet*>::iterator itor =
			ExplosionTextures::instance()->textureSets.find(weapon_->getExplosionTexture());
		if (itor != ExplosionTextures::instance()->textureSets.end())
		{
			texture = (*itor).second;
		}

		setActionRender(
			new ExplosionRenderer(
				position_, 
				*texture,
				explosionSize, explosionHurts_));
		if (width_ >=17 && deformType_==DeformDown)
		{
			context_->actionController.addAction(
				new SpriteAction(new ExplosionNukeRenderer(position_, float(width_ - 2))));
		}
	}
}

void Explosion::simulate(float frameTime, bool &remove)
{
	if (firstTime_)
	{
		firstTime_ = false;

		if (!OptionsParam::instance()->getOnServer()) 
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
				if (!OptionsParam::instance()->getOnServer()) 
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

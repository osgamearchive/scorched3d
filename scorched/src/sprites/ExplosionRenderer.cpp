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

#include <math.h>
#include <common/OptionsDisplay.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLCameraFrustum.h>
#include <landscape/Landscape.h>
#include <sprites/ExplosionTextures.h>
#include <sprites/ExplosionRenderer.h>
#include <sprites/SmokeActionRenderer.h>
#include <sprites/DebrisActionRenderer.h>
#include <actions/SpriteProjectile.h>
#include <engine/ScorchedContext.h>
#include <common/SoundStore.h>
#include <common/Defines.h>

ExplosionRenderer::ExplosionMainPart::ExplosionMainPart()
{
	subParts = new ExplosionSubPart[
		OptionsDisplay::instance()->getNumberExplosionSubParts()];

	float randRotXY = (RAND * TWOPI);
	float randRotXZ = (RAND * TWOPI);
	float cosRandRotXZ = (float) cos(randRotXZ);
	Vector move;
	move[0] = float(sin(randRotXY) * cosRandRotXZ);
	move[1] = float(cos(randRotXY) * cosRandRotXZ);
	move[2] = float(sin(randRotXZ));

	position = Vector();
	positionMove = move;
}

ExplosionRenderer::ExplosionMainPart::~ExplosionMainPart()
{
	delete [] subParts;
}

void ExplosionRenderer::ExplosionMainPart::simulate(float frameTime)
{
	position += positionMove * frameTime * 3.0f;

	for (int j=0; j<OptionsDisplay::instance()->getNumberExplosionSubParts(); j++)
	{
		subParts[j].simulate(frameTime);
	}
}

void ExplosionRenderer::ExplosionMainPart::draw(
	Vector &center, Vector &color, float w, float opacity1, 
	float opacity2, GLTexture *t1, GLTexture *t2)
{
	Vector newposition = center + position;

	for (int j=0; j<OptionsDisplay::instance()->getNumberExplosionSubParts(); j++)
	{
		subParts[j].draw(newposition, color, w, opacity1, opacity2, t1, t2);
	}
}

ExplosionRenderer::ExplosionSubPart::ExplosionSubPart()
{
	float randRotXY = (RAND * TWOPI);
	float randRotXZ = (RAND * TWOPI);
	float cosRandRotXZ = (float) cos(randRotXZ);
	Vector move;
	move[0] = float(sin(randRotXY) * cosRandRotXZ);
	move[1] = float(cos(randRotXY) * cosRandRotXZ);
	move[2] = float(sin(randRotXZ));

	rotation[0] = RAND * 1.0f - 0.5f;
	rotation[1] = RAND * 1.0f - 0.5f;
	rotation[2] = RAND * 1.0f - 0.5f;
	position = Vector();
	positionMove = move;
	graphicEntry1_ = new GLBilboardRenderer::GLBilboardOrderedEntry;
	graphicEntry2_ = new GLBilboardRenderer::GLBilboardOrderedEntry;

	graphicEntry1_->textureCoord = 
		graphicEntry2_->textureCoord = int(RAND * 4.0);

	GLBilboardRenderer::instance()->addEntry(graphicEntry1_);
	GLBilboardRenderer::instance()->addEntry(graphicEntry2_);
}

ExplosionRenderer::ExplosionSubPart::~ExplosionSubPart()
{
	GLBilboardRenderer::instance()->removeEntry(graphicEntry1_);
	GLBilboardRenderer::instance()->removeEntry(graphicEntry2_);	
}

void ExplosionRenderer::ExplosionSubPart::simulate(float frameTime)
{
	position += positionMove * frameTime * 1.5f;

	{
		float rotationx = rotation[0] * frameTime * 3.14f;
		Vector oldPos = position;
		position[1] = oldPos[1] * cosf(rotationx) - oldPos[2] * sinf(rotationx);
		position[2] = oldPos[1] * sinf(rotationx) + oldPos[2] * cosf(rotationx);
		oldPos = positionMove;
		positionMove[1] = oldPos[1] * cosf(rotationx) - oldPos[2] * sinf(rotationx);
		positionMove[2] = oldPos[1] * sinf(rotationx) + oldPos[2] * cosf(rotationx);
	}

	{
		float rotationy = rotation[1] * frameTime * 3.14f;
		Vector oldPos = position;
		position[0] = oldPos[0] * cosf(rotationy) + oldPos[2] * sinf(rotationy);
		position[2] = -oldPos[0] * sinf(rotationy) + oldPos[2] * cosf(rotationy);
		oldPos = positionMove;
		positionMove[0] = oldPos[0] * cosf(rotationy) + oldPos[2] * sinf(rotationy);
		positionMove[2] = -oldPos[0] * sinf(rotationy) + oldPos[2] * cosf(rotationy);
	}
}

void ExplosionRenderer::ExplosionSubPart::draw(
	Vector &center, Vector &color, float w, float opacity1, 
	float opacity2, GLTexture *t1, GLTexture *t2)
{	
	// Set the bilboard parameters and add it the renderer
	graphicEntry1_->posX = graphicEntry2_->posX = center[0] + position[0];
	graphicEntry1_->posY = graphicEntry2_->posY = center[1] + position[1];
	graphicEntry1_->posZ = graphicEntry2_->posZ = center[2] + position[2];
	graphicEntry1_->width = graphicEntry2_->width = w;
	graphicEntry1_->height = graphicEntry2_->height = w;
	graphicEntry1_->texture = t1;
	graphicEntry2_->texture = t2;
	graphicEntry1_->alpha = opacity1;
	graphicEntry2_->alpha = opacity2;
	graphicEntry1_->r_color = color[0];
	graphicEntry1_->g_color = color[1];
	graphicEntry1_->b_color = color[2];
	graphicEntry2_->r_color = color[0];
	graphicEntry2_->g_color = color[1];
	graphicEntry2_->b_color = color[2];
}

ExplosionRenderer::ExplosionRenderer(Vector &position, GLTextureSet &textureSet, 
									 Vector &color, float width, bool weapon) 
	: width_(width), currentWidth_(0.0f), textureSet_(textureSet),
	centrePosition_(position), firstTime_(true),
	weapon_(weapon), color_(color)
{
	mainParts = new ExplosionMainPart[
		OptionsDisplay::instance()->getNumberExplosionSubParts()];
}

ExplosionRenderer::~ExplosionRenderer()
{
	delete [] mainParts;
}

void ExplosionRenderer::simulate(Action *action, float frameTime, bool &remove)
{
	if (firstTime_)
	{
		firstTime_ = false;

		if (weapon_)
		{
			// If there is a weapon play a splash sound when in water
			if (centrePosition_[2] < 5.0f)
			{
				float mult = Landscape::instance()->getWater().getWidthMult();
				int posX = int((centrePosition_[0] + 64.0f) / mult);
				int posY = int((centrePosition_[1] + 64.0f) / mult);

				Landscape::instance()->getWater().addWave(posX, posY, 5.0f);

				CACHE_SOUND(sound,  PKGDIR "data/wav/misc/splash.wav");
				sound->play();
			}

			// Create particles from the center of the explosion
			// These particles will render smoke trails or bits of debris
			float mult = float(
				OptionsDisplay::instance()->getExplosionParticlesMult()) / 10.0f;
			for (int a=0; a<int(width_ * mult); a++)
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
					particle->setScorchedContext(action->getScorchedContext());
					particle->setPhysics(centrePosition_, velocity);
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

					action->getScorchedContext()->actionController.addAction(particle);
				}
			}
		}
		else
		{
			// Add initial smoke clouds
			for (int a=0; a<OptionsDisplay::instance()->getNumberExplosionSubParts() * 2; a++)
			{
				float posXY = (RAND * 4.0f) - 2.0f;
				float posYZ = (RAND * 4.0f) - 2.0f;

				Landscape::instance()->getSmoke().addSmoke(
					centrePosition_[0] + posXY, centrePosition_[1] + posYZ, centrePosition_[2] + 2.0f,
					0.0f, 0.0f, 0.0f,
					0.5f);
			}
		}
	}

	// Update position of explosion parts
	for (int i=0; i<OptionsDisplay::instance()->getNumberExplosionSubParts(); i++)
	{
		mainParts[i].simulate(frameTime);
	}		

	// Explosion gets larger
	currentWidth_ += frameTime * 5.0f;
	remove = (currentWidth_ >= width_);
}

void ExplosionRenderer::draw(Action *action)
{
	if (currentWidth_ < width_)
	{
		if (GLCameraFrustum::instance()->sphereInFrustum(centrePosition_, currentWidth_))
		{
			drawExplosion();
		}
	}
}

void ExplosionRenderer::drawExplosion()
{
	float width = currentWidth_ / 1.5f;
	float opacity = 0.5f;
	if (currentWidth_ > width_ - 1.0f)
	{
		float left = width_ - currentWidth_;
		opacity = left * 0.5f;
	}

	// Calculate which texture to render
	float textureNumf = (currentWidth_ / width_) * float(textureSet_.getNoTextures()-1);
	int firstTextureNum = int(textureNumf);
	int secondTextureNum = firstTextureNum + 1;
	float textureDiff = textureNumf - firstTextureNum;

	int i;
	// Draw explosions
	for (i=1; i<OptionsDisplay::instance()->getNumberExplosionSubParts(); i++)
	{
		mainParts[i].draw(centrePosition_, color_,
			width,
			opacity * (1.0f - textureDiff), 
			opacity * (textureDiff), 
			textureSet_.getTexture(firstTextureNum),
			textureSet_.getTexture(secondTextureNum));
	}

	// Draw smoke clouds
	static Vector white(1.0f, 1.0f, 1.0f);
	for (i=0; i<1; i++)
	{
		mainParts[i].draw(centrePosition_, white,
			0.3f, 0.3f,
			width, 
			&ExplosionTextures::instance()->smokeTexture,
			&ExplosionTextures::instance()->smokeTexture);
	}
}

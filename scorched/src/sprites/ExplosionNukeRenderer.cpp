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

#include <sprites/ExplosionNukeRenderer.h>
#include <sprites/ExplosionTextures.h>
#include <client/ScorchedClient.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <math.h>

static const int AlphaSteps = int(ExplosionNukeRenderer_STEPS * 0.9f);

void ExplosionNukeRenderer::Entry::simulate()
{
	position_ ++;
	if (position_ > AlphaSteps)
	{
		alpha -= alphaDec_;
	}
}

void ExplosionNukeRenderer::Entry::draw(Vector startPosition, float size)
{
	// Set the current position
	float z = positions_[position_][2] - size / 2.0f ;
	float w = positions_[position_][0] + 4;
	posX = startPosition[0] + sinf(rotation_) * w;
	posY = startPosition[1] + cosf(rotation_) * w;
	posZ = startPosition[2] + z;

	// Add a little offset to the current position
	// (NOTE: This should really be done in simulate!!)
	if (position_>=(ExplosionNukeRenderer_STEPS *0.3f)){
		//entry.posZ+=(RAND*2.0f)-1.0f;
		float delta= ExplosionNukeRenderer_STEPS-(ExplosionNukeRenderer_STEPS*0.3f);
		float diff = position_-(ExplosionNukeRenderer_STEPS*0.3f);
		float num = diff/delta;  //num =0..1
		posZ-=(num*num*num)*size;
	}
	
	// Add a shadow of the smoke on the ground
	if (smoke_ &&
		posX > 0.0f && posY > 0.0f && 
		posX < 255.0f && posY < 255.0f)
	{
		float aboveGround =	posZ - 
			ScorchedClient::instance()->getLandscapeMaps().getHMap().
			getHeight(int (posX), int(posY));
		Landscape::instance()->getShadowMap().
			addCircle(posX, posY, (height * aboveGround) / 10.0f, 0.7f * alpha);
	}
}

Vector *ExplosionNukeRenderer::positions_ = 0;

ExplosionNukeRenderer::ExplosionNukeRenderer(Vector &position, float size) 
	: totalTime_(0.0f), time_(0.0f), position_(position), size_(size)
{
	if (!positions_)
	{
		positions_ = new Vector[ExplosionNukeRenderer_STEPS];

		float zPos = 0.0f;
		float width = 0.0f;
		float widthAdd = 0.0f;

		for (int i=0; i<ExplosionNukeRenderer_STEPS; i++)
		{
			Vector pos;
			pos[0] = width;
			pos[1] = 0.0f;
			pos[2] = zPos;

			if (i > (ExplosionNukeRenderer_STEPS * (0.3f))){
				width += 0.5f;
				
			}else{ 
				
				zPos += 2.0f;
			}

			positions_[i] = pos;
		}
	}
}

ExplosionNukeRenderer::~ExplosionNukeRenderer()
{
	std::list<Entry*>::iterator itor = entries_.begin();
	std::list<Entry*>::iterator enditor = entries_.end();
	for (;itor != enditor; itor++)
	{
		Entry *entry = *itor;
		GLBilboardRenderer::instance()->removeEntry(entry, true);
	}
}

void ExplosionNukeRenderer::draw(Action *action)
{
 	std::list<Entry*>::iterator itor = entries_.begin();
 	std::list<Entry*>::iterator enditor = entries_.end();
 	for (;itor != enditor; itor++)
 	{
 		Entry *entry = *itor;
 		entry->draw(position_, size_);
 	}
}

void ExplosionNukeRenderer::simulate(Action *action, float frameTime, bool &remove)
{
	float AddSmokeTime = 0.08f;
	int SmokesPerTime = 2;
	if (OptionsDisplay::instance()->getEffectsDetail() == 0) 
	{
		SmokesPerTime = 1;
	}
	else if (OptionsDisplay::instance()->getEffectsDetail() == 2) 
	{
		SmokesPerTime = 5;
	}

	totalTime_ += frameTime;
	time_ += frameTime;
	
	while (time_ > AddSmokeTime)
	{
		time_ -= AddSmokeTime;

		// Simulate all current entries
		std::list<Entry*>::iterator itor = entries_.begin();
		std::list<Entry*>::iterator enditor = entries_.end();
		for (;itor != enditor; itor++)
		{
			(*itor)->simulate();
		}

		// Remove any entries that have finished the cycle
		while (!entries_.empty() && 
			entries_.back()->position_ >= ExplosionNukeRenderer_STEPS)
		{
			Entry *entry = entries_.back();
			GLBilboardRenderer::instance()->removeEntry(entry, true);
			entries_.pop_back();
		}

		// Add any new entries
		if (totalTime_ < size_ / 2.0f)
		{
			for (int i=0; i<SmokesPerTime; i++)
			{
				Entry *entry = new Entry;
				entry->width = 4.0f;
				entry->height = 4.0f;
				entry->alpha = 0.15f+(RAND *0.20f);
				entry->position_ = 0;
				entry->rotation_ = RAND * (3.14f * 2.0f);
				entry->texture = &ExplosionTextures::instance()->smokeTexture;
				entry->textureCoord = (int) (RAND * 3.0f);
				entry->smoke_ = (RAND > 0.80f);

				float red = RAND * 0.2f + 0.8f;
				float other = RAND * 0.2f + 0.8f;

				entry->r_color = red;
				entry->g_color = other;
				entry->b_color = other;

				entry->alphaDec_ = entry->alpha / 
					(float) (ExplosionNukeRenderer_STEPS - AlphaSteps);

				GLBilboardRenderer::instance()->addEntry(entry);
				entries_.push_front(entry);
			}
		}
		else remove = entries_.empty();
	}
}

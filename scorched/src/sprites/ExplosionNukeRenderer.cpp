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
#include <landscape/Landscape.h>
#include <math.h>

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
}

void ExplosionNukeRenderer::draw(Action *action)
{
	std::list<Entry>::iterator itor = entries_.begin();
	std::list<Entry>::iterator enditor = entries_.end();
	for (;itor != enditor; itor++)
	{
		Entry &entry = *itor;

		float z = positions_[entry.position_][2] - size_ / 2.0f ;
		float w = positions_[entry.position_][0] + 4;

		entry.posX = position_[0] + sinf((*itor).rotation_) * w;
		entry.posY = position_[1] + cosf((*itor).rotation_) * w;
		entry.posZ = position_[2] + z;
		if (entry.alpha>=0.1f){
		entry.alpha*=(0.95f+(RAND*0.05f));
		//entry.rotation_+=((RAND*(0.5f*3.14f))-(3.14f*0.25f));
		}
		if (entry.position_>=(ExplosionNukeRenderer_STEPS *0.3f)){
			//entry.posZ+=(RAND*2.0f)-1.0f;
			float delta= ExplosionNukeRenderer_STEPS-(ExplosionNukeRenderer_STEPS*0.3f);
			float diff = entry.position_-(ExplosionNukeRenderer_STEPS*0.3f);
			float num = diff/delta;  //num =0..1
			entry.posZ-=(num*num*num)*size_;


		}
		

		// Add a shadow of the smoke on the ground
		if (entry.posX > 0.0f && entry.posY > 0.0f && entry.posX < 255.0f && entry.posY < 255.0f)
		{
			float aboveGround =	entry.posZ - ScorchedClient::instance()->getLandscapeMaps().getHMap().getHeight(int (entry.posX), int(entry.posY));
			Landscape::instance()->getShadowMap().addCircle(entry.posX, entry.posY, (entry.height * aboveGround) / 10.0f, 0.2f);
		}

		// add the actual smoke cloud
		GLBilboardRenderer::instance()->addEntry(&entry);
	}
}

void ExplosionNukeRenderer::simulate(Action *action, float frameTime, bool &remove)
{
	const float AddSmokeTime = 0.08f;
	const int SmokesPerTime = 4;

	totalTime_ += frameTime;
	time_ += frameTime;
	
	while (time_ > AddSmokeTime)
	{
		time_ -= AddSmokeTime;

		std::list<Entry>::iterator itor = entries_.begin();
		std::list<Entry>::iterator enditor = entries_.end();
		for (;itor != enditor; itor++)
		{
			(*itor).position_ ++;
		}
			while (!entries_.empty() && 
				entries_.back().position_ >= ExplosionNukeRenderer_STEPS) entries_.pop_back();

		if (totalTime_ < size_ / 2.0f)
		{
			for (int i=0; i<SmokesPerTime; i++)
			{
				Entry entry;
				entry.style=1;
				entry.width = 4.0f;
				entry.height = 4.0f;
				entry.alpha = 0.5f+(RAND *0.5f);
				entry.position_ = 0;
				entry.rotation_ = RAND * (3.14f * 2.0f);
				entry.texture = &ExplosionTextures::instance()->smokeTexture;
				entry.textureCoord = (int) (RAND * 3.0f);
				entries_.push_front(entry);
			}
		}
		else remove = entries_.empty();
	}
}

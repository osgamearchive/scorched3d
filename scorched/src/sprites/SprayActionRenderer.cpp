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

#include <sprites/SprayActionRenderer.h>
#include <sprites/ExplosionTextures.h>
#include <landscape/Landscape.h>
#include <common/Defines.h>
#include <math.h>

SprayActionRenderer::SprayActionRenderer(Vector &position, float width) :
	totalTime_(0.0f), time_(0.0f)
{
	if (position[0] < 10.0f || position[1] < 10.0f ||
		position[0] > 250.0f || position[1] > 250.0f) totalTime_ = 100.0f;

	for (int i=0; i<6 + int(width) * 2; i++)
	{
		Entry entry;
		float rotation = RAND * 2.0f * 3.14f;
		float x = sinf(rotation);
		float y = cosf(rotation);
		entry.posX = (x * width * RAND) + position[0];
		entry.posY = (y * width * RAND) + position[1];
		entry.posZ = position[2];
		entry.offset[0] = (x * RAND) / 10.0f;
		entry.offset[1] = (y * RAND) / 10.0f;
		entry.offset[2] = 0.5f * RAND + 0.5f;
		entry.alpha = 0.4f;
		entry.width = 4.0f;
		entry.height = 4.0f;
		entry.textureCoord = int(RAND * 3.0f);

		if (position[2] < 5.0f)
		{
			entry.texture = &Landscape::instance()->getLandscapeTextureWater();
		}
		else
		{
			entry.texture = &Landscape::instance()->getLandscapeTexture1();
		}
		entries_.push_back(entry);
	}
}

SprayActionRenderer::~SprayActionRenderer()
{
}

void SprayActionRenderer::draw(Action *action)
{
	std::list<Entry>::iterator itor = entries_.begin();
	std::list<Entry>::iterator enditor = entries_.end();
	for (;itor != enditor; itor++)
	{
		Entry &entry = *itor;

		// add the actual smoke cloud
		GLBilboardRenderer::instance()->addEntry(&entry);	
	}
}

void SprayActionRenderer::simulate(Action *action, float frameTime, bool &remove)
{
	time_ += frameTime;
	totalTime_ += frameTime;

	while (time_ > 0.05f)
	{
		time_ -= 0.05f;

		std::list<Entry>::iterator itor = entries_.begin();
		std::list<Entry>::iterator enditor = entries_.end();
		for (;itor != enditor; itor++)
		{
			Entry &entry = *itor;

			entry.posX += entry.offset[0];
			entry.posY += entry.offset[1];
			entry.posZ += entry.offset[2];
			entry.alpha = 0.35f - (totalTime_ / 8.0f);

			entry.offset[2] -= 0.03f;
		}
	}

	if (totalTime_ > 4.0f) remove = true;
}

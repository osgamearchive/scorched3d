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
	totalTime_(0.0f), time_(0.0f), iteration_(0)
{
	for (int i=0; i<(int)width * 10; i++)
	{
		Entry entry;
		float rotation = RAND * 2.0f * 3.14f;
		float x = sinf(rotation);
		float y = cosf(rotation);
		entry.position[0] = x * width + position[0];
		entry.position[1] = y * width + position[1];
		entry.position[2] = position[2];
		entry.offset[0] = x;
		entry.offset[1] = y;
		entry.offset[2] = 0.5f * RAND + 1.0f;
		entry.offset /= 3.0f;
		if (RAND * 2.0f > 1.0f)
		{
			entry.texture = &Landscape::instance()->getLandscapeTexture1();
		}
		else
		{
			entry.texture = &Landscape::instance()->getLandscapeTexture2();
		}
		entries_.push_back(entry);
	}
}

SprayActionRenderer::~SprayActionRenderer()
{
}

void SprayActionRenderer::draw(Action *action)
{
	unsigned int bilPos = 0;

	std::list<Entry>::iterator itor = entries_.begin();
	std::list<Entry>::iterator enditor = entries_.end();
	for (;itor != enditor; itor++)
	{
		Entry &entry = *itor;
		Vector pos = entry.position;
		for (int i=0; i<iteration_; i++)
		{
			if (bilPos >= bilboardEntries_.size())
			{
				GLBilboardRenderer::Entry *glEntry = new GLBilboardRenderer::Entry();
				glEntry->width = 0.4f;
				glEntry->height = 0.4f;
				glEntry->alpha = 0.4f;
				glEntry->textureCoord = int(RAND * 3.0f);
				bilboardEntries_.push_back(glEntry);
			}
			GLBilboardRenderer::Entry *glEntry = bilboardEntries_[bilPos];
			bilPos++;

			glEntry->alpha = (0.8f - totalTime_);
			glEntry->texture = entry.texture;
			glEntry->posX = pos[0] + RAND * 0.1f;
			glEntry->posY = pos[1] + RAND * 0.1f;
			glEntry->posZ = pos[2];
			pos += entry.offset;

			// add the actual smoke cloud
			GLBilboardRenderer::instance()->addEntry(glEntry);	
		}
	}
}

void SprayActionRenderer::simulate(Action *action, float frameTime, bool &remove)
{
	time_ += frameTime;
	totalTime_ += frameTime;

	if (time_ > 0.03f)
	{
		time_ -= 0.03f;
		iteration_++;
	}

	remove = (totalTime_ > 0.5f);
}

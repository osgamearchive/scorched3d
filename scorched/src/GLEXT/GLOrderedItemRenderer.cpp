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

#include <GLEXT/GLOrderedItemRenderer.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLCameraFrustum.h>
#include <client/MainCamera.h>
#include <common/OptionsDisplay.h>
#include <algorithm>

GLOrderedItemRenderer *GLOrderedItemRenderer::instance_ = 0;

GLOrderedItemRenderer *GLOrderedItemRenderer::instance()
{
	if (!instance_)
	{
		instance_ = new GLOrderedItemRenderer;
	}
	return instance_;
}

GLOrderedItemRenderer::GLOrderedItemRenderer()
{
	// Calculate how many entries we can see
	numberOfBilboards_ = 4000;
	if (OptionsDisplay::instance()->getEffectsDetail() == 0) 
		numberOfBilboards_ = 500;
	else if (OptionsDisplay::instance()->getEffectsDetail() == 2) 
		numberOfBilboards_ = 8000;
}

GLOrderedItemRenderer::~GLOrderedItemRenderer()
{
}

inline unsigned int approx_distance(int  dx, int  dy)
{
   unsigned int approx = (dx * dx) + (dy * dy);
   return approx;
}

inline bool lt_distance(GLOrderedItemRenderer::OrderedEntry *o1, 
						GLOrderedItemRenderer::OrderedEntry *o2) 
{ 
	return o1->distance > o2->distance;
}

void GLOrderedItemRenderer::simulate(const unsigned int state, float simTime)
{
	static float total = 0.0f;
	total += simTime;
	if (total > 0.2f)
	{
		total = 0.0f;

		// Calculate the dist from camera
		Vector &cameraPos = 
			MainCamera::instance()->getCamera().getCurrentPos();

		if (!OptionsDisplay::instance()->getNoDepthSorting())
		{
			// Calculate the current distance for the object
			std::vector<OrderedEntry *>::iterator addItor;
			for (addItor = requiredEntries_.begin();
				addItor != requiredEntries_.end();
				addItor++)
			{
				OrderedEntry *entry = *addItor;
				entry->distance = (float) approx_distance(
					int(cameraPos[0] - entry->posX),
					int(cameraPos[1] - entry->posY));
			}

			// sort by distance
			std::sort(requiredEntries_.begin(), requiredEntries_.end(), lt_distance); 
		}
	}

	// Simulate all of the providers
	std::list<GLOrderedItemRendererProviderSetup *>::iterator itor;
	for (itor = setups_.begin();
		 itor != setups_.end();
		 itor++)
	{
		GLOrderedItemRendererProviderSetup *setup = (*itor);
		setup->itemsSimulate(simTime);
	}
}

void GLOrderedItemRenderer::addSetup(GLOrderedItemRendererProviderSetup *setup)
{
	setups_.push_back(setup);
}

void GLOrderedItemRenderer::addEntry(OrderedEntry *entry)
{
	if (entry->removeItem_)
	{
		entry->removeItem_ = false;
	}
	else
	{
		requiredEntries_.push_back(entry);
	}
}

void GLOrderedItemRenderer::rmEntry(OrderedEntry *entry, bool deleteItem)
{
	entry->removeItem_ = true;
	entry->deleteItem_ = deleteItem;
}

void GLOrderedItemRenderer::draw(const unsigned state)
{
	// Let all drawing classes initialize
	std::list<GLOrderedItemRendererProviderSetup *>::iterator setupitor;
	for (setupitor = setups_.begin();
		 setupitor != setups_.end();
		 setupitor++)
	{
		GLOrderedItemRendererProviderSetup *setup = (*setupitor);
		setup->itemsSetup();
	}

	// Setup the transparecy and textures
	GLState drawState(GLState::TEXTURE_ON | GLState::BLEND_ON | GLState::DEPTH_ON);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Remove previous entries
	tmpRequiredEntries_.clear();

	// Todo sort by depth (hmm)

	// Draw entries 
	int notDrawn = (int) requiredEntries_.size() - numberOfBilboards_;
	Vector point;
	std::vector<OrderedEntry *>::iterator addItor;
	for (addItor = requiredEntries_.begin();
		addItor != requiredEntries_.end();
		addItor++, notDrawn--)
	{
		OrderedEntry *entry = *addItor;

		// Check if we need to draw this item
		if (notDrawn <=0)
		{
			point[0] = entry->posX;
			point[1] = entry->posY;
			point[2] = entry->posZ;
			// Too expensive ??
			//if (GLCameraFrustum::instance()->pointInFrustum(point))
			//{			
				entry->provider_->drawItem(entry->distance, *entry);
			//}
		}

		// Check if we need to remove this item
		if (!entry->removeItem_)
		{
			tmpRequiredEntries_.push_back(entry);
		}
		else if (entry->deleteItem_)
		{
			delete entry;
		}
	}

	// Only use current entries
	requiredEntries_.swap(tmpRequiredEntries_);

	// Reset the depth mask
	glDepthMask(GL_TRUE);
}

GLOrderedItemRendererProvider::~GLOrderedItemRendererProvider()
{
}

GLOrderedItemRendererProviderSetup::~GLOrderedItemRendererProviderSetup()
{
}

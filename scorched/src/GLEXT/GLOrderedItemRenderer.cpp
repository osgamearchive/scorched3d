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
#include <client/MainCamera.h>
#include <common/OptionsDisplay.h>

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

}

GLOrderedItemRenderer::~GLOrderedItemRenderer()
{
}

void GLOrderedItemRenderer::simulate(const unsigned int state, float simTime)
{
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
	if (entry->requiredItem_) requiredEntries_.push_back(entry);
	else notRequiredEntries_.push_back(entry);
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

	std::multimap<float, OrderedEntry *> entries;
	Vector &cameraPos = 
		MainCamera::instance()->getCamera().getCurrentPos();
	std::vector<OrderedEntry *>::iterator addItor;

	// Add entries that are required
	for (addItor = requiredEntries_.begin();
		addItor != requiredEntries_.end();
		addItor++)
	{
		OrderedEntry *entry = *addItor;
		float dist = 0.0f;
		dist += (cameraPos[0] - entry->posX) * (cameraPos[0] - entry->posX);
		dist += (cameraPos[1] - entry->posY) * (cameraPos[1] - entry->posY);
		dist += (cameraPos[2] - entry->posZ) * (cameraPos[2] - entry->posZ);

		entries.insert(std::pair<float, OrderedEntry *>(dist,entry));
	}

	// Add entries that are NOT required
	int numberOfBilboards = 2000;
	if (OptionsDisplay::instance()->getEffectsDetail() == 0) 
		numberOfBilboards = 500;
	else if (OptionsDisplay::instance()->getEffectsDetail() == 2) 
		numberOfBilboards = 5000;

	float bilboardsAllowed = 
		float(numberOfBilboards) /
		float(notRequiredEntries_.size());
	float bilboardCount = 0.0f;
	float bilboardDrawn = 0.0f;
	for (addItor = notRequiredEntries_.begin();
		addItor != notRequiredEntries_.end();
		addItor++)
	{
		bilboardCount += bilboardsAllowed;
		if (bilboardDrawn > bilboardCount) continue;	
		bilboardDrawn += 1.0f;

		OrderedEntry *entry = *addItor;
		float dist = 0.0f;
		dist += (cameraPos[0] - entry->posX) * (cameraPos[0] - entry->posX);
		dist += (cameraPos[1] - entry->posY) * (cameraPos[1] - entry->posY);
		dist += (cameraPos[2] - entry->posZ) * (cameraPos[2] - entry->posZ);

		entries.insert(std::pair<float, OrderedEntry *>(dist,entry));
	}

	// Setup the transparecy and textures
	GLState drawState(GLState::TEXTURE_ON | GLState::BLEND_ON | GLState::DEPTH_ON);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw all the items
	std::multimap<float, OrderedEntry *>::reverse_iterator itor;
	std::multimap<float, OrderedEntry *>::reverse_iterator endItor = entries.rend();
	for (itor = entries.rbegin();
			itor != endItor;
			itor++)
	{
		float distance = (*itor).first;
		OrderedEntry *entry = (*itor).second;
		entry->provider_->drawItem(distance, *entry);
	}

	// Reset the depth mask
	glDepthMask(GL_TRUE);

	// Remove all entries
	notRequiredEntries_.clear();
	requiredEntries_.clear();
}

GLOrderedItemRendererProvider::~GLOrderedItemRendererProvider()
{
}

GLOrderedItemRendererProviderSetup::~GLOrderedItemRendererProviderSetup()
{
}

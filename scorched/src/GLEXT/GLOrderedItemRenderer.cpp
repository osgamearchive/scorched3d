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
	Vector &cameraPos = 
		MainCamera::instance()->getCamera().getCurrentPos();

	float dist = 0.0f;
	dist += (cameraPos[0] - entry->posX) * (cameraPos[0] - entry->posX);
	dist += (cameraPos[1] - entry->posY) * (cameraPos[1] - entry->posY);
	dist += (cameraPos[2] - entry->posZ) * (cameraPos[2] - entry->posZ);

	entries_.insert(std::pair<float, OrderedEntry *>(dist,entry));
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

	// Draw all the items
	std::multimap<float, OrderedEntry *>::reverse_iterator itor;
	std::multimap<float, OrderedEntry *>::reverse_iterator endItor = entries_.rend();
	for (itor = entries_.rbegin();
			itor != endItor;
			itor++)
	{
		OrderedEntry *entry = (*itor).second;
		entry->provider_->drawItem(*entry);
	}

	// Reset the depth mask
	glDepthMask(GL_TRUE);

	// Remove all items, so they need to be added to be drawn again
	entries_.clear();
}

GLOrderedItemRendererProvider::~GLOrderedItemRendererProvider()
{
}

GLOrderedItemRendererProviderSetup::~GLOrderedItemRendererProviderSetup()
{
}

///////////////////////////////////////////////////////////////////////////////
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

#include <GLEXT/GLBilboardRenderer.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLConsoleRuleFnIAdapter.h>
#include <client/MainCamera.h>
#include <common/OptionsDisplay.h>
#include <common/Logger.h>

GLBilboardRenderer *GLBilboardRenderer::instance_ = 0;

GLBilboardRenderer *GLBilboardRenderer::instance()
{
	if (!instance_)
	{
		instance_ = new GLBilboardRenderer;
	}
	return instance_;
}

GLBilboardRenderer::GLBilboardRenderer() : 
	totalTime_(0.0f), totalSwitches_(0), totalBilboards_(0), showMessages_(false)
{
	new GLConsoleRuleFnIBooleanAdapter("BilboardStats", showMessages_);
	GLOrderedItemRenderer::instance()->addSetup(this);
}

GLBilboardRenderer::~GLBilboardRenderer()
{
}

void GLBilboardRenderer::itemsSimulate(float simTime)
{
	const float printTime = 5.0f;
	totalTime_ += simTime;

	if (totalTime_ > printTime)
	{
		if (showMessages_)
		{
			Logger::log(0, "%.2f Bilboards Per Second, %.2f Textures Per Second)", 
				float(totalBilboards_) / totalTime_,
				float(totalSwitches_) / totalTime_);

			totalSwitches_ = 0;
			totalBilboards_ = 0;
			totalTime_ = 0.0f;
		}
	}
}

void GLBilboardRenderer::itemsSetup()
{
	// Setup the bilboard items
	bilX_ = GLCameraFrustum::instance()->getBilboardVectorX();
	bilY_ = GLCameraFrustum::instance()->getBilboardVectorY();
}

void GLBilboardRenderer::addEntry(GLBilboardOrderedEntry *entry)
{
	entry->provider_ = this;
	entry->requiredItem_ = false;
	GLOrderedItemRenderer::instance()->addEntry(entry);
}

void GLBilboardRenderer::removeEntry(GLBilboardOrderedEntry *entry, bool deleteItem)
{
	GLOrderedItemRenderer::instance()->rmEntry(entry, deleteItem);
}

void GLBilboardRenderer::drawItem(float distance, GLOrderedItemRenderer::OrderedEntry &oentry)
{
	GLBilboardRenderer::GLBilboardOrderedEntry &entry =
		(GLBilboardRenderer::GLBilboardOrderedEntry &) oentry;
	if (!entry.texture) return;

	totalBilboards_++;
	if (entry.texture != GLTexture::getLastBind())
	{
		entry.texture->draw();
		totalSwitches_++;
	}
	glColor4f(entry.r_color, entry.g_color, entry.b_color, entry.alpha);

	float bilXX = bilX_[0] * entry.width;
	float bilXY = bilX_[1] * entry.width;
	float bilXZ = bilX_[2] * entry.width;
	float bilYX = bilY_[0] * entry.height;
	float bilYY = bilY_[1] * entry.height;
	float bilYZ = bilY_[2] * entry.height;

	glBegin(GL_QUADS);
	switch(entry.textureCoord)
	{
	default: glTexCoord2d(1.0f, 1.0f); break;
	case 1:  glTexCoord2d(0.0f, 1.0f); break;
	case 2:  glTexCoord2d(0.0f, 0.0f); break;
	case 3:  glTexCoord2d(1.0f, 0.0f); break;
	}
	glVertex3f(
		entry.posX + bilXX + bilYX, 
		entry.posY + bilXY + bilYY, 
		entry.posZ + bilXZ + bilYZ);
	switch(entry.textureCoord)
	{
	default: glTexCoord2d(0.0f, 1.0f); break;
	case 1:  glTexCoord2d(0.0f, 0.0f); break;
	case 2:  glTexCoord2d(1.0f, 0.0f); break;
	case 3:  glTexCoord2d(1.0f, 1.0f); break;
	}
	glVertex3f(
		entry.posX - bilXX + bilYX, 
		entry.posY - bilXY + bilYY, 
		entry.posZ - bilXZ + bilYZ);
	switch(entry.textureCoord)
	{
	default: glTexCoord2d(0.0f, 0.0f); break;
	case 1:  glTexCoord2d(1.0f, 0.0f); break;
	case 2:  glTexCoord2d(1.0f, 1.0f); break;
	case 3:  glTexCoord2d(0.0f, 1.0f); break;
	}
	glVertex3f(
		entry.posX - bilXX - bilYX, 
		entry.posY - bilXY - bilYY, 
		entry.posZ - bilXZ - bilYZ);
	switch(entry.textureCoord)
	{
	default: glTexCoord2d(1.0f, 0.0f); break;
	case 1:  glTexCoord2d(1.0f, 1.0f); break;
	case 2:  glTexCoord2d(0.0f, 1.0f); break;
	case 3:  glTexCoord2d(0.0f, 0.0f); break;
	}
	glVertex3f(
		entry.posX + bilXX - bilYX, 
		entry.posY + bilXY - bilYY, 
		entry.posZ + bilXZ - bilYZ);
	
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

}

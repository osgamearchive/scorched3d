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
	totalTime_(0.0f), totalSwitches_(0), totalBilboards_(0), showMessages_(false),
	bilboardsThisFrame_(0)
{
	new GLConsoleRuleFnIBooleanAdapter("BilboardStats", showMessages_);
}

GLBilboardRenderer::~GLBilboardRenderer()
{
}

void GLBilboardRenderer::simulate(const unsigned int state, float simTime)
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

void GLBilboardRenderer::addEntry(Entry *entry)
{
	Vector &cameraPos = 
		MainCamera::instance()->getCamera().getCurrentPos();

	float dist = 0.0f;
	dist += (cameraPos[0] - entry->posX) * (cameraPos[0] - entry->posX);
	dist += (cameraPos[1] - entry->posY) * (cameraPos[1] - entry->posY);
	dist += (cameraPos[2] - entry->posZ) * (cameraPos[2] - entry->posZ);

	++bilboardsThisFrame_;
	entries_.insert(std::pair<float, Entry *>(dist,entry));
}

void GLBilboardRenderer::draw(const unsigned state)
{
	float bilboardsAllowed = 
		float(OptionsDisplay::instance()->getNumberBilboards()) /
		float(bilboardsThisFrame_);
	bilboardsThisFrame_ = 0;

	// Setup the transparecy and textures
	GLState drawState(GLState::TEXTURE_ON | GLState::BLEND_ON | GLState::DEPTH_ON);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Setup the bilboard
	Vector &bilX = GLCameraFrustum::instance()->getBilboardVectorX();
	Vector &bilY = GLCameraFrustum::instance()->getBilboardVectorY();
	GLTexture *lastTexture = 0;
	
	float bilboardCount = 0.0f;
	float bilboardDrawn = 0.0f;
	std::multimap<float, Entry *>::reverse_iterator itor;
	std::multimap<float, Entry *>::reverse_iterator endItor = entries_.rend();
	for (itor = entries_.rbegin();
			itor != endItor;
			itor++)
	{
		bilboardCount += bilboardsAllowed;
		if (bilboardDrawn > bilboardCount) continue;
		
		bilboardDrawn += 1.0f;
		Entry *entry = (*itor).second;

		totalBilboards_++;
		if (entry->texture != lastTexture)
		{
			entry->texture->draw();
			totalSwitches_++;
			lastTexture = entry->texture;
		}
		glColor4f(1.0f, 1.0f, 1.0f, entry->alpha);

		float bilXX = bilX[0] * entry->width;
		float bilXY = bilX[1] * entry->width;
		float bilXZ = bilX[2] * entry->width;
		float bilYX = bilY[0] * entry->height;
		float bilYY = bilY[1] * entry->height;
		float bilYZ = bilY[2] * entry->height;

		glBegin(GL_QUADS);
			switch(entry->textureCoord)
			{
				default: glTexCoord2d(1.0f, 1.0f); break;
				case 1:  glTexCoord2d(0.0f, 1.0f); break;
				case 2:  glTexCoord2d(0.0f, 0.0f); break;
				case 3:  glTexCoord2d(1.0f, 0.0f); break;
			}
			glVertex3f(
				entry->posX + bilXX + bilYX, 
				entry->posY + bilXY + bilYY, 
				entry->posZ + bilXZ + bilYZ);
			switch(entry->textureCoord)
			{
				default: glTexCoord2d(0.0f, 1.0f); break;
				case 1:  glTexCoord2d(0.0f, 0.0f); break;
				case 2:  glTexCoord2d(1.0f, 0.0f); break;
				case 3:  glTexCoord2d(1.0f, 1.0f); break;
			}
			glVertex3f(
				entry->posX - bilXX + bilYX, 
				entry->posY - bilXY + bilYY, 
				entry->posZ - bilXZ + bilYZ);
			switch(entry->textureCoord)
			{
				default: glTexCoord2d(0.0f, 0.0f); break;
				case 1:  glTexCoord2d(1.0f, 0.0f); break;
				case 2:  glTexCoord2d(1.0f, 1.0f); break;
				case 3:  glTexCoord2d(0.0f, 1.0f); break;
			}
			glVertex3f(
				entry->posX - bilXX - bilYX, 
				entry->posY - bilXY - bilYY, 
				entry->posZ - bilXZ - bilYZ);
			switch(entry->textureCoord)
			{
				default: glTexCoord2d(1.0f, 0.0f); break;
				case 1:  glTexCoord2d(1.0f, 1.0f); break;
				case 2:  glTexCoord2d(0.0f, 1.0f); break;
				case 3:  glTexCoord2d(0.0f, 0.0f); break;
			}
			glVertex3f(
				entry->posX + bilXX - bilYX, 
				entry->posY + bilXY - bilYY, 
				entry->posZ + bilXZ - bilYZ);
		glEnd();
	}
	glDepthMask(GL_TRUE);

	entries_.clear();
}

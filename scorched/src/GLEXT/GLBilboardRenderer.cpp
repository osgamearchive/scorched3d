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
#include <client/MainCamera.h>

GLBilboardRenderer *GLBilboardRenderer::instance_ = 0;

GLBilboardRenderer *GLBilboardRenderer::instance()
{
	if (!instance_)
	{
		instance_ = new GLBilboardRenderer;
	}
	return instance_;
}

GLBilboardRenderer::GLBilboardRenderer()
{
}

GLBilboardRenderer::~GLBilboardRenderer()
{
}

void GLBilboardRenderer::addEntry(Entry *entry)
{
	Vector &cameraPos = 
		MainCamera::instance()->getCamera().getCurrentPos();

	float dist = 0.0f;
	dist += (cameraPos[0] - entry->posX) * (cameraPos[0] - entry->posX);
	dist += (cameraPos[1] - entry->posY) * (cameraPos[1] - entry->posY);
	dist += (cameraPos[2] - entry->posZ) * (cameraPos[2] - entry->posZ);

	entries_.insert(std::pair<float, Entry *>(dist,entry));
}

void GLBilboardRenderer::draw(const unsigned state)
{
	// Setup the transparecy and textures
	GLState drawState(GLState::TEXTURE_ON | GLState::BLEND_ON | GLState::DEPTH_ON);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Setup the bilboard
	Vector &bilX = GLCameraFrustum::instance()->getBilboardVectorX();
	Vector &bilY = GLCameraFrustum::instance()->getBilboardVectorY();
	
	std::multimap<float, Entry *>::reverse_iterator itor;
	std::multimap<float, Entry *>::reverse_iterator endItor = entries_.rend();
	for (itor = entries_.rbegin();
			itor != endItor;
			itor++)
	{
		Entry *entry = (*itor).second;

		entry->texture->draw();
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

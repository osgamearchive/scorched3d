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
	totalTime_(0.0f), totalSwitches_(0), totalBilboards_(0), showMessages_(false),
	bilboardsThisFrame_(0)
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
	bilboardsAllowed_ = 
		float(OptionsDisplay::instance()->getNumberBilboards()) /
		float(bilboardsThisFrame_);
	bilboardsThisFrame_ = 0;
	bilboardCount_ = 0.0f;
	bilboardDrawn_ = 0.0f;
}

void GLBilboardRenderer::addEntry(Entry *entry)
{
	++bilboardsThisFrame_;
	entry->provider_ = this;
	GLOrderedItemRenderer::instance()->addEntry(entry);
}

void GLBilboardRenderer::drawItem(GLOrderedItemRenderer::OrderedEntry &oentry)
{
	GLBilboardRenderer::Entry &entry =
		(GLBilboardRenderer::Entry &) oentry;

	bilboardCount_ += bilboardsAllowed_;
	if (bilboardDrawn_ > bilboardCount_) return;	
	bilboardDrawn_ += 1.0f;

	totalBilboards_++;
	if (entry.texture != GLTexture::getLastBind())
	{
		entry.texture->draw();
		totalSwitches_++;
	}
	if (entry.style==1){
		float r = 0;
		float g = 0;
		float b = 0;
		r=1.0f;
		if (entry.alpha>=0.5f){
			b=(1.0f - (entry.alpha * 2.0f));
			if (g<0) g*=-1.0f;
			if (g>1.0f) g=1.0f;
		}else{
			g=(1.0f - (entry.alpha * 2.0f));
			if (g<0) g*=-1.0f;
			if (g>1.0f) g=1.0f;
		}
		if (entry.alpha>=0.5f){
			b=0.0f;
		}else{
			b=(1.0f - (entry.alpha * 2.0f));
			if (b<0) b*=-1.0f;
			if (g>1.0f) g=1.0f;
		}
		glColor4f(r, g, b, entry.alpha);
	}else{
		glColor4f(1.0f, 1.0f, 1.0f, entry.alpha);
	}

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

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

#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLBitmap.h>
#include <sprites/TalkRenderer.h>

GLTexture TalkRenderer::talkTex_;

TalkRenderer::TalkRenderer(Vector &position) : 
	frameTime_(0.0f), position_(position)
{
	// Load the speach bubble talk texture
	if (!talkTex_.textureValid())
	{
		GLBitmap talkBitmap( PKGDIR "data/textures/talk.bmp", true);
		talkTex_.create(talkBitmap, GL_RGBA);
	}

	bilEntry_ = new GLBilboardRenderer::GLBilboardOrderedEntry;
	bilEntry_->texture = &talkTex_;
	bilEntry_->alpha = 1.0f;
	bilEntry_->width = 2.0f; bilEntry_->height = 2.0f;
	bilEntry_->textureCoord = 0;
	GLBilboardRenderer::instance()->addEntry(bilEntry_);
}

TalkRenderer::~TalkRenderer()
{
	GLBilboardRenderer::instance()->removeEntry(bilEntry_);
}

void TalkRenderer::draw(Action *action)
{
	Vector &bilX = GLCameraFrustum::instance()->getBilboardVectorX();
	Vector &bilY = GLCameraFrustum::instance()->getBilboardVectorY();

	bilEntry_->posX = position_[0] + 4.0f * bilX[0];
	bilEntry_->posY = position_[1] + 4.0f * bilX[1];
	bilEntry_->posZ = position_[2] + 5.0f;
}

void TalkRenderer::simulate(Action *action, float timepassed, bool &remove)
{
	frameTime_ += timepassed;
	remove = (frameTime_ > 8.0f);
}

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


// TalkRenderer.cpp: implementation of the TalkRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLBitmap.h>
#include <sprites/TalkRenderer.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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
}

TalkRenderer::~TalkRenderer()
{

}

void TalkRenderer::draw(Action *action)
{
	GLState currentState(GLState::DEPTH_ON | GLState::TEXTURE_ON | GLState::BLEND_ON);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	talkTex_.draw(true);
	Vector &bilX = GLCameraFrustum::instance()->getBilboardVectorX();
	Vector &bilY = GLCameraFrustum::instance()->getBilboardVectorY();

	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
		glTranslatef(
			position_[0] + 4.0f * bilX[0], 
			position_[1] + 4.0f * bilX[1], 
			position_[2] + 5.0f);
		glScalef(2.0f, 2.0f, 2.0f);
		glBegin(GL_QUADS);
			glTexCoord2d(1.0f, 0.0f);
			glVertex3fv( bilX - bilY);
			glTexCoord2d(1.0f, 1.0f);
			glVertex3fv( bilX + bilY);
			glTexCoord2d(0.0f, 1.0f);
			glVertex3fv( -bilX + bilY);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3fv( -bilX - bilY);
		glEnd();
	glPopMatrix();
	glDepthMask(GL_TRUE);
}

void TalkRenderer::simulate(Action *action, float timepassed, bool &remove)
{
	frameTime_ += timepassed;
	remove = (frameTime_ > 8.0f);
}

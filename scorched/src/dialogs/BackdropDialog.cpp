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

#include <GLEXT/GLViewPort.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLBitmap.h>
#include <dialogs/BackdropDialog.h>

GLTexture BackdropDialog::backTex_ = GLTexture();
GLTexture BackdropDialog::titleTex_ = GLTexture();
GLuint BackdropDialog::displayList_ = 0;
BackdropDialog *BackdropDialog::instance_ = 0;

BackdropDialog *BackdropDialog::instance()
{
	if (!instance_)
	{
		instance_ = new BackdropDialog;
	}
	return instance_;
}

BackdropDialog::BackdropDialog() : 
	GLWWindow("", 0.0f, 0.0f, 0.0f, 0.0f, 0,
		"The backdrop dialog"), offset_(0)
{
	if (!displayList_)
	{
		GLBitmap backMap(getDataFile("data/windows/logotiled.bmp"));
		backTex_.create(backMap, GL_RGB, false);

		// Use string temporaries to get around use of statics in getDataFile
		std::string file1 = getDataFile("data/windows/scorched.bmp");
		std::string file2 = getDataFile("data/windows/scorchedi.bmp");
		GLBitmap topMap( file1.c_str(), file2.c_str(), true);
		titleTex_.create(topMap, GL_RGBA, false);

		displayList_ = 1;
	}
}

BackdropDialog::~BackdropDialog()
{
}

void BackdropDialog::draw()
{
	GLState currentState(GLState::DEPTH_OFF | GLState::TEXTURE_ON);

	// Calcuate how may tiles are needed
	float wWidth = (float) GLViewPort::getWidth();
	float wHeight = (float) GLViewPort::getHeight();
	float xScale = wWidth / 128.0f;
	float yScale = wHeight / 128.0f;

	// Draw the tiled logo backdrop
	backTex_.draw(true);
	glColor3f(0.3f, 0.3f, 0.7f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f - offset_, 0.0f + offset_);
		glVertex2f(0.0f, 0.0f);
		glTexCoord2f(xScale - offset_, 0.0f + offset_);
		glVertex2f(wWidth, 0.0f);
		glTexCoord2f(xScale - offset_, yScale + offset_);
		glVertex2f(wWidth, wHeight);
		glTexCoord2f(0.0f - offset_, yScale + offset_);
		glVertex2f(0.0f, wHeight);
	glEnd();	

	{
		glPushMatrix();
			glTranslatef(wWidth / 2 - 256, wHeight - 64, 0.0f);
			GLState currentStateBlend(GLState::BLEND_ON);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
			titleTex_.draw();
			glColor3f(1.0f, 1.0f, 1.0f);
			glBegin(GL_QUADS);
				glTexCoord2d(0.0f, 0.0f);
				glVertex2f(0.0f, 0.0f);
				glTexCoord2d(1.0f, 0.0f);
				glVertex2f(512.0f, 0.0f);
				glTexCoord2d(1.0f, 1.0f);
				glVertex2f(512.0f, 64.0f);
				glTexCoord2d(0.0f, 1.0f);
				glVertex2f(0.0f, 64.0f);
			glEnd();
			glPopMatrix();
	}
}


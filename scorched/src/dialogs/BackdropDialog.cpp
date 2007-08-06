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
#include <GLEXT/GLImageFactory.h>
#include <graph/Main2DCamera.h>
#include <dialogs/BackdropDialog.h>
#include <common/Defines.h>

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
		"The backdrop dialog"),
	pixels_(0)
{
	windowLevel_ = 5000000;

	GLImageHandle backMap = GLImageFactory::loadImageHandle(
		getDataFile("data/windows/backdrop.jpg"));
		//getDataFile("data/windows/logotiled.bmp"));
	backTex_.create(backMap, false);
}

BackdropDialog::~BackdropDialog()
{
}

void BackdropDialog::draw()
{
	if (!pixels_)
	{
		drawBackground();
	}
	else
	{
		GLState state(GLState::DEPTH_OFF | GLState::TEXTURE_OFF);

		glColor3f(1.0f, 1.0f, 1.0f);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

		glRasterPos2i(0, 0);
		glDrawPixels(GLViewPort::getActualWidth(), GLViewPort::getActualHeight(), 
			GL_RGB, GL_UNSIGNED_BYTE, pixels_);
	}

	drawLogo();
}

void BackdropDialog::drawBackground()
{
	GLState currentState(GLState::DEPTH_OFF | GLState::TEXTURE_ON);

	// Calcuate how may tiles are needed
	float wWidth = (float) GLViewPort::getWidth();
	float wHeight = (float) GLViewPort::getHeight();

	// Draw the tiled logo backdrop
	backTex_.draw(true);
	glColor3f(1.0f, 1.0f, 1.0f);//0.2f, 0.2f, 0.2f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(wWidth, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(wWidth, wHeight);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f, wHeight);
	glEnd();	
}

void BackdropDialog::drawBackgroundTiled()
{
	GLState currentState(GLState::DEPTH_OFF | GLState::TEXTURE_ON);

	// Calcuate how may tiles are needed
	float wWidth = (float) GLViewPort::getWidth();
	float wHeight = (float) GLViewPort::getHeight();
	float xScale = wWidth / 128.0f;
	float yScale = wHeight / 128.0f;

	// Draw the tiled logo backdrop
	float offset_ = 0.0f;
	backTex_.draw(true);
	glColor3f(0.2f, 0.2f, 0.2f);
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
}

void BackdropDialog::drawLogo()
{
	if (0 != strcmp(getDataFileMod(), lastMod_.c_str()))
	{
		lastMod_ = getDataFileMod();

		GLImageHandle logoMap = GLImageFactory::loadImageHandle(
			formatString(getDataFile("data/windows/scorched.jpg")),
			formatString(getDataFile("data/windows/scorcheda.jpg")),
			false);
		logoTex_.create(logoMap, false);
	}

	GLState currentState(GLState::DEPTH_OFF | GLState::BLEND_ON | GLState::TEXTURE_ON);

	// Calcuate how may tiles are needed
	float wWidth = (float) GLViewPort::getWidth();
	float wHeight = (float) GLViewPort::getHeight();

	// Draw the higer rez logo
	const float logoWidth = 480.0f;
	const float logoHeight = 90.0f;
	logoTex_.draw(true);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
		glTranslatef(
			wWidth - logoWidth - 50.0f, 
			wHeight - logoHeight - 50.0f, 
			0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(logoWidth, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(logoWidth, logoHeight);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(0.0f, logoHeight);
		glEnd();
	glPopMatrix();
}

void BackdropDialog::capture()
{
	glRasterPos2i(0, 0);

	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

	unsigned char *screenpixels = 
		new unsigned char[GLViewPort::getActualWidth() * GLViewPort::getActualHeight() * 3];
	glReadPixels(0, 0, GLViewPort::getActualWidth(), GLViewPort::getActualHeight(), 
		GL_RGB, GL_UNSIGNED_BYTE, screenpixels);

	if (!pixels_) pixels_ = new unsigned char[GLViewPort::getActualWidth() * GLViewPort::getActualHeight() * 3];

	unsigned char *dest = pixels_;
	unsigned char *src = screenpixels;
	for (int y=0; y<GLViewPort::getActualHeight(); y++)
	{
		for (int x=0; x<GLViewPort::getActualWidth(); x++, dest+=3, src+=3)
		{
			int totalr = 0;
			int totalg = 0;
			int totalb = 0;
			if (x>=3 && x<GLViewPort::getActualWidth()-3 &&
				y>=3 && y<GLViewPort::getActualHeight()-3)
			{
				for (int a=-3; a<=3; a++)
				{
					for (int b=-3; b<=3; b++)
					{
						int srcx = a + x;
						int srcy = b + y;
						unsigned char *src2 = src + a * 3 + b * GLViewPort::getActualWidth() * 3;

						totalr += src2[0];
						totalg += src2[1];
						totalb += src2[2];
					}
				}
				totalr /= 49;
				totalg /= 49;
				totalb /= 49;
			}
			else
			{
				totalr = src[0];
				totalg = src[1];
				totalb = src[2];
			}

			dest[0] = totalr;
			dest[1] = totalg;
			dest[2] = totalb;
		}
	}

	delete [] screenpixels;
}

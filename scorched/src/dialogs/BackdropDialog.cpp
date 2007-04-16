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
		"The backdrop dialog")
{
	windowLevel_ = 5000000;

	GLImageHandle backMap = GLImageFactory::loadImageHandle(
		getDataFile("data/windows/backdrop.png"));
	backTex_.create(backMap, GL_RGB, false);

	/*GLImageHandle logoMap = GLImageFactory::loadImageHandle(
		getDataFile("data/windows/scorched.jpg"));
	logoTex_.create(logoMap, GL_RGB, false);*/
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

	// Draw the higer rez logo
	/*const float logoWidth = 320.0f;
	const float logoHeight = 40.0f;
	logoTex_.draw(true);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
		glTranslatef(10.0f, 10.0f, 0.0f);
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
	glPopMatrix();*/
}


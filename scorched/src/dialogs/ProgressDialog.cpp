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

#include <dialogs/ProgressDialog.h>
#include <dialogs/BackdropDialog.h>
#include <dialogs/HelpButtonDialog.h>
#include <client/ScorchedClient.h>
#include <client/Main2DCamera.h>
#include <client/ClientMain.h>
#include <engine/MainLoop.h>
#include <common/Clock.h>
#include <common/Defines.h>
#include <GLW/GLWFont.h>
#include <GLEXT/GLBitmap.h>
#include <math.h>
#include <string.h>

ProgressDialog *ProgressDialog::instance_ = 0;

ProgressDialog *ProgressDialog::instance()
{
	if (!instance_)
	{
		instance_ = new ProgressDialog;
	}
	return instance_;
}

ProgressDialog::ProgressDialog() : 
	GLWWindow("Progress", 10.0f, 10.0f, 420.0f, 300.0f, eNoTitle,
		"Shows loading progress")
{
	setUser(this);
	progress_ = (GLWProgress *) new GLWProgress(10, 10, 260);
	progressLabel_ = (GLWLabel *) new GLWLabel(10, 30);
}

ProgressDialog::~ProgressDialog()
{
}

void ProgressDialog::changeTip()
{
	std::string file1 = getDataFile("data/windows/wait.bmp");
	std::string file2 = getDataFile("data/windows/waita.bmp");
	GLBitmap map(file1.c_str(), file2.c_str());
	DIALOG_ASSERT(wait_.create(map, GL_RGBA, false));

	tips_.getLines().clear();
	tips_.readFile((char *) getDataFile("data/tips.txt"));
	needsCentered();

	const char *tip = tips_.getRandomLine();
	char *nl = (char *) strchr(tip, ':');
	if (nl)
	{
		*nl = '\0';
		tip1_ = tip;
		tip2_ = nl + 1;
		*nl = ':';
	}
	else
	{
		tip1_ = tip;
		tip2_ = "";
	}
}

void ProgressDialog::draw()
{
	GLWWindow::draw();

	{
		GLState state(GLState::DEPTH_OFF | GLState::TEXTURE_ON | GLState::BLEND_ON);
		wait_.draw();

		glPushMatrix();
			glTranslatef(x_, y_ + 20.0f, 0.0f);
			{
				glColor3f(1.0f, 1.0f, 1.0f);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f);
					glVertex2f(20.0f, 60.0f);
					glTexCoord2f(1.0f, 0.0f);
					glVertex2f(400.0f, 60.0f);
					glTexCoord2f(1.0f, 1.0f);
					glVertex2f(400.0f, 260.0f);
					glTexCoord2f(0.0f, 1.0f);
					glVertex2f(20.0f, 260.0f);
				glEnd();
			}

			{
				GLState state3(GLState::TEXTURE_OFF);

				glColor3f(0.6f, 0.0f, 0.0f);
				glBegin(GL_QUADS);
					glVertex2f(20.0f, 21.0f);
					glVertex2f(20.0f + 380.0f * progress_->getCurrent() / 100.0f, 21.0f);
					glVertex2f(20.0f + 380.0f * progress_->getCurrent() / 100.0f, 26.0f);
					glVertex2f(20.0f, 26.0f);
				glEnd();
				glColor3f(0.2f, 0.2f, 0.2f);
				glBegin(GL_LINE_LOOP);
					glVertex2f(20.0f, 21.0f);
					glVertex2f(400.0f, 21.0f);
					glVertex2f(400.0f, 26.0f);
					glVertex2f(20.0f, 26.0f);
				glEnd();
			}

			Vector color(0.2f, 0.2f, 0.2f);
			GLWFont::instance()->getLargePtFont()->draw(color, 
				14.0f, 20.0f, 33.0f, 0.0f, 
				progressLabel_->getText());

			HelpButtonDialog::instance()->getHelpTexture().draw();
			glPushMatrix();
				glColor3f(1.0f, 1.0f, 1.0f);
				glTranslatef(18.0f, -11.0f, 0.0f);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f);
					glVertex2f(0.0f, 0.0f);
					glTexCoord2f(1.0f, 0.0f);
					glVertex2f(22.0f, 0.0f);
					glTexCoord2f(1.0f, 1.0f);
					glVertex2f(22.0f, 22.0f);
					glTexCoord2f(0.0f, 1.0f);
					glVertex2f(0.0f, 22.0f);
				glEnd();
			glPopMatrix();

			Vector color2(0.4f, 0.4f, 0.4f);
			GLWFont::instance()->getLargePtFont()->drawWidth(380, 
				color2, 10.0f, 44.0f, 3.0f, 0.0f, 
				tip1_.c_str());
			GLWFont::instance()->getLargePtFont()->drawWidth(380, 
				color2, 10.0f, 44.0f, -12.0f, 0.0f, 
				tip2_.c_str());
		glPopMatrix();
	}
}

void ProgressDialog::progressChange(const char *op, const float percentage)
{
	static Clock localTimer;
	static float timeDelay = 0.0f;
	timeDelay += localTimer.getTimeDifference();

	clientEventLoop();	

	progressLabel_->setText(op);
	progress_->setCurrent(percentage);

	//for (int i=0; i<100000000; i++);

	if ((timeDelay > 0.25f) || 
		(percentage > 99.0f))
	{
		timeDelay = 0.0f;
	
		Main2DCamera::instance()->draw(0);
		BackdropDialog::instance()->draw();
		draw();

		ScorchedClient::instance()->getMainLoop().swapBuffers();
	}
}


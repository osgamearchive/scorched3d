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
#include <client/ScorchedClient.h>
#include <GLW/GLWFont.h>
#include <GLEXT/GLBitmap.h>
#include <math.h>

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
	GLWWindow("Progress", 10.0f, 10.0f, 340.0f, 200.0f, eNoTitle)
{
	setUser(this);
	progress_ = (GLWProgress *) new GLWProgress(10, 10, 260);
	progressLabel_ = (GLWLabel *) new GLWLabel(10, 30);
	GLBitmap map(PKGDIR "data/windows/wait.bmp", PKGDIR "data/windows/waita.bmp");
	DIALOG_ASSERT(wait_.create(map, GL_RGBA, false));

	needsCentered();
}

ProgressDialog::~ProgressDialog()
{
}

void ProgressDialog::draw()
{
	GLWWindow::draw();

	{
		GLState state(GLState::DEPTH_OFF);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		wait_.draw();

		glPushMatrix();
			glTranslatef(x_, y_, 0.0f);
			{
				GLState state2(GLState::TEXTURE_ON | GLState::BLEND_ON);
				glColor3f(1.0f, 1.0f, 1.0f);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f);
					glVertex2f(20.0f, 50.0f);
					glTexCoord2f(1.0f, 0.0f);
					glVertex2f(320.0f, 50.0f);
					glTexCoord2f(1.0f, 1.0f);
					glVertex2f(320.0f, 180.0f);
					glTexCoord2f(0.0f, 1.0f);
					glVertex2f(20.0f, 180.0f);
				glEnd();
			}

			{
				GLState state3(GLState::TEXTURE_OFF);

				glColor3f(0.6f, 0.0f, 0.0f);
				glBegin(GL_QUADS);
					glVertex2f(20.0f, 15.0f);
					glVertex2f(20.0f + 300.0f * progress_->getCurrent() / 100.0f, 15.0f);
					glVertex2f(20.0f + 300.0f * progress_->getCurrent() / 100.0f, 21.0f);
					glVertex2f(20.0f, 21.0f);
				glEnd();
				glColor3f(0.2f, 0.2f, 0.2f);
				glBegin(GL_LINE_LOOP);
					glVertex2f(20.0f, 15.0f);
					glVertex2f(320.0f, 15.0f);
					glVertex2f(320.0f, 21.0f);
					glVertex2f(20.0f, 21.0f);
				glEnd();
			}

			Vector color(0.2f, 0.2f, 0.2f);
			GLWFont::instance()->getFont()->draw(color, 14.0f, 20.0f, 25.0f, 0.0f, 
				progressLabel_->getText());
		glPopMatrix();
	}
}

void ProgressDialog::progressChange(const char *op, const float percentage)
{
	static Timer localTimer;
	static float timeDelay = 0.0f;
	timeDelay += localTimer.getTimeDifference();

	static float oldPercentage = 0.0f;

	progressLabel_->setText(op);
	progress_->setCurrent(percentage);

	if ((timeDelay > 0.25f) || (percentage > 99.9f) || 
		(fabsf(oldPercentage - percentage) > 5.0f))
	{
		oldPercentage = percentage;
		timeDelay = 0.0f;
	
		BackdropDialog::instance()->draw();
		draw();

		ScorchedClient::instance()->getMainLoop().swapBuffers();
	}
}
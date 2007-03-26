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
#include <landscape/LandscapeMusicManager.h>
#include <sound/Sound.h>
#include <client/ScorchedClient.h>
#include <client/ClientChannelManager.h>
#include <graph/Main2DCamera.h>
#include <client/ClientMain.h>
#include <client/ClientState.h>
#include <engine/MainLoop.h>
#include <common/Clock.h>
#include <common/Defines.h>
#include <common/ChannelText.h>
#include <GLW/GLWFont.h>
#include <GLW/GLWWindowManager.h>
#include <GLEXT/GLPng.h>
#include <GLEXT/GLBitmap.h>
#include <math.h>
#include <string.h>

ProgressDialog *ProgressDialog::instance_ = 0;

ProgressDialog *ProgressDialog::instance()
{
	if (!instance_) instance_ = new ProgressDialog();
	return instance_;
}

ProgressDialog::ProgressDialog() : 
	GLWWindow("", 10.0f, 10.0f, 450.0f, 60.0f, eTransparent | eNoTitle, ""),
	progressPercentage_(0)
{
	setUser(this);
}

ProgressDialog::~ProgressDialog()
{
}

void ProgressDialog::progressChange(const char *op, const float percentage)
{
	progressText_ = op;
	progressPercentage_ = percentage;
}

void ProgressDialog::changeTip()
{
	tips_.getLines().clear();
	tips_.readFile((char *) getDataFile("data/tips.txt"));
	needsCentered();

	std::string tip = tips_.getLines()[rand() % tips_.getLines().size()].c_str();
	char *nl = (char *) strchr(tip.c_str(), ':');
	if (nl) *nl = ' ';

	ChannelText text("info", formatString("Tip: %s", tip.c_str()));
	ClientChannelManager::instance()->showText(text);
}

void ProgressDialog::setIcon(const char *iconName)
{
	GLBitmap map(iconName);
	map.resize(64, 64);
	icon_.create(map);
}

void ProgressDialog::draw()
{
	GLWWindow::draw();

	{
		GLState state(GLState::DEPTH_OFF | GLState::TEXTURE_ON | GLState::BLEND_ON);

		if (!icon_.textureValid())
		{
			setIcon(getDataFile("data/windows/tank2.bmp"));

			GLPng bar1(getDataFile("data/windows/bar1.png"), true);
			GLPng bar2(getDataFile("data/windows/bar2.png"), true);
			bar1_.create(bar1, GL_RGBA);
			bar2_.create(bar2, GL_RGBA);

			bar1_.draw(true);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

			bar2_.draw(true);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		}

		// Draw the progress bars
		glPushMatrix();
			glTranslatef(x_ + 60.0f, y_ + 0.0f, 0.0f);

			bar1_.draw();
			glColor3f(1.0f, 1.0f, 1.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(0.0f, 15.0f);
				glTexCoord2f(progressPercentage_ / 100.0f, 0.0f);
				glVertex2f(380.0f * progressPercentage_ / 100.0f, 15.0f);
				glTexCoord2f(progressPercentage_ / 100.0f, 1.0f);
				glVertex2f(380.0f * progressPercentage_ / 100.0f, 26.0f);
				glTexCoord2f(0.0, 1.0f);
				glVertex2f(0.0f, 26.0f);
			glEnd();
			bar2_.draw();
			glBegin(GL_QUADS);
				glTexCoord2f(progressPercentage_ / 100.0f, 0.0f);
				glVertex2f(380.0f * progressPercentage_ / 100.0f, 15.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(380.0f, 15.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(380.0f, 26.0f);
				glTexCoord2f(progressPercentage_ / 100.0f, 1.0f);
				glVertex2f(380.0f * progressPercentage_ / 100.0f, 26.0f);
			glEnd();
			
			// Draw the progress text
			Vector color(1.0f, 1.0f, 1.0f);
			GLWFont::instance()->getLargePtFont()->draw(color, 
				14.0f, 0.0f, 33.0f, 0.0f, 
				progressText_.c_str());
		glPopMatrix();

		// Draw the icon
		icon_.draw();
		glPushMatrix();
		{
			glTranslatef(x_ + 12.0f, y_ + 12.0f, 0.0f);
			glColor3f(1.0f, 1.0f, 1.0f);

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(0.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(36.0f, 0.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(36.0f, 36.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex2f(0.0f, 36.0f);
			glEnd();
		}
		{
			GLState state2(GLState::TEXTURE_OFF);
			glColor3f(0.0f, 0.0f, 0.0f);
			glLineWidth(2.0f);
			glBegin(GL_LINE_LOOP);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(0.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(36.0f, 0.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(36.0f, 36.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex2f(0.0f, 36.0f);
			glEnd();
			glLineWidth(1.0f);
		}
		glPopMatrix();
	}
}

ProgressDialogSync *ProgressDialogSync::instance_ = 0;

ProgressDialogSync *ProgressDialogSync::instance()
{
	if (!instance_) instance_ = new ProgressDialogSync();
	return instance_;
}

ProgressDialogSync::ProgressDialogSync()
{
	setUser(this);
}

ProgressDialogSync::~ProgressDialogSync()
{
}

void ProgressDialogSync::progressChange(const char *op, const float percentage)
{
	static Clock localTimer;
	static float timeDelay = 0.0f;
	float frameTime = localTimer.getTimeDifference();
	timeDelay += frameTime;

	ClientMain::clientEventLoop(frameTime);	

	ProgressDialog::instance()->progressChange(op, percentage);

	if ((timeDelay > 0.25f) || 
		(percentage > 99.0f))
	{
		Main2DCamera::instance()->draw(0);

		unsigned int state = ScorchedClient::instance()->getGameState().getState();
		if (state >= ClientState::StateGetPlayers)
		{
			GLWWindowManager::instance()->simulate(ClientState::StateLoadLevel, MIN(0.25f, timeDelay));
			GLWWindowManager::instance()->draw(ClientState::StateLoadLevel);
			LandscapeMusicManager::instance()->simulate(ClientState::StateLoadLevel, frameTime);
			Sound::instance()->simulate(ClientState::StateLoadLevel, frameTime);
		}
		else
		{
			BackdropDialog::instance()->draw();
			ProgressDialog::instance()->draw();
			LandscapeMusicManager::instance()->simulate(state, frameTime);
			Sound::instance()->simulate(state, frameTime);
		}

		ScorchedClient::instance()->getMainLoop().swapBuffers();

		timeDelay = 0.0f;
	}
}

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

#include <dialogs/SkipAllDialog.h>
#include <engine/MainLoop.h>
#include <GLW/GLWTextButton.h>
#include <GLW/GLWWindowManager.h>
#include <client/ScorchedClient.h>
#include <tankai/TankAIHuman.h>
#include <tank/TankContainer.h>
#include <common/Defines.h>
#include <time.h>

SkipAllDialog *SkipAllDialog::instance_ = 0;

SkipAllDialog *SkipAllDialog::instance()
{
	if (!instance_)
	{
		instance_ = new SkipAllDialog;
	}
	return instance_;
}

SkipAllDialog::SkipAllDialog() : 
	GLWWindow("", 250.0f, 80.0f, 0, ""),
	skipAll_(false)
{
	label_ = (GLWLabel *) addWidget(new GLWLabel(10, 45));
	cancelId_ = addWidget(new GLWTextButton("Cancel", 135, 10, 105, this, 
		GLWButton::ButtonFlagCancel | GLWButton::ButtonFlagCenterX))->getId();
}

SkipAllDialog::~SkipAllDialog()
{

}

void SkipAllDialog::simulate(float frameTime)
{
	if (skipAll_)
	{
		label_->setText(formatString("Skipping move in %i...", (int) timeLeft_));
		timeLeft_ -= ScorchedClient::instance()->getMainLoop().getDrawTime();
		if (timeLeft_ < 0.0f)
		{
			Tank *firstTank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
			if (firstTank)
			{
				TankAIHuman *tankAI = (TankAIHuman *) firstTank->getTankAI();
				if (tankAI)
				{
					tankAI->skipShot();
				}
			}
			GLWWindowManager::instance()->hideWindow(id_);
		}
	}

	GLWWindow::simulate(frameTime);
}

void SkipAllDialog::windowInit(const unsigned state)
{
	timeLeft_ = 5.0f;
	skipAll_ = false;

	Tank *firstTank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (firstTank)
	{
		if (firstTank->getState().getSkipShots())
		{
			skipAll_ = true;
		}
	}

	if (skipAll_)
	{
		GLWWindowManager::instance()->showWindow(id_);
	}
	else
	{
		GLWWindowManager::instance()->hideWindow(id_);
	}
}

void SkipAllDialog::buttonDown(unsigned int id)
{
	if (id == cancelId_)
	{
		Tank *firstTank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
		if (firstTank)
		{
			firstTank->getState().setSkipShots(false);
		}
		GLWWindowManager::instance()->hideWindow(id_);
	}
}

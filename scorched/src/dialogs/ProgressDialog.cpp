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
	GLWWindow("Progress", 10.0f, 10.0f, 280.0f, 65.0f, eNoTitle)
{
	setUser(this);
	progress_ = (GLWProgress *) addWidget(new GLWProgress(10, 10, 260));
	progressLabel_ = (GLWLabel *) addWidget(new GLWLabel(10, 30));
	needsCentered();
}

ProgressDialog::~ProgressDialog()
{
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
		GLWWindow::draw();
		ScorchedClient::instance()->getMainLoop().swapBuffers();
	}
}
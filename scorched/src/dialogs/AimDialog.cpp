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

#include <dialogs/AimDialog.h>
#include <client/ScorchedClient.h>

AimDialog *AimDialog::instance_ = 0;

AimDialog *AimDialog::instance()
{
	if (!instance_)
	{
		instance_ = new AimDialog();
	}
	return instance_;
}

AimDialog::AimDialog() : 
	GLWWindow("Aim", 10, 100, 120, 120, eCircle | eSmallTitle)
{
	pwrSlider_ = (GLWSlider *) 
		addWidget(new GLWSlider(140, 10, 100, 0.0f, 100.0f));
	pwrSlider_->setHandler(this);

	tracker_ = (GLWTracker *)
		addWidget(new GLWTracker(10.0f, 10.0f, 130.0f, 100.0f));
	tracker_->setHandler(this);
}

AimDialog::~AimDialog()
{

}

void AimDialog::draw()
{
	Tank *currentTank =
		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (currentTank)
	{
		if (currentTank->getState().getState() == TankState::sNormal)
		{
			pwrSlider_->setCurrent(currentTank->getState().getPower());
			tracker_->setCurrentX(currentTank->getPhysics().getRotationGunXY());
			tracker_->setCurrentY(currentTank->getPhysics().getRotationGunYZ());
		}
	}
	
	GLWWindow::draw();
}

void AimDialog::currentChanged(unsigned int id, float valueX, float valueY)
{
	Tank *currentTank =
		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (currentTank)
	{
		if (currentTank->getState().getState() == TankState::sNormal)
		{
			if (id == tracker_->getId())
			{
				currentTank->getPhysics().rotateGunXY(valueX, false);
				currentTank->getPhysics().rotateGunYZ(valueY, false);
			}
			
		}
	}	
}

void AimDialog::currentChanged(unsigned int id, float value)
{
	Tank *currentTank =
		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (currentTank)
	{
		if (currentTank->getState().getState() == TankState::sNormal)
		{
			if (id == pwrSlider_->getId())
			{
				currentTank->getState().changePower(value, false);
			}
		}
	}	
}

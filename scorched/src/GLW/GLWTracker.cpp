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

#include <GLW/GLWTracker.h>
#include <GLW/GLWFont.h>
#include <GLEXT/GLState.h>
#include <common/Keyboard.h>
#include <common/OptionsDisplay.h>
#include <client/ScorchedClient.h>

REGISTER_CLASS_SOURCE(GLWTracker);

GLWTracker::GLWTracker(float x, float y, float w, float range) :
	GLWVisibleWidget(x, y, w, w), 
	range_(range),
	dragging_(false), handler_(0), currentX_(0.0f), currentY_(0.0f)
{

}

GLWTracker::~GLWTracker()
{

}

void GLWTracker::mouseDown(float x, float y, bool &skipRest)
{
	if (inBox(x, y, x_, y_, w_, h_))
	{
		skipRest = true;
		dragging_ = true;
	}
}

void GLWTracker::mouseUp(float x, float y, bool &skipRest)
{
	dragging_ = false;
}

void GLWTracker::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	if (dragging_)
	{            
		float rangeMult = 1.0f;
		unsigned int keyState = 
			Keyboard::instance()->getKeyboardState();
		if (keyState & KMOD_LSHIFT) rangeMult = 0.5f;

		currentX_ -= x / w_ * range_ * rangeMult;
		if (OptionsDisplay::instance()->getInvertUpDownKey())
		{
			rangeMult = -rangeMult;
		}
		currentY_ -= y /w_ * range_ * rangeMult;
		if (handler_) handler_->currentChanged(getId(), currentX_, currentY_);

		skipRest = true;
	}
}

REGISTER_CLASS_SOURCE(GLWTankTracker);

GLWTankTracker::GLWTankTracker() : 
	GLWTracker(0.0f, 0.0f, 0.0f, 100.0f)
{
	setHandler(this);
	setToolTip(new GLWTip("Rotation/Elevation",
		"Change the rotation and elevation of the\n"
		"current tank by clicking with the left\n"
		"mouse button and dragging up and down,\n"
		"left and right."));
}

GLWTankTracker::~GLWTankTracker()
{
}

void GLWTankTracker::draw()
{
	Tank *currentTank =
		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (currentTank)
	{
		if (currentTank->getState().getState() == TankState::sNormal)
		{
			setCurrentX(currentTank->getPhysics().getRotationGunXY());
			setCurrentY(currentTank->getPhysics().getRotationGunYZ());
		}
	}
	GLWTracker::draw();
}

void GLWTankTracker::currentChanged(unsigned int id, float valueX, float valueY)
{
	Tank *currentTank =
		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (currentTank)
	{
		if (currentTank->getState().getState() == TankState::sNormal)
		{
			if (id == getId())
			{
				currentTank->getPhysics().rotateGunXY(valueX, false);
				currentTank->getPhysics().rotateGunYZ(valueY, false);
			}
		}
	}	
}

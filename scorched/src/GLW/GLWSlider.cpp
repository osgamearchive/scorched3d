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

#include <GLW/GLWSlider.h>
#include <GLW/GLWFont.h>
#include <GLEXT/GLState.h>
#include <common/Keyboard.h>
#include <client/ScorchedClient.h>
#include <tank/TankContainer.h>

REGISTER_CLASS_SOURCE(GLWSlider);

GLWSlider::GLWSlider(float x, float y, float w,  float range) :
	GLWidget(x, y, w, 20.0f), 
	range_(range),
	dragging_(false), handler_(0)
{

}

GLWSlider::~GLWSlider()
{

}

void GLWSlider::mouseDown(int button, float x, float y, bool &skipRest)
{
	if (inBox(x, y, x_, y_, w_, h_))
	{
		skipRest = true;
		dragging_ = true;
	}
}

void GLWSlider::mouseUp(int button, float x, float y, bool &skipRest)
{
	dragging_ = false;
}

void GLWSlider::mouseDrag(int button, float mx, float my, float x, float y, bool &skipRest)
{
	if (dragging_)
	{
		float rangeMult = 1.0f;
		unsigned int keyState = 
			Keyboard::instance()->getKeyboardState();
		if (keyState & KMOD_LSHIFT) rangeMult = 0.5f;

		current_ += y /w_ * range_ * rangeMult;
		if (handler_) handler_->currentChanged(getId(), current_);

		skipRest = true;
	}
}

REGISTER_CLASS_SOURCE(GLWTankSlider);

GLWTankSlider::GLWTankSlider() : 
	GLWSlider(0.0f, 0.0f, 0.0f, 100.0f)
{
	setHandler(this);
	setToolTip(new GLWTip("Power",
		"Change the power of the current tank\n"
		"by clicking with the left mouse button\n"
		"and dragging up and down.\n"
		"Shift key decreases sensitivity."));
}

GLWTankSlider::~GLWTankSlider()
{
}

void GLWTankSlider::draw()
{
	Tank *currentTank =
		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (currentTank)
	{
		if (currentTank->getState().getState() == TankState::sNormal)
		{
			setCurrent(currentTank->getPosition().getPower());
		}
	}
	
	GLWSlider::draw();
}

void GLWTankSlider::currentChanged(unsigned int id, float value)
{
	Tank *currentTank =
		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (currentTank)
	{
		if (currentTank->getState().getState() == TankState::sNormal)
		{
			if (id == getId())
			{
				currentTank->getPosition().changePower(value, false);
			}
		}
	}
}

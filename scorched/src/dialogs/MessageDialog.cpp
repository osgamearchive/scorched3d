////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <dialogs/MessageDialog.h>
#include <client/ScorchedClient.h>
#include <common/OptionsGame.h>
#include <GLEXT/GLConsole.h>
#include <GLEXT/GLViewPort.h>
#include <GLW/GLWFont.h>

MessageDialog *MessageDialog::instance_ = 0;

MessageDialog *MessageDialog::instance()
{
	if (!instance_) instance_ = new MessageDialog();
	return instance_;
}

MessageDialog::MessageDialog() :
	showTime_(0.0f), GLWWindow("", 10.0f, 10.0f, 447.0f, 310.0f, 
		GLWWindow::eTransparent | GLWWindow::eNoTitle,
		"")
{
	disabled_ = true;
	windowLevel_ = 90000;
}

MessageDialog::~MessageDialog()
{
}

void MessageDialog::addMessage(const char *text)
{
	texts_.push_back(text);
}

void MessageDialog::simulate(float simTime)
{
	showTime_ -= simTime;
	if (showTime_ <= 0.0f)
	{
		if (!texts_.empty())
		{
			if (currentText_.size())
			{
				GLConsole::instance()->addLine(
					false, currentText_.c_str());
			}
			currentText_ = texts_.front();
			texts_.pop_front();
			showTime_ = 5.0f;
		}
	}
}

void MessageDialog::clear()
{
	texts_.clear(); 
	showTime_ = 0.0f;
}

void MessageDialog::draw()
{
	if (showTime_ <= 0.0f) return;

	GLState state(GLState::BLEND_ON | GLState::TEXTURE_OFF | GLState::DEPTH_OFF); 

	float wHeight = (float) GLViewPort::getHeight();
	float wWidth = (float) GLViewPort::getWidth();
	float textWidth = GLWFont::instance()->getLargePtFont()->getWidth(
		30, currentText_.c_str());

	float x = (wWidth/2.0f) - (textWidth / 2) - 10.0f;
	float y = wHeight - 60.0f;

	setW(textWidth + 20.0f);
	setH(40);
	setX(x);
	setY(y);
	GLWWindow::draw();

	Vector white(0.9f, 0.9f, 1.0f);
	GLWFont::instance()->getLargePtFont()->draw(
		white, 30, 
		x + 10.0f, y + 7.0f, 0.0f,
		currentText_.c_str());
}
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

#include <client/MessageDisplay.h>
#include <client/ScorchedClient.h>
#include <common/OptionsGame.h>
#include <GLW/GLWFont.h>

MessageDisplay *MessageDisplay::instance_ = 0;

MessageDisplay *MessageDisplay::instance()
{
	if (!instance_)
	{
		instance_ = new MessageDisplay;
	}
	return instance_;
}

MessageDisplay::MessageDisplay() : showTime_(0.0f)
{
}

MessageDisplay::~MessageDisplay()
{
}

void MessageDisplay::addMessage(const char *text)
{
	text_ = text;
	showTime_ = 5.0f;
}

void MessageDisplay::simulate(const unsigned state, float simTime)
{
	showTime_ -= simTime;
}

void MessageDisplay::draw(const unsigned currentstate)
{
	if (showTime_ <= 0.0f) return;

	GLState state(GLState::BLEND_ON | GLState::TEXTURE_OFF | GLState::DEPTH_OFF); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	static float fVPort[4];
	glGetFloatv(GL_VIEWPORT, fVPort);

	static Vector fontColor;
	fontColor = Vector(0.7f, 0.7f, 0.2f);

	GLWFont::instance()->getFont()->draw(
		fontColor, 20, 
		(fVPort[2]/2.0f) - (10.0f * text_.size()), (fVPort[3]/2.0f) - 10.0f, 0.0f, 
		text_.c_str());
}

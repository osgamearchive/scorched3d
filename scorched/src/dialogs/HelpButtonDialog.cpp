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

#include <scorched/MainDialog.h>
#include <dialogs/HelpButtonDialog.h>
#include <GLEXT/GLViewPort.h>
#include <GLEXT/GLBitmap.h>

HelpButtonDialog *HelpButtonDialog::instance_ = 0;

HelpButtonDialog *HelpButtonDialog::instance()
{
	if (!instance_)
	{
		instance_ = new HelpButtonDialog();
	}
	return instance_;
}

HelpButtonDialog::HelpButtonDialog() : 
	GLWWindow("", 0.0f, 10.0f, 32.0f, 32.0f, 0, "")
{
		windowLevel_ = 10000;
}

HelpButtonDialog::~HelpButtonDialog()
{
}

void HelpButtonDialog::draw()
{
	if (!helpTexture_.textureValid())
	{
		std::string file = getDataFile("data/windows/help.bmp");
		std::string filea = getDataFile("data/windows/helpa.bmp");
		GLBitmap map(file.c_str(), filea.c_str(), false);
		helpTexture_.create(map, GL_RGBA, false);
	}

	setY(float(GLViewPort::getHeight()) - h_);
	setX(float(GLViewPort::getWidth()) - h_);

	GLState state(GLState::TEXTURE_ON | GLState::BLEND_ON | GLState::DEPTH_OFF);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	helpTexture_.draw();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(x_, y_);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(x_ + 32.0f, y_);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(x_ + 32.0f, y_ + 32.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(x_, y_ + 32.0f);
	glEnd();
}

void HelpButtonDialog::mouseDown(float x, float y, bool &skipRest)
{
	if (inBox(x, y, x_, y_, w_, h_))
	{
		skipRest = true;
		runScorched3D("-starthelp");
	}
}

void HelpButtonDialog::mouseUp(float x, float y, bool &skipRest)
{
}

void HelpButtonDialog::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
}

void HelpButtonDialog::keyDown(char *buffer, unsigned int keyState,
	KeyboardHistory::HistoryElement *history, int hisCount, 
	bool &skipRest)
{
}

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
#include <scorched/HtmlHelpDialog.h>
#include <dialogs/HelpButtonDialog.h>
#include <sound/Sound.h>
#include <common/OptionsDisplay.h>
#include <common/Defines.h>
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
	GLWWindow("", 0.0f, 10.0f, 64.0f, 32.0f, 0, "")
{
	windowLevel_ = 200000;
}

HelpButtonDialog::~HelpButtonDialog()
{
}

GLTexture &HelpButtonDialog::getHelpTexture()
{
	if (!helpTexture_.textureValid())
	{
		std::string file = getDataFile("data/windows/help.bmp");
		std::string filea = getDataFile("data/windows/helpa.bmp");
		GLBitmap map(file.c_str(), filea.c_str(), false);
		helpTexture_.create(map, GL_RGBA, false);
	}
	return helpTexture_;
}

void HelpButtonDialog::draw()
{
	getHelpTexture();
	if (!soundTexture_.textureValid())
	{
		std::string file = getDataFile("data/windows/sound.bmp");
		std::string filea = getDataFile("data/windows/sounda.bmp");
		GLBitmap map(file.c_str(), filea.c_str(), false);
		soundTexture_.create(map, GL_RGBA, false);
	}

	setY(float(GLViewPort::getHeight()) - h_);
	setX(float(GLViewPort::getWidth()) - w_);

	GLState state(GLState::TEXTURE_ON | GLState::BLEND_ON | GLState::DEPTH_OFF);

	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	soundTexture_.draw();
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

	helpTexture_.draw();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(x_ + 32.0f, y_);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(x_ + 64.0f, y_);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(x_ + 64.0f, y_ + 32.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(x_ + 32.0f, y_ + 32.0f);
	glEnd();
}

void HelpButtonDialog::mouseDown(float x, float y, bool &skipRest)
{
	if (inBox(x, y, x_, y_, w_, h_))
	{
		skipRest = true;
		if (x > x_ + 32)
		{
			showHtmlHelpDialog();
			//runScorched3D("-starthelp");
		}
		else
		{
			std::list<GLWSelectorEntry> entries;

			if (OptionsDisplay::instance()->getNoSound())
			{
				int i = -1;
				entries.push_back(
						GLWSelectorEntry(
							"Sound Off", 
							0, true, 0, (void *) i));
			}
			else
			{
				for (int i=0; i<=10; i++)
				{
					int volume = int(float(i) * 12.8f);
					bool selected = 
						(OptionsDisplay::instance()->getSoundVolume() >= volume &&
						OptionsDisplay::instance()->getSoundVolume() < volume + 12);
					entries.push_back(
						GLWSelectorEntry(
							formatString("Volume : %i", i * 10), 
							0, selected, 0, (void *) i));
				}
			}

			GLWSelector::instance()->showSelector(
				this, x, y, entries);
		}
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

void HelpButtonDialog::itemSelected(GLWSelectorEntry *entry, int position)
{
	int data = (int) entry->getUserData();
	if (data != -1)
	{
		int volume = int(float(data) * 12.8f);
		Sound::instance()->getDefaultListener()->setGain(float(volume) / 128.0f);
		OptionsDisplay::instance()->setSoundVolume(volume);
	}
}

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

#include <GLEXT/GLMenu.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLViewPort.h>
#include <GLW/GLWFont.h>
#include <client/ScorchedClient.h>
#include <common/WindowManager.h>

GLMenu::GLMenu() : GLWWindow("", 0.0f, 10.0f, 10000.0f, 25.0f, 0, "")
{
	menuFont_ = GLWFont::instance()->getLargePtFont();
}

GLMenu::~GLMenu()
{

}

GLMenuEntry *GLMenu::getMenu(char *menuItem)
{
	GLMenuEntry *foundEntry = 0;

	std::string menu(menuItem);
	std::map<std::string, GLMenuEntry *>::iterator itor = menuList_.find(menu);
	if (itor != menuList_.end())
	{
		foundEntry = itor->second;
	}

	return foundEntry;
}

bool GLMenu::addMenu(char *menuName, float width, GLMenuI *selectFn, 
					 GLMenuI *textFn, GLMenuI *subMenuFn,
					 GLMenuI *enabledFn)
{
	if (getMenu(menuName)) return false;

	GLMenuEntry *entry = new GLMenuEntry(menuName, width, selectFn, 
		textFn, subMenuFn, enabledFn);
	menuList_[std::string(menuName)] = entry;

	return true;
}

bool GLMenu::addMenuItem(char *menuName, const GLMenuItem item)
{
	GLMenuEntry *entry = getMenu(menuName);
	if (!entry) return false;

	entry->addMenuItem((GLMenuItem &) item);

	return true;
}

void GLMenu::draw()
{
	GLState currentState(GLState::DEPTH_OFF | GLState::TEXTURE_OFF);

	bool depressed = false;
	std::map<std::string, GLMenuEntry *>::iterator itor;
	for (itor = menuList_.begin();
		itor != menuList_.end();
		itor++)
	{
		GLMenuEntry *entry = itor->second;
		if (entry->getEnabled() && entry->getDepressed())
		{
			depressed = true;
		}
	}	
	if (depressed) setH(1000000.0f);
	else setH(25.0f);

	float currentTop = (float) GLViewPort::getHeight();
	setY(currentTop - h_);

	int x = ScorchedClient::instance()->getGameState().getMouseX();
	int y = ScorchedClient::instance()->getGameState().getMouseY();
	if (WindowManager::instance()->getFocus(x, y) == getId())
	{
		GLfloat currentWidth = 0.0f;
		for (itor = menuList_.begin();
			itor != menuList_.end();
			itor++)
		{
			GLMenuEntry *entry = itor->second;
			if (entry->getEnabled())
			{
				entry->draw(*menuFont_, currentTop - 1.0f, currentWidth);
				currentWidth += entry->getWidth() + 1.0f;
			}
		}		
	}
}

void GLMenu::mouseDown(float x, float y, bool &hitMenu)
{
	hitMenu = false;
	float currentTop = (float) GLViewPort::getHeight();

	std::map<std::string, GLMenuEntry *>::iterator itor;
	for (itor = menuList_.begin();
		itor != menuList_.end();
		itor++)
	{
		GLMenuEntry *entry = itor->second;
		if (entry->getEnabled())
		{
			if (entry->click(currentTop, int(x), int(y)))
			{
				hitMenu = true;
			}
		}
	}
}

void GLMenu::mouseUp(float x, float y, bool &skipRest)
{

}

void GLMenu::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{

}

void GLMenu::keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest)
{

}

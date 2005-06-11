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

#include <GLW/GLWWindowSkinManager.h>
#include <GLW/GLWWindowManager.h>
#include <GLEXT/GLViewPort.h>
#include <XML/XMLFile.h>
#include <common/Defines.h>

GLWWindowSkinManager *GLWWindowSkinManager::instance_ = 0;

GLWWindowSkinManager *GLWWindowSkinManager::instance()
{
	if (!instance_)
	{
		instance_ = new GLWWindowSkinManager();
	}
	return instance_;
}

GLWWindowSkinManager::GLWWindowSkinManager()
{
}

GLWWindowSkinManager::~GLWWindowSkinManager()
{
}

bool GLWWindowSkinManager::loadWindows()
{
	XMLFile file;
	const char *fileName = getDataFile("data/windows.xml");
	if (!file.readFile(fileName) ||
		!file.getRootNode())
	{
		dialogMessage("GLWWindowSkinManager", 
					  "Failed to parse \"%s\"\n%s", 
					  fileName,
					  file.getParserError());
		return false;
	}

	// Itterate all of the windows in the file
    std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = file.getRootNode()->getChildren();
    for (childrenItor = children.begin();
        childrenItor != children.end();
        childrenItor++)
    {
		// For each node named window
        XMLNode *currentNode = (*childrenItor);

		GLWWindowSkin *window = new GLWWindowSkin;
		if (!window->initFromXML(currentNode))
		{
			return false;
		}
		windows_.push_back(window);
	}

	return true;
}

std::list<GLWWindowSkin *> GLWWindowSkinManager::getAllStateWindows()
{
	std::list<GLWWindowSkin *> windows;
	std::list<GLWWindowSkin *>::iterator itor;
	for (itor = windows_.begin();
		itor != windows_.end();
		itor++)
	{
		GLWWindowSkin *window = (GLWWindowSkin *) *itor;
		if (0 == strcmp(window->getStates(), "all"))
		{
			windows.push_back(window);
		}
	}
	return windows;
}

std::list<GLWWindowSkin *> GLWWindowSkinManager::getPlayerStateWindows()
{
	std::list<GLWWindowSkin *> windows;
	std::list<GLWWindowSkin *>::iterator itor;
	for (itor = windows_.begin();
		itor != windows_.end();
		itor++)
	{
		GLWWindowSkin *window = (GLWWindowSkin *) *itor;
		if (0 == strcmp(window->getStates(), "player"))
		{
			windows.push_back(window);
		}
	}
	return windows;
}

void GLWWindowSkinManager::loadPositions()
{
	XMLFile file;
	const char *fileName = getSettingsFile("windowpositions.xml");
	if (!file.readFile(fileName))
	{
		dialogMessage("GLWWindowSkinManager", 
					  "Failed to parse \"%s\"\n%s", 
					  fileName,
					  file.getParserError());
		return;
	}
	if (!file.getRootNode()) return;

	// Get root nodes
	XMLNode *display = 0, *positions = 0;
	if (!file.getRootNode()->getNamedChild("display", display)) return;
	if (!file.getRootNode()->getNamedChild("positions", positions)) return;

	// Get display size
	int width, height;
	if (!display->getNamedChild("width", width)) return;
	if (!display->getNamedChild("height", height)) return;

	// Check display size is the same as the current size
	// if not then discard the windows size changes
	if (width != GLViewPort::getWidth() ||
		height != GLViewPort::getHeight())
	{
		return;
	}

	// Itterate all of the positions in the file
    std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = positions->getChildren();
    for (childrenItor = children.begin();
        childrenItor != children.end();
        childrenItor++)
    {
		XMLNode *node = (*childrenItor);

		// Get the window position
		std::string window;
		int x, y;
		bool visible;
		if (!node->getNamedChild("window", window)) return;
		if (!node->getNamedChild("x", x)) return;
		if (!node->getNamedChild("y", y)) return;
		if (!node->getNamedChild("visible", visible)) return;

		// Set the new position
		std::list<GLWWindowSkin *> windows;
		std::list<GLWWindowSkin *>::iterator winitor;
		for (winitor = windows_.begin();
			winitor != windows_.end();
			winitor++)
		{
			GLWWindowSkin *w = *winitor;
			if (0 == strcmp(w->getName(), window.c_str()))
			{
				w->setX(float(x));
				w->setY(float(y));
				w->setVisible(visible);
			}
		}
	}
}

void GLWWindowSkinManager::savePositions()
{
	XMLNode node("positions");

	XMLNode *display = new XMLNode("display");
	XMLNode *positions = new XMLNode("positions");
	node.addChild(display);
	node.addChild(positions);

	// Add display
	display->addChild(new XMLNode("width", GLViewPort::getWidth()));
	display->addChild(new XMLNode("height", GLViewPort::getHeight()));

	// Add positions
	std::list<GLWWindowSkin *> windows;
	std::list<GLWWindowSkin *>::iterator winitor;
	for (winitor = windows_.begin();
		winitor != windows_.end();
		winitor++)
	{
		GLWWindowSkin *w = *winitor;

		if (w->getX() >= 0 && w->getY() >= 0)
		{
			bool visible = GLWWindowManager::instance()->windowVisible(w->getId());

			XMLNode *position = new XMLNode("position");
			positions->addChild(position);
			position->addChild(new XMLNode("window", w->getName()));
			position->addChild(new XMLNode("x", int(w->getX())));
			position->addChild(new XMLNode("y", int(w->getY())));
			position->addChild(new XMLNode("visible", visible));
		}
	}

	const char *fileName = getSettingsFile("windowpositions.xml");
	node.writeToFile(fileName);
}

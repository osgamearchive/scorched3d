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

#include <GLW/GLWWindowSkin.h>
#include <GLW/GLWVisibleWidget.h>
#include <GLW/GLWWindowManager.h>
#include <GLW/GLWToolTip.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLViewPort.h>
#include <GLEXT/GLBitmap.h>
#include <client/ScorchedClient.h>

GLWWindowSkin::GLWWindowSkin() : 
	GLWWindow("None", 0.0f, 0.0f,
		0.0f, 0.0f, eNoDraw, "")
{
	setToolTip(&toolTip_);
}

GLWWindowSkin::~GLWWindowSkin()
{
}

bool GLWWindowSkin::initFromXML(XMLNode *node)
{
	if (!GLWWindow::initFromXML(node)) return false;

	// States
	XMLNode *stateNode = node->removeNamedChild("states", true);
	if (!stateNode) return false; states_ = stateNode->getContent();

	// Key
	XMLNode *keyNode = node->removeNamedChild("key", true);
	if (!keyNode) return false; key_ = keyNode->getContent();

	// Visible
	XMLNode *visibleNode = node->removeNamedChild("visible", true);
	if (!visibleNode) return false; 
	visible_ = (strcmp("true", visibleNode->getContent()) == 0);

	return true;
}

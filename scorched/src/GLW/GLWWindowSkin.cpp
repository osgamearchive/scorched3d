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
		0.0f, 0.0f, eCircle, ""), init_(false)
{
	setToolTip(&toolTip_);
}

GLWWindowSkin::~GLWWindowSkin()
{
}

bool GLWWindowSkin::loadWindow(XMLNode *node)
{
	// Name
	XMLNode *nameNode = node->removeNamedChild("name", true);
	if (!nameNode) return false; name_ = nameNode->getContent();

	// Desc
	XMLNode *descNode = node->removeNamedChild("description", true);
	if (!descNode) return false; description_ = descNode->getContent();
	toolTip_.setText(name_.c_str(), description_.c_str());

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

	// Position
	if ((x_ = node->getNamedFloatChild("positionx", true)) == 
		XMLNode::ErrorFloat) return false;
	if ((y_ = node->getNamedFloatChild("positiony", true)) == 
		XMLNode::ErrorFloat) return false;

	// Width/Height
	if ((w_ = node->getNamedFloatChild("width", true)) == 
		XMLNode::ErrorFloat) return false;
	if ((h_ = node->getNamedFloatChild("height", true)) == 
		XMLNode::ErrorFloat) return false;

	// Items
	XMLNode *itemsNode = node->removeNamedChild("items", true);
	if (!itemsNode) return false;

	// Itterate all of the items in the file
    std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = itemsNode->getChildren();
    for (childrenItor = children.begin();
        childrenItor != children.end();
        childrenItor++)
    {
		// For each node named items
        XMLNode *currentNode = (*childrenItor);

		// The type
		XMLNode *typeNode = currentNode->getNamedParameter("type", true);
		if (!typeNode) return false;

		// Create new type
		GLWVisibleWidget *widget = (GLWVisibleWidget *)
			MetaClassRegistration::getNewClass(typeNode->getContent());
		if (!widget) 
		{
			dialogMessage("GLWWindowSkin",
				"Unknown type \"%s\"",
				typeNode->getContent());
			return false;
		}
		if (!widget->initFromXML(currentNode))
		{
			dialogMessage("GLWWindowSkin",
				"Failed to parse \"%s\" type",
				typeNode->getContent());
			return false;			
		}
		addWidget(widget);
	}

	return true;
}

void GLWWindowSkin::draw()
{
	if (!moveTexture_.textureValid())
	{
		GLBitmap moveMap(getDataFile("data/windows/move.bmp"), true);
		moveTexture_.create(moveMap, GL_RGBA, false);
	}

	if (!init_)
	{
		if (x_ < 0.0f) setX(GLViewPort::getWidth() + x_);
		if (y_ < 0.0f) setY(GLViewPort::getHeight() + y_);
		init_ = true;
	}

	GLState currentState(GLState::DEPTH_OFF | GLState::TEXTURE_OFF);
	glPushMatrix();
		GLWVisiblePanel::draw();
	glPopMatrix();
	
	int x = ScorchedClient::instance()->getGameState().getMouseX();
	int y = ScorchedClient::instance()->getGameState().getMouseY();
	if (GLWWindowManager::instance()->getFocus(x, y) == getId())
	{
		float sizeX = 20.0f;
		float sizeY = 20.0f;

		static GLWTip moveTip("Move",
			"Left click and drag to move the window.");
		GLWToolTip::instance()->addToolTip(&moveTip, 
			x_, y_ + h_ - sizeY, sizeX, sizeY);

		GLState currentStateBlend(GLState::BLEND_ON | GLState::TEXTURE_ON);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		moveTexture_.draw();
		glColor4f(0.8f, 0.0f, 0.0f, 0.8f);
		glPushMatrix();
			glTranslatef(x_, y_ + h_ - sizeY, 0.0f);
			glScalef(sizeX / 16.0f, sizeY / 16.0f, 1.0f);
			drawIconBox(0.0f, 0.0f);
		glPopMatrix();
	}
}

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

#include <GLEXT/GLState.h>
#include <GLW/GLWPanel.h>

GLWPanel::GLWPanelEntry::GLWPanelEntry(GLWidget *w, GLWCondition *c,
	unsigned int f, float wi) :
	widget(w), condition(c), flags(f),
	leftSpace(0.0f), rightSpace(0.0f),
	topSpace(0.0f), bottomSpace(0.0f)
{
	if (f & GLWPanel::SpaceRight) rightSpace = wi;
	if (f & GLWPanel::SpaceLeft) leftSpace = wi;
	if (f & GLWPanel::SpaceTop) topSpace = wi;
	if (f & GLWPanel::SpaceBottom) bottomSpace = wi;
}

REGISTER_CLASS_SOURCE(GLWPanel);

GLWPanel::GLWPanel() : GLWidget()
{

}

GLWPanel::~GLWPanel()
{
	clear();
}

GLWidget *GLWPanel::addWidget(GLWidget *widget, GLWCondition *condition, 
	unsigned int flags, float width)
{
	GLWPanelEntry entry(widget, condition, flags, width);
	widgets_.push_back(entry);
	widget->setParent(this);
	return widget;
}

void GLWPanel::clear()
{
	std::list<GLWPanelEntry>::iterator itor;
	for (itor = widgets_.begin();
		itor != widgets_.end();
		itor++)
	{
		delete (*itor).widget;
		delete (*itor).condition;
	}
	widgets_.clear();
}

void GLWPanel::simulate(float frameTime)
{
	std::list<GLWPanelEntry>::iterator itor;
	for (itor = widgets_.begin();
		itor != widgets_.end();
		itor++)
	{
		GLWPanelEntry &entry = *itor;
		if (!entry.condition || 
			entry.condition->getResult(entry.widget))
		{
			entry.widget->simulate(frameTime);
		}
	}
}

void GLWPanel::draw()
{
	GLState currentState(GLState::DEPTH_OFF | GLState::TEXTURE_OFF);

	std::list<GLWPanelEntry>::iterator itor;
	for (itor = widgets_.begin();
		itor != widgets_.end();
		itor++)
	{
		GLWPanelEntry &entry = *itor;
		if (!entry.condition || 
			entry.condition->getResult(entry.widget))
		{
			glPushMatrix();
				(*itor).widget->draw();
			glPopMatrix();
		}
	}
}

void GLWPanel::mouseDown(float x, float y, bool &skipRest)
{
	std::list<GLWPanelEntry>::reverse_iterator itor;
	for (itor = widgets_.rbegin();
		itor != widgets_.rend();
		itor++)
	{
		GLWPanelEntry &entry = *itor;
		if (!entry.condition || 
			entry.condition->getResult(entry.widget))
		{
			(*itor).widget->mouseDown(x, y, skipRest);
			if (skipRest) break;
		}
	}
}

void GLWPanel::mouseUp(float x, float y, bool &skipRest)
{
	std::list<GLWPanelEntry>::reverse_iterator itor;
	for (itor = widgets_.rbegin();
		itor != widgets_.rend();
		itor++)
	{
		GLWPanelEntry &entry = *itor;
		if (!entry.condition || 
			entry.condition->getResult(entry.widget))
		(*itor).widget->mouseUp(x, y, skipRest);
		if (skipRest) break;
	}
}

void GLWPanel::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	std::list<GLWPanelEntry>::reverse_iterator itor;
	for (itor = widgets_.rbegin();
		itor != widgets_.rend();
		itor++)
	{
		GLWPanelEntry &entry = *itor;
		if (!entry.condition || 
			entry.condition->getResult(entry.widget))
		{
			(*itor).widget->mouseDrag(mx, my, x, y, skipRest);
			if (skipRest) break;
		}
	}
}

void GLWPanel::keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest)
{
	std::list<GLWPanelEntry>::reverse_iterator itor;
	for (itor = widgets_.rbegin();
		itor != widgets_.rend();
		itor++)
	{
		GLWPanelEntry &entry = *itor;
		if (!entry.condition || 
			entry.condition->getResult(entry.widget))
		{
			(*itor).widget->keyDown(buffer, keyState, history, hisCount, skipRest);
			if (skipRest) break;
		}
	}
}

bool GLWPanel::initFromXML(XMLNode *node)
{
	// Items
	XMLNode *itemsNode = node->getNamedChild("items", true, true);
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

		// The Widget
		XMLNode *widgetNode = currentNode->getNamedChild("widget", true, true);
		if (!widgetNode) return false;

		// The type
		XMLNode *widgetTypeNode = widgetNode->getNamedParameter("type", true);
		if (!widgetTypeNode) return false;

		// Create new type
		GLWidget *widget = (GLWidget *)
			MetaClassRegistration::getNewClass(widgetTypeNode->getContent());
		if (!widget) 
		{
			dialogMessage("GLWPanel",
				"Unknown widget type \"%s\"",
				widgetTypeNode->getContent());
			return false;
		}
		if (!widget->initFromXML(widgetNode))
		{
			dialogMessage("GLWPanel",
				"Failed to parse \"%s\" widget type",
				widgetTypeNode->getContent());
			return false;			
		}

		// The condition (if any)
		GLWCondition *condition = 0;
		XMLNode *conditionNode = currentNode->getNamedChild("condition", false, true);
		if (conditionNode)
		{
			// Get the type of this condition
			XMLNode *conditionTypeNode = conditionNode->
				getNamedParameter("type", true);
			if (!conditionTypeNode) return false;

			// Create type
			condition = (GLWCondition *)
				MetaClassRegistration::getNewClass(conditionTypeNode->getContent());
			if (!condition)
			{
				dialogMessage("GLWPanel",
					"Unknown condition type \"%s\"",
					conditionTypeNode->getContent());
				return false;
			}
			if (!condition->initFromXML(conditionNode))
			{
				dialogMessage("GLWPanel",
					"Failed to parse \"%s\" condition type",
					conditionTypeNode->getContent());
				return false;
			}
		}

		addWidget(widget, condition);
	}
	return true;
}


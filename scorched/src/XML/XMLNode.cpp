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

#include <XML/XMLNode.h>
#include <common/Defines.h>
#include <limits.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>

void XMLNode::removeSpecialChars(std::string &content, std::string &result)
{
	result = "";
	for (char *c=(char *) content.c_str(); *c; c++)
	{
		char newchar = *c;
		if (*c == '\n') result += "&#10;";
		else if (*c < 32 || *c > 126) result += " ";
		else if (*c == '>') result += "&gt;";
		else if (*c == '<') result += "&lt;";
		else if (*c == '\'') result += "&apos;";
		else if (*c == '"') result += "&quot;";
		else if (*c == '&') result += "&amp;";
		else if (*c == '%') result += "&#37;";
		else result += *c;
	}
}

void XMLNode::addSpecialChars(std::string &content, std::string &result)
{
	result = "";
	for (char *c=(char *) content.c_str(); *c; c++)
	{
		if (*c == '&')
		{
			if (strstr(c, "&gt;") == c) { result += '>'; c+=3; }
			else if (strstr(c, "&lt;") == c) { result += '<'; c+=3; }
			else if (strstr(c, "&apos;") == c) { result += '\''; c+=5; }
			else if (strstr(c, "&quot;") == c) { result += '"'; c+=5; }
			else if (strstr(c, "&amp;") == c) { result += '&'; c+=4; }
			else if (strstr(c, "&#") == c)
			{
				char *pos = strchr(c, ';');
				if (pos <= c + 3)
				{
					c+=2;
					result += formatString("%c", atoi(c));
					*pos = '\0';
					c = pos + 1;
				}
			}
			else result += *c;
		}
		else result += *c;
	}	
}

const char *XMLNode::getSpacer(int space)
{
	static std::string spacestr;
	spacestr = "";

	for (int i=0; i<space; i++) spacestr+="\t";
	return spacestr.c_str();
}

XMLNode::XMLNode(const char *name, const char *content, NodeType type) : 
	name_(name), parent_(0), type_(type),
	content_(content)
{
}

XMLNode::XMLNode(const char *name, float content, NodeType type) :
	name_(name), parent_(0), type_(type)
{
	char buffer[20];
	snprintf(buffer, 20, "%.2f", content);
	content_ = buffer;
}

XMLNode::XMLNode(const char *name, int content, NodeType type) :
	name_(name), parent_(0), type_(type)
{
	char buffer[20];
	snprintf(buffer, 20, "%i", content);
	content_ = buffer;
}

XMLNode::XMLNode(const char *name, unsigned int content, NodeType type) :
	name_(name), parent_(0), type_(type)
{
	char buffer[20];
	snprintf(buffer, 20, "%i", content);
	content_ = buffer;
}

XMLNode::XMLNode(const char *name, bool content, NodeType type) :
	name_(name), parent_(0), type_(type)
{
	content_ = (content?"true":"false");
}

XMLNode::XMLNode(const char *name, Vector &content, NodeType type) :
	name_(name), parent_(0), type_(type)
{
	addChild(new XMLNode("A", content[0]));
	addChild(new XMLNode("B", content[1]));
	addChild(new XMLNode("C", content[2]));
}

XMLNode::~XMLNode()
{
	while (!children_.empty())
	{
		XMLNode *node = children_.front();
		children_.pop_front();
		delete node;
	}
	while (!removedChildren_.empty())
	{
		XMLNode *node = removedChildren_.front();
		removedChildren_.pop_front();
		delete node;
	}
	while (!parameters_.empty())
	{
		XMLNode *node = parameters_.front();
		parameters_.pop_front();
		delete node;
	}
	while (!removedParameters_.empty())
	{
		XMLNode *node = removedParameters_.front();
		removedParameters_.pop_front();
		delete node;
	}
}

bool XMLNode::writeToFile(const char *filename)
{
	FileLines lines;
	addNodeToFile(lines, 0);
	return lines.writeFile((char *) filename);
}

void XMLNode::addNodeToFile(FileLines &lines, int spacing)
{
	if (type_ == XMLNodeType)
	{
		std::string params;
		std::list<XMLNode *>::iterator pitor;
		for (pitor = parameters_.begin();
			pitor != parameters_.end();
			pitor++)
		{
			XMLNode *node = (*pitor);
			DIALOG_ASSERT(node->type_ == XMLParameterType);
			
			std::string newContent;
			removeSpecialChars(node->content_, newContent);
			
			params += " " + node->name_ + "='" + newContent + "'";
		}

		if (children_.empty())
		{
			std::string newContent;
			removeSpecialChars(content_, newContent);
			
			lines.addLine(formatString("%s<%s%s>%s</%s>", 
				getSpacer(spacing),
				name_.c_str(), params.c_str(), 
				newContent.c_str(), name_.c_str()));
		}
		else
		{
			lines.addLine(formatString("%s<%s%s>", 
				getSpacer(spacing),
				name_.c_str(), params.c_str()));

			std::list<XMLNode *>::iterator itor;
			for (itor = children_.begin();
				itor != children_.end();
				itor++)
			{
				XMLNode *node = (*itor);
				node->addNodeToFile(lines, spacing + 1);
			}

			lines.addLine(formatString("%s</%s>", 
				getSpacer(spacing), name_.c_str()));
		}
	}
	else if (type_ == XMLCommentType)
	{
		lines.addLine(formatString("%s<!-- %s -->", 
			getSpacer(spacing), content_.c_str()));
	}
}

bool XMLNode::failChildren()
{
	if (!children_.empty())
	{
		XMLNode *node = children_.front();
		node->returnError(formatString("Unrecognised node."));
		return false;
	}
	for (const char *c=content_.c_str(); *c; c++)
	{
		if (*c != '\n' &&
			*c != '\r' &&
			*c != '\t' &&
			*c != ' ')
		{
			returnError(formatString("Unexpected context : %s", 
					content_.c_str()));
			return false;
		}
	}

	return true;
}

void XMLNode::resurrectRemovedChildren()
{
	while (!removedChildren_.empty())
	{
		XMLNode *node = removedChildren_.front();
		removedChildren_.pop_front();
		children_.push_back(node);
		
		node->resurrectRemovedChildren();
	}
	while (!removedParameters_.empty())
	{
		XMLNode *node = removedParameters_.front();
		removedParameters_.pop_front();
		parameters_.push_back(node);
	}
}

bool XMLNode::getNamedChild(const char *name, XMLNode *&value,
	bool failOnError, bool remove)
{
	std::list<XMLNode *>::iterator itor;
	for (itor = children_.begin();
		itor != children_.end();
		itor++)
	{
		XMLNode *node = (*itor);
		if (strcmp(name, node->getName()) == 0) 
		{
			if (remove)
			{
				removedChildren_.push_back(node);
				children_.erase(itor);
			}
			value = node;
			return true;
		}
	}

	if (failOnError)
	{
		returnError(formatString("Failed to find \"%s\" node", name));
	}
	return false;
}

bool XMLNode::getNamedParameter(const char *name, XMLNode *&value,
	bool failOnError, bool remove)
{
	std::list<XMLNode *>::iterator itor;
	for (itor = parameters_.begin();
		itor != parameters_.end();
		itor++)
	{
		XMLNode *node = (*itor);
		if (strcmp(name, node->getName()) == 0)
		{
			if (remove)
			{
				removedParameters_.push_back(node);
				parameters_.erase(itor);
			}
			value = node;
			return true;
		}
	}

	if (failOnError)
	{
		returnError(formatString("Failed to find \"%s\" parameter", name));
	}
	return false;
}

bool XMLNode::getNamedParameter(const char *name, std::string &value,
	bool failOnError, bool remove)
{
	XMLNode *node;
	if (!getNamedParameter(name, node, failOnError, remove)) return false;
	value = node->getContent();
	return true;
}

bool XMLNode::getNamedChild(const char *name, std::string &value,
	bool failOnError, bool remove)
{
	XMLNode *node;
	if (!getNamedChild(name, node, failOnError, remove)) return false;
	value = node->getContent();
	return true;
}

bool XMLNode::getNamedChild(const char *name, bool &value,
	bool failOnError, bool remove)
{
	XMLNode *node;
	if (!getNamedChild(name, node, failOnError, remove)) return false;

	if (0 == strcmp(node->getContent(), "true")) value = true;
	else if (0 == strcmp(node->getContent(), "false")) value = false;
	else 
	{
		return node->returnError(
			"Failed to parse boolean value (should be true or false)");
	}
	return true;
}

bool XMLNode::getNamedChild(const char *name, float &value,
	bool failOnError, bool remove)
{
	XMLNode *node;
	if (!getNamedChild(name, node, failOnError, remove)) return false;
	if (sscanf(node->getContent(), "%f", &value) != 1) 
		return node->returnError("Failed to parse float value");
	return true;
}

bool XMLNode::getNamedChild(const char *name, int &value,
	bool failOnError, bool remove)
{
	XMLNode *node;
	if (!getNamedChild(name, node, failOnError, remove)) return false;
	if (sscanf(node->getContent(), "%i", &value) != 1)
		return node->returnError("Failed to parse int value");
	return true;
}

bool XMLNode::getNamedChild(const char *name, unsigned int &value,
	bool failOnError, bool remove)
{
	XMLNode *node;
	if (!getNamedChild(name, node, failOnError, remove)) return false;
	if (sscanf(node->getContent(), "%u", &value) != 1)
		return node->returnError("Failed to parse unsigned int value");
	return true;
}

bool XMLNode::getNamedChild(const char *name, Vector &value, 
	bool failOnError, bool remove)
{
	XMLNode *node, *nodeA, *nodeB, *nodeC;
	if (!getNamedChild(name, node, failOnError, remove)) return false;
	if (!node->getNamedChild("A", nodeA, failOnError, true)) return false;
	if (!node->getNamedChild("B", nodeB, failOnError, true)) return false;
	if (!node->getNamedChild("C", nodeC, failOnError, true)) return false;
	value[0] = (float) atof(nodeA->getContent());
	value[1] = (float) atof(nodeB->getContent());
	value[2] = (float) atof(nodeC->getContent());
	return true;
}

void XMLNode::setSource(const char *source)
{ 
	source_ = source; 
}

void XMLNode::setLine(int line, int col)
{
	line_ = line;
	col_ = col;
}

void XMLNode::addChild(XMLNode *node) 
{ 
	children_.push_back(node); 
	node->parent_ = this; 
	node->source_ = source_; 
}

void XMLNode::addParameter(XMLNode *node) 
{ 
	parameters_.push_back(node); 
	node->parent_ = this; 
	node->source_ = source_; 
}

void XMLNode::addContent(const char *data, int len)
{ 
	content_.append(data, len); 
}

void XMLNode::convertContent()
{
	std::string oldContent = content_;
	addSpecialChars(oldContent, content_);
}

bool XMLNode::returnError(const char *error)
{
	dialogMessage("XMLNode",
		formatString("Parse Error, File:%s Line:%i Col:%i Node:%s Error:%s",
		source_.c_str(), line_, col_, getName(), error));
	return false;
}

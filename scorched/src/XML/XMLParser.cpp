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

#include <XML/XMLParser.h>
#include <common/Defines.h>
#include <limits.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>

static void removeSpecialChars(std::string &content, std::string &result)
{
	result = "";
	for (char *c=(char *) content.c_str(); *c; c++)
	{
		char newchar = *c;
		if (*c < 32 || *c > 126) result += ' ';
		else if (*c == '>') result += "&gt;";
		else if (*c == '<') result += "&lt;";
		else if (*c == '\'') result += "&apos;";
		else if (*c == '"') result += "&quot;";
		else if (*c == '&') result += "&amp;";
		else if (*c == '%') result += "&#37;";
		else result += *c;
	}
}

static void addSpecialChars(std::string &content, std::string &result)
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
			else if (strstr(c, "&#37;") == c) { result += '%'; c+=4; }
			else result += *c;
		}
		else result += *c;
	}	
}

static const char *getSpacer(int space)
{
	static std::string spacestr;
	spacestr = "";

	for (int i=0; i<space; i++) spacestr+="\t";
	return spacestr.c_str();
}

static bool testMacro()
{
	XMLNode *node;
	float testFloat;
	Vector testVector;
	int testInt;
	unsigned int testUInt;
	const char *testString;

	XML_PARSE_FLOAT(node, testFloat, "test");
	XML_PARSE_VECTOR(node, testVector, "test");
	XML_PARSE_INT(node, testInt, "test");
	XML_PARSE_UINT(node, testUInt, "test");
	XML_PARSE_STRING(node, testString, "test");
	return true;
}

float XMLNode::ErrorFloat = FLT_MAX;
int XMLNode::ErrorInt = INT_MAX;
unsigned int XMLNode::ErrorUInt = UINT_MAX;
Vector XMLNode::ErrorVector = Vector(FLT_MAX, FLT_MAX, FLT_MAX);
const char *XMLNode::ErrorString = "Error"; 

XMLNode::XMLNode(const char *name, const char *content, NodeType type) : 
	name_(name), parent_(0), type_(type),
	content_(content)
{
}

XMLNode::XMLNode(const char *name, float content, NodeType type) :
	name_(name), parent_(0), type_(type)
{
	char buffer[20];
	sprintf(buffer, "%.2f", content);
	content_ = buffer;
}

XMLNode::XMLNode(const char *name, int content, NodeType type) :
	name_(name), parent_(0), type_(type)
{
	char buffer[20];
	sprintf(buffer, "%i", content);
	content_ = buffer;
}

XMLNode::XMLNode(const char *name, unsigned int content, NodeType type) :
	name_(name), parent_(0), type_(type)
{
	char buffer[20];
	sprintf(buffer, "%i", content);
	content_ = buffer;
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
	while (!removedNodes_.empty())
	{
		XMLNode *node = removedNodes_.front();
		removedNodes_.pop_front();
		delete node;
	}
	while (!parameters_.empty())
	{
		XMLNode *node = parameters_.front();
		parameters_.pop_front();
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
			
			lines.addLine("%s<%s%s>%s</%s>", 
				getSpacer(spacing),
				name_.c_str(), params.c_str(), 
				newContent.c_str(), name_.c_str());
		}
		else
		{
			lines.addLine("%s<%s%s>", 
				getSpacer(spacing),
				name_.c_str(), params.c_str());

			std::list<XMLNode *>::iterator itor;
			for (itor = children_.begin();
				itor != children_.end();
				itor++)
			{
				XMLNode *node = (*itor);
				node->addNodeToFile(lines, spacing + 1);
			}

			lines.addLine("%s</%s>", 
				getSpacer(spacing), name_.c_str());
		}
	}
	else if (type_ == XMLCommentType)
	{
		lines.addLine("%s<!-- %s -->", 
			getSpacer(spacing), content_.c_str());
	}
}

XMLNode *XMLNode::getNamedChild(const char *name, bool failOnError, bool remove)
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
				removedNodes_.push_back(node);
				children_.erase(itor);
			}
			return node;
		}
	}

	if (failOnError)
	{
		dialogMessage("XMLNode",
			"Error: Failed to find \"%s\" node in \"%s:%s\"\n",
			name, source_.c_str(),
			(parent_->getName()?parent_->getName():"Root"));
	}
	return 0;
}

XMLNode *XMLNode::getNamedParameter(const char *name, bool failOnError, bool remove)
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
				removedNodes_.push_back(node);
				parameters_.erase(itor);
			}
			return node;
		}
	}

	if (failOnError)
	{
		dialogMessage("XMLNode",
			"Error: Failed to find \"%s\" parameter in \"%s:%s\"\n",
			name, source_.c_str(), 
			(parent_->getName()?parent_->getName():"Root"));
	}
	return 0;
}

const char *XMLNode::getNamedStringChild(const char *name, bool failOnError, bool remove)
{
	XMLNode *node = getNamedChild(name, failOnError, remove);
	if (!node) return ErrorString;
	return node->getContent();
}

float XMLNode::getNamedFloatChild(const char *name, bool failOnError, bool remove)
{
	XMLNode *node = getNamedChild(name, failOnError, remove);
	if (!node) return ErrorFloat;
	return (float) atof(node->getContent());
}

int XMLNode::getNamedIntChild(const char *name, bool failOnError, bool remove)
{
	XMLNode *node = getNamedChild(name, failOnError, remove);
	if (!node) return ErrorInt;
	return (int) atoi(node->getContent());
}

unsigned int XMLNode::getNamedUIntChild(const char *name, bool failOnError, bool remove)
{
	XMLNode *node = getNamedChild(name, failOnError, remove);
	if (!node) return ErrorUInt;
	return (unsigned int) atoi(node->getContent());
}

Vector XMLNode::getNamedVectorChild(const char *name, bool failOnError, bool remove)
{
	XMLNode *node = getNamedChild(name, failOnError, remove);
	if (!node) return ErrorVector;
	XMLNode *nodeA = node->getNamedChild("A", failOnError, true);
	XMLNode *nodeB = node->getNamedChild("B", failOnError, true);
	XMLNode *nodeC = node->getNamedChild("C", failOnError, true);
	if (!nodeA || !nodeB || !nodeC) return ErrorVector;
	Vector result;
	result[0] = (float) atof(nodeA->getContent());
	result[1] = (float) atof(nodeB->getContent());
	result[2] = (float) atof(nodeC->getContent());
	return result;
}

void XMLNode::addSource(const char *source)
{ 
	source_ = source; 
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

XMLParser::XMLParser() : root_(0), current_(0), 
	source_("Not Specified")
{
	// Init the XML parser
	p_ = XML_ParserCreate(0);
	XML_SetStartElementHandler(p_, startElementStaticHandler);
	XML_SetEndElementHandler(p_, endElementStaticHandler);
	XML_SetCharacterDataHandler(p_, characterDataStaticHandler);
	XML_SetUserData(p_, this);
}

XMLParser::~XMLParser()
{
	// Cleanup
	XML_ParserFree(p_);
	delete root_;
}

bool XMLParser::parse(const char *data, int len, int final)
{
	// Parser the XML
	XML_Status status;
	status = XML_Parse(p_, data, len, final);
	return (status == XML_STATUS_OK);
}

const char *XMLParser::getParseError()
{
	XML_Error errorCode = XML_GetErrorCode(p_);

	static char message[1024];
	sprintf(message, "Parse Error, Line:%i Col:%i Error:%s",
		XML_GetCurrentLineNumber(p_),
		XML_GetCurrentColumnNumber(p_),
		XML_ErrorString(errorCode));
	return message;
}

void XMLParser::startElementHandler(const XML_Char *name,
                           const XML_Char **atts)
{
	if (!root_)
	{
		// Create the root node
		root_ = current_ = new XMLNode(name);
		root_->addSource(source_.c_str());
	}
	else
	{
		DIALOG_ASSERT(current_);

		// Add a new child to this node
		XMLNode *newNode = new XMLNode(name);
		current_->addChild(newNode);
		current_ = newNode;
	}

	if (atts)
	{
		while (*atts)
		{
			const XML_Char *name = *atts;
			atts++;
			const XML_Char *value = *atts;
			atts++;

			XMLNode *param = new XMLNode(name, "", XMLNode::XMLParameterType);
			param->addContent(value, strlen(value));
			param->convertContent();
			current_->addParameter(param);
		}
	}
}

void XMLParser::endElementHandler(const XML_Char *name)
{
	DIALOG_ASSERT(current_);
	DIALOG_ASSERT(strcmp(name, current_->getName()) == 0);

	current_->convertContent();
	current_ = (XMLNode *) current_->getParent();
}

void XMLParser::characterDataHandler(const XML_Char *s,
                            int len)
{
	current_->addContent(s, len);
}

void XMLParser::startElementStaticHandler(void *userData,
                           const XML_Char *name,
                           const XML_Char **atts)
{
	XMLParser *parser = (XMLParser *) userData;
	parser->startElementHandler(name, atts);
}

void XMLParser::endElementStaticHandler(void *userData,
                         const XML_Char *name)
{
	XMLParser *parser = (XMLParser *) userData;
	parser->endElementHandler(name);
}

void XMLParser::characterDataStaticHandler(void *userData,
                            const XML_Char *s,
                            int len)
{
	XMLParser *parser = (XMLParser *) userData;
	parser->characterDataHandler(s, len);
}

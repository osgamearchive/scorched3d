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

#if !defined(__INCLUDE_XMLNodeh_INCLUDE__)
#define __INCLUDE_XMLNodeh_INCLUDE__

#include <string>
#include <list>
#include <XML/expat.h>
#include <common/FileLines.h>
#include <common/Vector.h>

class XMLNode
{
public:
	enum NodeType
	{
		XMLNodeType,
		XMLParameterType,
		XMLCommentType
	};

	XMLNode(const char *name, const char *content = "", 
		NodeType = XMLNode::XMLNodeType);
	XMLNode(const char *name, float content, 
		NodeType = XMLNode::XMLNodeType);
	XMLNode(const char *name, int content, 
		NodeType = XMLNode::XMLNodeType);
	XMLNode(const char *name, Vector &content, 
		NodeType = XMLNode::XMLNodeType);
	XMLNode(const char *name, unsigned int content, 
		NodeType = XMLNode::XMLNodeType);
	virtual ~XMLNode();

	bool writeToFile(const char *fileName);

	NodeType getType() { return type_; }
	const char *getName() { return name_.c_str(); }
	const char *getContent() { return content_.c_str(); }
	const char *getSource() { return source_.c_str(); }
	const XMLNode *getParent() { return parent_; }
	std::list<XMLNode *> &getChildren() { return children_; }
	std::list<XMLNode *> &getParameters() { return parameters_; }

	bool getNamedParameter(const char *name, XMLNode *&node, 
		bool failOnError = true, bool remove = true);
	bool getNamedChild(const char *name, XMLNode *&node, 
		bool failOnError = true, bool remove = true);
	bool getNamedString(const char *name, std::string &value,
		bool failOnError = true, bool remove = true);
	bool getNamedBool(const char *name, bool &value,
		bool failOnError = true, bool remove = true);
	bool getNamedFloat(const char *name, float &value,
		bool failOnError = true, bool remove = true);
	bool getNamedInt(const char *name, int &value,
		bool failOnError = true, bool remove = true);
	bool getNamedUInt(const char *name, unsigned int &value,
		bool failOnError = true, bool remove = true);
	bool getNamedVector(const char *name, Vector &value,
		bool failOnError = true, bool remove = true);

	bool failChildren();

	void addSource(const char *source);
	void addChild(XMLNode *node); 
	void addParameter(XMLNode *node);
	void addContent(const char *data, int len);
	void convertContent();

protected:
	NodeType type_;
	XMLNode *parent_;
	std::list<XMLNode *> children_;
	std::list<XMLNode *> removedNodes_; // So they are tidied up as well
	std::list<XMLNode *> parameters_;
	std::string name_;
	std::string content_;
	std::string source_;

	void addNodeToFile(FileLines &lines, int spacing);

};

class XMLParser
{
public:
	XMLParser();
	virtual ~XMLParser();

	bool parse(const char *data, int len, int final);
	const char *getParseError();

	void setSource(const char *source) { source_ = source; }
	XMLNode *getRoot() { return root_; }

protected:
	XMLNode *root_;
	XMLNode *current_;
	XML_Parser p_;
	std::string source_;

	void startElementHandler(const XML_Char *name,
                           const XML_Char **atts);
	void endElementHandler(const XML_Char *name);
	void characterDataHandler(const XML_Char *s,
                            int len);

	static void startElementStaticHandler(void *userData,
                           const XML_Char *name,
                           const XML_Char **atts);
	static void endElementStaticHandler(void *userData,
                         const XML_Char *name);
	static void characterDataStaticHandler(void *userData,
                            const XML_Char *s,
                            int len);
};

#endif

#if !defined(__INCLUDE_XMLNodeh_INCLUDE__)
#define __INCLUDE_XMLNodeh_INCLUDE__

#include <string>
#include <list>
#include <XML/expat.h>

class XMLNode
{
public:
	enum NodeType
	{
		XMLNodeType,
		XMLParameterType
	};

	XMLNode(const char *name, XMLNode *parent, 
		NodeType type = XMLNode::XMLNodeType);
	virtual ~XMLNode();

	const char *getName() { return name_.c_str(); }
	const char *getContent() { return content_.c_str(); }
	const XMLNode *getParent() { return parent_; }
	std::list<XMLNode *> &getChildren() { return children_; }
	std::list<XMLNode *> &getParameters() { return parameters_; }

	NodeType getType() { return type_; }
	XMLNode *getNamedParameter(const char *name);
	XMLNode *getNamedChild(const char *name);

	void addChild(XMLNode *node) { children_.push_back(node); }
	void addContent(const char *data, int len) { content_.append(data, len); }

protected:
	NodeType type_;
	XMLNode *parent_;
	std::list<XMLNode *> children_;
	std::list<XMLNode *> parameters_;
	std::string name_;
	std::string content_;

};

class XMLParser
{
public:
	XMLParser();
	virtual ~XMLParser();

	bool parse(const char *data, int len, int final);
	const char *getParseError();

	XMLNode *getRoot() { return root_; }

protected:
	XMLNode *root_;
	XMLNode *current_;
	XML_Parser p_;

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

#if !defined(__INCLUDE_XMLFileh_INCLUDE__)
#define __INCLUDE_XMLFileh_INCLUDE__

#include <XML/XMLParser.h>

class XMLFile
{
public:
	XMLFile();
	virtual ~XMLFile();

	bool readFile(const char *fileName);

	const char *getParserError() { return parser_.getParseError(); }
	XMLNode *getRootNode() { return parser_.getRoot(); }

protected:
	XMLParser parser_;

};

#endif

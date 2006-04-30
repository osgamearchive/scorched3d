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

#if !defined(__INCLUDE_XMLParserh_INCLUDE__)
#define __INCLUDE_XMLParserh_INCLUDE__

#include <XML/expat.h>
#include <XML/XMLNode.h>

class XMLParser
{
public:
	XMLParser(bool useContentNodes = false);
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
	bool useContentNodes_;

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

#endif // __INCLUDE_XMLParserh_INCLUDE__

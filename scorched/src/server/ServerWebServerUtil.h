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

#if !defined(__INCLUDE_ServerWebServerUtilh_INCLUDE__)
#define __INCLUDE_ServerWebServerUtilh_INCLUDE__

#include <string>
#include <list>
#include <map>

class NetMessage;
class OptionEntry;
namespace ServerWebServerUtil
{
	// Get query fields
	void extractMultiPartPost(const char *start, 
		const char *boundry, int sizeleft, std::map<std::string, NetMessage *> &parts);
	void extractQueryFields(std::map<std::string, std::string> &fields, char *sep);

	// General fns
	const char *strstrlen(const char *start, const char *find, int size);
	void generateSettingValue(OptionEntry *entry, std::string &value);
	const char *getField(std::map<std::string, std::string> &fields, const char *field);
	std::string getFile(const char *filename);
	std::string concatLines(std::list<std::string> &lines);

	// HTML template functions
	bool getTemplate(
		const char *name,
		std::map<std::string, std::string> &fields,
		std::string &result);
	bool getHtmlTemplate(
		const char *name,
		std::map<std::string, std::string> &fields,
		std::string &result);
	void getHtmlRedirect(
		const char *url,
		std::string &result);
	void getHtmlNotFound(
		std::string &result);
	bool getHtmlMessage(
		const char *title,
		const char *text,
		std::map<std::string, std::string> &fields,
		std::string &result);
};

#endif

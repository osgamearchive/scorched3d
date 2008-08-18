////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#if !defined(__INCLUDE_ResourceBundleEntry_INCLUDE__)
#define __INCLUDE_ResourceBundleEntry_INCLUDE__

#include <lang/LangString.h>
#include <vector>

class ResourceBundleEntry
{
public:
	ResourceBundleEntry(const std::string &key);
	ResourceBundleEntry(const std::string &key, 
		const std::string &value);

	const char *getKey() { return key_.c_str(); }
	const char *getValue() { return value_.c_str(); }

	LangString getString();
	LangString getString(std::string param1);
	LangString getString(std::string param1, std::string param2);
	LangString getString(std::string param1, std::string param2, std::string param3);

private:
	std::vector<LangString> parts_;
	std::vector<int> positions_;
	std::string key_, value_;
};

#endif // __INCLUDE_ResourceBundleEntry_INCLUDE__
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

#if !defined(__INCLUDE_LandscapeDefinitionsItemh_INCLUDE__)
#define __INCLUDE_LandscapeDefinitionsItemh_INCLUDE__

#include <common/Defines.h>
#include <XML/XMLFile.h>
#include <map>
#include <string>

class LandscapeDefinitions;

template <class T>
class LandscapeDefinitionsItem
{
public:
	void clearItems()
	{
		typename std::map<std::string, T *>::iterator itor;
		for (itor = items_.begin();
			itor != items_.end();
			itor++)
		{
			T *item = (*itor).second;
			delete item;
		}
		items_.clear();
	}

	T *getItem(LandscapeDefinitions *defns,
		const char *fileName, bool load = false)
	{
		T *item = 0;
		typename std::map<std::string, T *>::iterator itor;
		itor = items_.find(fileName);
		if (itor != items_.end())
		{
			item = (*itor).second;
		}
		else if (load)
		{
			std::string dataFile = getDataFile(fileName);
			XMLFile file;
			if (!file.readFile(dataFile.c_str()) ||
				!file.getRootNode())
			{
				dialogMessage("Scorched Landscape", 
							formatString("Failed to parse \"%s\"\n"
							"%s", 
							dataFile.c_str(),
							file.getParserError()));
				return 0;
			}

			item = new T;
			if (!item->readXML(defns, file.getRootNode()))
			{
				dialogMessage("Scorched Landscape", 
					formatString("Failed to parse \"%s\"",
					dataFile.c_str()));
				return 0;
			}
			items_[fileName] = item;
		}

		return item;
	}

protected:
	std::map<std::string, T *> items_;
};

#endif

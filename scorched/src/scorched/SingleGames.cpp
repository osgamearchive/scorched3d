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

#include <scorched/SingleGames.h>
#include <common/Defines.h>
#include <XML/XMLFile.h>

SingleGames::SingleGames()
{

}

SingleGames::~SingleGames()
{
}

bool SingleGames::parse(const char *fileName)
{
	entries.clear();
	XMLFile file;
	if (!file.readFile(fileName))
	{
		dialogMessage("SingleGames", 
			"Failed to parse \"%s\":%s\n", 
					  fileName,
					  file.getParserError());
		return false;
	}
	if (!file.getRootNode())
	{
		dialogMessage("SingleGames",
					  "Failed to find single games definition file \"%s\"",
					  fileName);
		return false;		
	}

	XMLNode *mainNode = 0;
	if (!file.getRootNode()->getNamedChild("main", mainNode)) return false;
	if (!mainNode->getNamedChild("description", description)) return false;
	if (!mainNode->getNamedChild("icon", icon)) return false;
	if (!mainNode->getNamedChild("url", url)) return false;
	if (!checkDataFile(icon.c_str())) return false;

	XMLNode *gameNode = 0;
	while (file.getRootNode()->getNamedChild("game", gameNode, false))
	{
		Entry entry;
		if (!gameNode->getNamedChild("description", entry.description)) return false;
		if (!gameNode->getNamedChild("icon", entry.icon)) return false;
		if (!gameNode->getNamedChild("gamefile", entry.gamefile)) return false;
		if (!checkDataFile(entry.icon.c_str())) return false;
		if (!checkDataFile(entry.gamefile.c_str())) return false;
		if (!gameNode->failChildren()) return false;
		entries.push_back(entry);
	}

	return file.getRootNode()->failChildren();
}

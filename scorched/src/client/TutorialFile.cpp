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

#include <client/TutorialFile.h>

TutorialFile::TutorialFile() : file_(true)
{
}

TutorialFile::~TutorialFile()
{
}

bool TutorialFile::parseFile(const char *file)
{
	if (!file_.readFile(file) ||
		!file_.getRootNode())
	{
		dialogMessage("Scorched Tutorial", formatString(
					  "Failed to parse \"%s\"\n%s", 
					  file,
					  file_.getParserError()));
		return false;
	}

	std::string start;
	if (!file_.getRootNode()->getNamedChild("start", start)) return false;

	XMLNode *steps = 0;
	if (!file_.getRootNode()->getNamedChild("steps", steps)) return false;

	std::list<XMLNode *>::iterator stepitor;
	for (stepitor = steps->getChildren().begin();
		stepitor != steps->getChildren().end();
		stepitor++)
	{
		XMLNode *node = (*stepitor);

		if (node->getType() == XMLNode::XMLNodeType)
		{
			Entry *entry = new Entry();
			if (!node->getNamedChild("name", entry->name_)) return false;
			if (!node->getNamedChild("text", entry->text_)) return false;
			entries_[entry->name_] = entry;
		}
	}

	std::map<std::string, Entry *>::iterator findItor = entries_.find(start);
	if (findItor == entries_.end())
	{
		dialogMessage("TutorialFile", "Failed to find start node");
		return false;
	}
	start_ = (*findItor).second;

	return true;
}

XMLNode *TutorialFile::getText(const char *name)
{
	std::map<std::string, Entry *>::iterator findItor = entries_.find(name);
	if (findItor == entries_.end())
	{
		return 0;
	}
	return (*findItor).second->text_;
}

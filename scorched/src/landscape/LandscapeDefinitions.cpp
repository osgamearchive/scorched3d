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

#include <landscape/LandscapeDefinitions.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

bool LandscapeDefinitionsEntry::readXML(XMLNode *node)
{
	if (!node->getNamedChild("name", name)) return false;
	if (!node->getNamedChild("weight", weight)) return false;
	if (!node->getNamedChild("description", description)) return false;
	if (!node->getNamedChild("picture", picture)) return false;

	XMLNode *tex, *defn, *tmp;
	if (!node->getNamedChild("defn", defn)) return false;
	while (defn->getNamedChild("item", tmp, false, true))
		defns.push_back(tmp->getContent());
	if (!node->getNamedChild("tex", tex)) return false;
	while (tex->getNamedChild("item", tmp, false, true))
		texs.push_back(tmp->getContent());

	DIALOG_ASSERT(!texs.empty() && !defns.empty());
	return node->failChildren();
}

LandscapeDefinitions::LandscapeDefinitions()
{
}

LandscapeDefinitions::~LandscapeDefinitions()
{
}

void LandscapeDefinitions::clearLandscapeDefinitions()
{
	entries_.clear();
	while (!texs_.empty()) 
	{
		delete texs_.front();
		texs_.pop_front();
	}
	while (!defns_.empty())
	{
		delete defns_.front();
		defns_.pop_front();
	}
	while (!places_.empty())
	{
		delete places_.front();
		places_.pop_front();
	}
}

bool LandscapeDefinitions::readLandscapeDefinitions()
{
	if (!readPlaces()) return false;
	if (!readTexs()) return false;
	if (!readDefns()) return false;
	if (!readDefinitions()) return false;
	return true;
}

bool LandscapeDefinitions::readPlaces()
{
	// Load landscape definition file
	XMLFile file;
	if (!file.readFile(getDataFile("data/landscapesplace.xml")) ||
		!file.getRootNode())
	{
		dialogMessage("Scorched Landscape",
			"Failed to parse \"data/landscapesplace.xml\"\n%s",
			file.getParserError());
		return false;
	}
	// Itterate all of the landscapes in the file
	std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = file.getRootNode()->getChildren();
	for (childrenItor = children.begin();
		childrenItor != children.end();
		childrenItor++)
	{
		LandscapePlace *newPlace = new LandscapePlace;
		if (!newPlace->readXML(*childrenItor))
		{
			dialogMessage("Scorched Landscape",
				"Failed to parse  \"data/landscapesplace.xml\"");
			return false;
		}
		places_.push_back(newPlace);
	}
	return true;
}

bool LandscapeDefinitions::readTexs()
{
	// Load landscape definition file
	XMLFile file;
	if (!file.readFile(getDataFile("data/landscapestex.xml")) ||
		!file.getRootNode())
	{
		dialogMessage("Scorched Landscape",
			"Failed to parse \"data/landscapestex.xml\"\n%s",
			file.getParserError());
		return false;
	}
	// Itterate all of the landscapes in the file
	std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = file.getRootNode()->getChildren();
	for (childrenItor = children.begin();
		childrenItor != children.end();
		childrenItor++)
	{
		LandscapeTex *newTex = new LandscapeTex;
		if (!newTex->readXML(this, *childrenItor))
		{
			dialogMessage("Scorched Landscape",
				"Failed to parse  \"data/landscapestex.xml\"");
			return false;
		}
		texs_.push_back(newTex);
	}
	return true;
}

LandscapeTex *LandscapeDefinitions::getTex(const char *name)
{
	std::list<LandscapeTex*>::iterator itor;
	for (itor = texs_.begin();
		itor != texs_.end();
		itor++)
	{
		LandscapeTex *tex = (*itor);
		if (0 == strcmp(tex->name.c_str(), name)) return tex;
	}
	return 0;
}

bool LandscapeDefinitions::readDefns()
{
	// Load landscape definition file
	XMLFile file;
	if (!file.readFile(getDataFile("data/landscapesdefn.xml")) ||
		!file.getRootNode())
	{
		dialogMessage("Scorched Landscape",
			"Failed to parse \"data/landscapesdefn.xml\"\n%s",
			file.getParserError());
		return false;
	}
	// Itterate all of the landscapes in the file
	std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = file.getRootNode()->getChildren();
	for (childrenItor = children.begin();
		childrenItor != children.end();
		childrenItor++)
	{
		LandscapeDefn *newDefn = new LandscapeDefn;
		if (!newDefn->readXML(*childrenItor))
		{
			dialogMessage("Scorched Landscape",
				"Failed to parse  \"data/landscapesdefn.xml\"");
			return false;
		}
		defns_.push_back(newDefn);
	}
	return true;
}

LandscapePlace *LandscapeDefinitions::getPlace(const char *name)
{
	std::list<LandscapePlace *>::iterator itor;
	for (itor = places_.begin();
		itor != places_.end();
		itor++)
	{
		LandscapePlace *place = *itor;
		if (0 == strcmp(place->name.c_str(), name)) return place;
	}
	return 0;
}

LandscapeDefn *LandscapeDefinitions::getDefn(const char *name)
{
	std::list<LandscapeDefn *>::iterator itor;
	for (itor = defns_.begin();
		itor != defns_.end();
		itor++)
	{
		LandscapeDefn *defn = *itor;
		if (0 == strcmp(defn->name.c_str(), name)) return defn;
	}
	return 0;
}

bool LandscapeDefinitions::readDefinitions()
{
	// Load landscape definition file
	XMLFile file;
	if (!file.readFile(getDataFile("data/landscapes.xml")) ||
		!file.getRootNode())
	{
		dialogMessage("Scorched Landscape", 
					  "Failed to parse \"data/landscapes.xml\"\n%s", 
					  file.getParserError());
		return false;
	}
	// Itterate all of the landscapes in the file
	std::list<XMLNode *>::iterator childrenItor;
		std::list<XMLNode *> &children = file.getRootNode()->getChildren();
	for (childrenItor = children.begin();
		childrenItor != children.end();
		childrenItor++)
	{
		LandscapeDefinitionsEntry newDefn;
		if (!newDefn.readXML(*childrenItor))
		{
			dialogMessage("Scorched Landscape", 
					  "Failed to parse \"data/landscapes.xml\"");
			return false;
		}
		std::vector<std::string>::iterator checkItor;
		for (checkItor = newDefn.texs.begin();
			checkItor != newDefn.texs.end();
			checkItor++)
		{
			const char *name = (*checkItor).c_str();
			if (!getTex(name))
			{
				dialogMessage("Scorched Landscape", 
						"Failed to find tex named \"%s\" in \"data/landscapes.xml\"",
						name);
				return false;
			}
		}
		for (checkItor = newDefn.defns.begin();
			checkItor != newDefn.defns.end();
			checkItor++)
		{
			const char *name = (*checkItor).c_str();
			if (!getDefn(name))
			{
				dialogMessage("Scorched Landscape", 
						"Failed to find defn named \"%s\" in \"data/landscapes.xml\"",
						name);
				return false;
			}
		}

		entries_.push_back(newDefn);
	}
	return true;
}

bool LandscapeDefinitions::landscapeEnabled(OptionsGame &context, 
											const char *name)
{
	std::string landscapes = context.getLandscapes();
	if (landscapes.empty()) return true; // Default un-initialized state

	char *token = strtok((char *) landscapes.c_str(), ":");
	while(token != 0)
	{
		if (0 == strcmp(token, name)) return true;
		token = strtok(0, ":");
	}
	return false;
}

LandscapeDefinition *LandscapeDefinitions::getRandomLandscapeDefn(
	OptionsGame &context)
{
	// Build a list of the maps that are enabled
	float totalWeight = 0.0f;
	std::list<LandscapeDefinitionsEntry *> passedLandscapes;
	std::list<LandscapeDefinitionsEntry>::iterator itor;
	for (itor = entries_.begin();
		itor != entries_.end();
		itor++)
	{
		LandscapeDefinitionsEntry &result = *itor;
		if (landscapeEnabled(context, result.name.c_str()))
		{
			passedLandscapes.push_back(&result);
			totalWeight += result.weight;
		}
	}

	// Check we have a least one map
	if (passedLandscapes.empty()) return 0;

	// Map cycle mode
	LandscapeDefinitionsEntry *result = 0;
	if (context.getCycleMaps())
	{
		// Just cycle through the maps
		static LandscapeDefinitionsEntry* last = 0;
		bool next = false;
		result = passedLandscapes.front();

		std::list<LandscapeDefinitionsEntry*>::iterator passedItor;
		for (passedItor = passedLandscapes.begin();
			passedItor != passedLandscapes.end();
			passedItor++)
		{
			LandscapeDefinitionsEntry *current = *passedItor;
			if (next) 
			{
				result = current;
				break;
			}
			if (current == last) next = true;
		}
		last = result;
	}
	else
	{
		// Choose a map based on probablity
		float pos = RAND * totalWeight;
		float soFar = 0.0f;

		std::list<LandscapeDefinitionsEntry*>::iterator passedItor;
		for (passedItor = passedLandscapes.begin();
			passedItor != passedLandscapes.end();
			passedItor++)
		{
			LandscapeDefinitionsEntry *current = *passedItor;
			soFar += current->weight;

			if (pos <= soFar)
			{
				result = current;
				break;
			}
		}
	}

	// Check we found map
	if (!result) return 0;

	// Return the chosen definition
	unsigned int texPos = int(RAND * float(result->texs.size()));
	unsigned int defnPos = int(RAND * float(result->defns.size()));
	DIALOG_ASSERT(texPos < result->texs.size());
	DIALOG_ASSERT(defnPos < result->defns.size());
	std::string tex = result->texs[texPos].c_str();
	std::string defn = result->defns[defnPos].c_str();
	unsigned int seed = (unsigned int) rand();
	LandscapeDefinition *entry = 
		new LandscapeDefinition(
			tex.c_str(), defn.c_str(), seed);
	return entry;
}


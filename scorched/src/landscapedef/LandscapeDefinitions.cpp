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

#include <landscapedef/LandscapeDefinitions.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeDefn.h>
#include <landscapedef/LandscapeInclude.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

LandscapeDefinitions::LandscapeDefinitions() :
	lastDefinition_(0),
	texs_("Tex"),
	defns_("Defns"),
	include_("Include")
{
}

LandscapeDefinitions::~LandscapeDefinitions()
{
}

void LandscapeDefinitions::clearLandscapeDefinitions()
{
	LandscapeDefinitionsBase::clearLandscapeDefinitions();

	defns_.clearItems();
	texs_.clearItems();
	include_.clearItems();
}

LandscapeTex *LandscapeDefinitions::getTex(const char *file, bool load)
{
	return texs_.getItem(this, file, load, true);
}

LandscapeInclude *LandscapeDefinitions::getInclude(const char *file, bool load)
{
	return include_.getItem(this, file, load, true);
}

LandscapeDefn *LandscapeDefinitions::getDefn(const char *file, bool load)
{
	return defns_.getItem(this, file, load, true);
}

bool LandscapeDefinitions::readLandscapeDefinitions()
{
	// Parse base landscape information
	if (!LandscapeDefinitionsBase::readLandscapeDefinitions()) return false;

	// Now check that the landscape tex and defn files parse correctly
	std::list<LandscapeDefinitionsEntry>::iterator itor;
	for (itor = entries_.begin();
		itor != entries_.end();
		itor++)
	{
		LandscapeDefinitionsEntry &entry = (*itor);
		std::vector<std::string>::iterator itor2;

		std::vector<std::string> &defns = entry.defns;
		for (itor2 = defns.begin();
			itor2 != defns.end();
			itor2++)
		{
			const char *landscapeDefnFile = (*itor2).c_str();
			LandscapeDefn *landscapeDefn = getDefn(landscapeDefnFile, true);
			if (!landscapeDefn) return false;
		}

		std::vector<std::string> &texs = entry.texs;
		for (itor2 = texs.begin();
			itor2 != texs.end();
			itor2++)
		{
			const char *landscapeTexFile = (*itor2).c_str();
			LandscapeTex *landscapeTex = getTex(landscapeTexFile, true);
			if (!landscapeTex) return false;
		}
	}

	return true;
}

const char *LandscapeDefinitions::getLeastUsedFile(std::vector<std::string> &files)
{
	DIALOG_ASSERT(!files.empty());

	const char *result = "";
	int usedTimes = INT_MAX;

	std::vector<std::string>::iterator itor;
	for (itor = files.begin();
		itor != files.end();
		itor++)
	{
		std::string &file = (*itor);

		int used = 0;
		std::map<std::string, int>::iterator findItor = 
			usedFiles_.find(file);
		if (findItor != usedFiles_.end())
		{
			used = (*findItor).second;
		}

		if (used < usedTimes)
		{
			usedTimes = used;
			result = file.c_str();
		}
	}
	
	usedFiles_[result] = usedTimes + 1;
	return result;
}

void LandscapeDefinitions::checkEnabled(OptionsGame &context)
{
	std::list<LandscapeDefinitionsEntry>::iterator itor;
	for (itor = entries_.begin();
		itor != entries_.end();
		itor++)
	{
		LandscapeDefinitionsEntry &result = *itor;
		if (landscapeEnabled(context, result.name.c_str()))
		{
			return;
		}
	}

	dialogExit("Scorched3D", formatString(
		"No existing landscapes are enabled (Landscapes : %s)",
		context.getLandscapes()));
}

LandscapeDefinition LandscapeDefinitions::getRandomLandscapeDefn(
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
	DIALOG_ASSERT(!passedLandscapes.empty());

	// Map cycle mode
	LandscapeDefinitionsEntry *result = 0;
	if (context.getCycleMaps())
	{
		// Just cycle through the maps
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
			if (current == lastDefinition_) next = true;
		}
		lastDefinition_ = result;
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
	if (!result)
	{
		dialogExit("Scorched3D",
			"Failed to select a landscape definition");
	}

	// Return the chosen definition
	std::string tex = getLeastUsedFile(result->texs);
	std::string defn = getLeastUsedFile(result->defns);
	unsigned int seed = (unsigned int) rand();

	LandscapeDefinition entry(
		tex.c_str(), defn.c_str(), seed, result->name.c_str());
	return entry;
}


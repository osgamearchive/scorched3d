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

LandscapeDefinitions *LandscapeDefinitions::instance_ = 0;

LandscapeDefinitions *LandscapeDefinitions::instance()
{
	if (!instance_)	
	{
		instance_ = new LandscapeDefinitions;
	}
	return instance_;
}

LandscapeDefinitions::LandscapeDefinitions()
{
}

LandscapeDefinitions::~LandscapeDefinitions()
{
}

void LandscapeDefinitions::clearLandscapeDefinitions()
{
	definitions_.clear();
}

bool LandscapeDefinitions::readLandscapeDefinitions()
{
	// Load landscape definition file
	XMLFile file;
	if (!file.readFile(getDataFile("data/landscapes.xml")))
	{
		dialogMessage("Scorched Landscape", 
					  "Failed to parse \"data/landscapes.xml\"\n%s", 
					  file.getParserError());
		return false;
	}

	// Check file exists
	if (!file.getRootNode())
	{
		dialogMessage("Scorched Landscape",
					"Failed to find landscape definition file \"data/landscapes.xml\"");
		return false;		
	}

	// Itterate all of the landscapes in the file
	std::list<XMLNode *>::iterator childrenItor;
		std::list<XMLNode *> &children = file.getRootNode()->getChildren();
	for (childrenItor = children.begin();
		childrenItor != children.end();
		childrenItor++)
	{
		LandscapeDefinition newDefn;
		if (!newDefn.readXML(*childrenItor)) return false;
		definitions_.push_back(newDefn);
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

LandscapeDefinition &LandscapeDefinitions::getRandomLandscapeDefn(
	OptionsGame &context)
{
	float totalWeight = 0.0f;
	std::list<LandscapeDefinition *> passedLandscapes;
	std::vector<LandscapeDefinition>::iterator itor;
	for (itor = definitions_.begin();
		itor != definitions_.end();
		itor++)
	{
		LandscapeDefinition &result = *itor;
		if (landscapeEnabled(context, result.name.c_str()))
		{
			passedLandscapes.push_back(&result);
			totalWeight += result.weight;
		}
	}

	float pos = RAND * totalWeight;
	float soFar = 0.0f;

	std::list<LandscapeDefinition *>::iterator passedItor;
	for (passedItor = passedLandscapes.begin();
		passedItor != passedLandscapes.end();
		passedItor++)
	{
		LandscapeDefinition *result = *passedItor;
		soFar += result->weight;

		if (pos <= soFar)
		{
			result->generate();
			return *result;
		}

	}

	dialogMessage("Landscape",
		"No landscapes marked as active");
	exit(0);
	static LandscapeDefinition null;
	return null;
}

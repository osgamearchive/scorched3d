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

#if !defined(__INCLUDE_LandscapeDefinitionsh_INCLUDE__)
#define __INCLUDE_LandscapeDefinitionsh_INCLUDE__

#include <landscape/LandscapeDefinition.h>
#include <landscape/LandscapeTex.h>
#include <landscape/LandscapeDefn.h>
#include <string>
#include <list>

class LandscapeDefinitionsEntry
{
public:
	std::string name; 
	std::vector<std::string> texs; 
	std::vector<std::string> defns; 
	float weight; // The posibility this defn will be choosen
	std::string description;  // Description of this landscape definition type
	std::string picture; // Visible view of this landscape definition type

	virtual bool readXML(XMLNode *node);
};

class OptionsGame;
class LandscapeDefinitions
{
public:
	LandscapeDefinitions();
	virtual ~LandscapeDefinitions();

	bool readLandscapeDefinitions();
	void clearLandscapeDefinitions();

	LandscapeDefinition *getRandomLandscapeDefn(OptionsGame &context);
	LandscapeTex *getTex(const char *name);
	LandscapeDefn *getDefn(const char *name);

	bool landscapeEnabled(OptionsGame &context, const char *name);
	std::list<LandscapeDefinitionsEntry> &getAllLandscapes() 
		{ return entries_; }

protected:
	std::list<LandscapeDefinitionsEntry> entries_;
	std::list<LandscapeTex*> texs_;
	std::list<LandscapeDefn*> defns_;

	bool readDefinitions();
	bool readTexs();
	bool readDefns();

};

#endif

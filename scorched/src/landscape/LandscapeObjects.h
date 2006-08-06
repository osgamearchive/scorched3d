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

#if !defined(__INCLUDE_LandscapeObjectsh_INCLUDE__)
#define __INCLUDE_LandscapeObjectsh_INCLUDE__

#include <common/ProgressCounter.h>
#include <common/RandomGenerator.h>
#include <landscape/LandscapeObjectsEntry.h>
#include <landscape/LandscapeObjectsGroupEntry.h>
#include <landscape/LandscapeObjectsSetEntry.h>
#include <placement/PlacementShadowDefinition.h>
#include <engine/ScorchedContext.h>
#include <map>
#include <list>
#include <string>

class LandscapePlace;
class HeightMap;
class LandscapeObjects
{
public:
	LandscapeObjects();
	virtual ~LandscapeObjects();

	void draw();
	void simulate(float frameTime);
	void generate(RandomGenerator &generator, 
		LandscapePlace &place,
		ScorchedContext &context,
		unsigned int &playerId,
		ProgressCounter *counter = 0);

	// Objects
	void removeAllObjects();
	void removeObjects(ScorchedContext &context,
		unsigned int x, unsigned int y, unsigned int r,
		unsigned int playerId);
	void burnObjects(ScorchedContext &context,
		unsigned int x, unsigned int y, 
		unsigned int playerId);
	void addObject(unsigned int x, unsigned int y, 
		LandscapeObjectsEntry *entry);

	// Groups
	void removeFromGroups(LandscapeObjectEntryBase *obj);
	LandscapeObjectsGroupEntry *getGroup(const char *name, HeightMap *create = 0);
	void clearGroups();

	// Accessors
	std::multimap<unsigned int, LandscapeObjectsEntry*> &getEntries() { return entries_; }
	std::list<PlacementShadowDefinition::Entry> &getShadows() { return shadows_; }

protected:
	std::multimap<unsigned int, LandscapeObjectsEntry*> entries_;
	std::map<std::string, LandscapeObjectsGroupEntry*> groups_;
	std::list<PlacementShadowDefinition::Entry> shadows_;

};

#endif

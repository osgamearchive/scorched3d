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

#include <landscapedef/LandscapeDefinitionsBase.h>
#include <landscapedef/LandscapeDefinitionsItem.h>
#include <landscapedef/LandscapeDefinition.h>

class LandscapeDefinitions;
class LandscapePlace;
class LandscapeSound;
class LandscapeBoids;
class LandscapeShips;
class LandscapeDefn;
class LandscapeTex;
class LandscapeEvents;
class OptionsGame;

class LandscapeDefinitions : public LandscapeDefinitionsBase
{
public:
	LandscapeDefinitions();
	virtual ~LandscapeDefinitions();

	virtual bool readLandscapeDefinitions();
	virtual void clearLandscapeDefinitions();

	void checkEnabled(OptionsGame &context);
	LandscapeDefinition getRandomLandscapeDefn(OptionsGame &context);
	LandscapeTex *getTex(const char *file, bool load = false);
	LandscapeDefn *getDefn(const char *file, bool load = false);
	LandscapePlace *getPlace(const char *file, bool load = false);
	LandscapeSound *getSound(const char *file, bool load = false);
	LandscapeBoids *getBoids(const char *file, bool load = false);
	LandscapeShips *getShips(const char *file, bool load = false);
	LandscapeEvents *getEvents(const char *file, bool load = false);

protected:
	LandscapeDefinitionsEntry* lastDefinition_;
	std::map<std::string, int> usedFiles_;

	LandscapeDefinitionsItem<LandscapeTex> texs_;
	LandscapeDefinitionsItem<LandscapeDefn> defns_;
	LandscapeDefinitionsItem<LandscapePlace> places_;
	LandscapeDefinitionsItem<LandscapeSound> sounds_;
	LandscapeDefinitionsItem<LandscapeBoids> boids_;
	LandscapeDefinitionsItem<LandscapeShips> ships_;
	LandscapeDefinitionsItem<LandscapeEvents> events_;

	const char *getLeastUsedFile(std::vector<std::string> &files);
};

#endif

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

#if !defined(__INCLUDE_LanscapeMapsh_INCLUDE__)
#define __INCLUDE_LanscapeMapsh_INCLUDE__

#include <landscape/MovementMap.h>
#include <landscape/NapalmMap.h>
#include <coms/ComsLevelMessage.h>
#include <common/ProgressCounter.h>

class ScorchedContext;
class LandscapeTex;
class LandscapeDefn;
class LandscapeMaps
{
public:
	LandscapeMaps();
	virtual ~LandscapeMaps();

	// Returns the inner maps
	// Joy, I have used letters that all look the same :(
	HeightMap &getHMap() { return map_; }
	HeightMap &getRMap() { return rmap_; }
	HeightMap &getSMap() { return smap_; }
	MovementMap &getMMap() { return mmap_; }
	NapalmMap &getNMap() { return nmap_; }

	LandscapeTex &getTex(ScorchedContext &context);
	LandscapeDefn &getDefn(ScorchedContext &context);
	unsigned int getSeed();
	LandscapeDefinition *getLandDfn() { return storedHdef_; }
	bool getRoof() { return roof_; }
	bool getSurround() { return surround_; }

	// Generates the next level
	void generateHMap(ScorchedContext &context,
		LandscapeDefinition *hdef,
		ProgressCounter *counter = 0);

	// Compresses the level into a form that can be
	// sent to the clients
	bool generateHMapDiff(ScorchedContext &context,
		ComsLevelMessage &message);

	// De-compresses the level sent from the server
	// into the hmap structure
	bool generateHMapFromDiff(ScorchedContext &context,
		ComsLevelMessage &message,
		ProgressCounter *counter = 0);

protected:
	// The current level's heightmap
	HeightMap map_;

	// The current levels + players movement possibilities
	MovementMap mmap_;

	// The part of the landscape covered by napalm
	NapalmMap nmap_;

	// The roof of the landscape
	bool roof_;
	HeightMap rmap_;

	// The surround of the landscape
	bool surround_;
	HeightMap smap_;

	// The values at the start of the level
	float *storedMap_;
	LandscapeDefinition *storedHdef_;
};

#endif

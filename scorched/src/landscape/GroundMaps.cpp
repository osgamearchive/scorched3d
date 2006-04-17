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

#include <landscape/GroundMaps.h>
#include <landscape/HeightMapLoader.h>
#include <landscapedef/LandscapePlace.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeDefinitions.h>
#include <landscapedef/LandscapeDefinitionCache.h>
#include <common/Logger.h>
#include <tankai/TankAIAdder.h>

GroundMaps::GroundMaps(LandscapeDefinitionCache &defnCache) :
	defnCache_(defnCache)
{
}

GroundMaps::~GroundMaps()
{
}

void GroundMaps::generateMaps(
	ScorchedContext &context,
	ProgressCounter *counter)
{
	generateHMap(context, counter);
	generateObjects(context, counter);
	nmap_.create(getMapWidth(), getMapHeight());
}

void GroundMaps::generateHMap(
	ScorchedContext &context, 
	ProgressCounter *counter)
{
	// Initialize the ground and surround maps
	map_.create(defnCache_.getDefn()->landscapewidth, 
		defnCache_.getDefn()->landscapeheight);

	// Surround map is 640 units larger the heightmap (on each side)
	// And each surround map square is equal to 16 heightmap units
	int surroundWidth = defnCache_.getDefn()->landscapewidth + 640 * 2;
	int surroundHeight = defnCache_.getDefn()->landscapeheight + 640 * 2;
	surroundWidth /= 16;
	surroundHeight /= 16;
	smap_.create(surroundWidth, surroundHeight);

	// Generate the landscape
	bool levelSurround = false;
	if (!HeightMapLoader::generateTerrain(
		defnCache_.getSeed(),
		defnCache_.getDefn()->heightmap,
		map_,
		levelSurround,
		counter))
	{
		dialogExit("Landscape", "Failed to generate landscape");
	}

	// Generate the surround
	if (defnCache_.getDefn()->surround->getType() != LandscapeDefnType::eNone)
	{
		if (!HeightMapLoader::generateTerrain(
			defnCache_.getSeed() + 1,
			defnCache_.getDefn()->surround,
			smap_,
			levelSurround,
			counter))
		{
			dialogExit("Landscape", "Failed to generate surround");
		}

		if (levelSurround)
		{
			for (int j=40; j<=56; j++)
			{
				for (int i=40; i<=56; i++)
				{
					smap_.setHeight(i, j, 0);
				}
			}
			smap_.generateNormals(0, smap_.getMapWidth(), 0, smap_.getMapHeight());
		}
	}

	// Save this height map for later
	map_.resetMinHeight();
	map_.backup();
}

void GroundMaps::generateObjects(
	ScorchedContext &context,
	ProgressCounter *counter)
{
	LandscapeTex *tex = defnCache_.getTex();
	LandscapeDefn *defn = defnCache_.getDefn();

	// Add objects to the landscape (if any)
	// Do this now as it adds shadows to the mainmap
	unsigned int playerId = TankAIAdder::MIN_TARGET_ID;
	objects_.removeAllObjects();
	{
		// Do this for the definition file
		std::vector<LandscapePlace *>::iterator itor;
		for (itor = defn->texDefn.placements.begin();
			itor != defn->texDefn.placements.end();
			itor++)
		{
			LandscapePlace *place = (*itor);
			RandomGenerator objectsGenerator;
			objectsGenerator.seed(defnCache_.getSeed());
			objects_.generate(objectsGenerator, *place, 
				context, playerId, counter);
		}
	}
	{
		// Do this for the texture file
		std::vector<LandscapePlace *>::iterator itor;
		for (itor = tex->texDefn.placements.begin();
			itor != tex->texDefn.placements.end();
			itor++)
		{
			LandscapePlace *place = (*itor);
			RandomGenerator objectsGenerator;
			objectsGenerator.seed(defnCache_.getSeed());
			objects_.generate(objectsGenerator, *place, 
				context, playerId, counter);
		}
	}

	// Deform the main map with respect to the objects
	{
		std::list<PlacementShadowDefinition::Entry> &shadows = 
			objects_.getShadows();
		std::list<PlacementShadowDefinition::Entry>::iterator itor;
		for (itor = shadows.begin();
			itor != shadows.end();
			itor++)
		{
			PlacementShadowDefinition::Entry &entry = (*itor);
			entry.definition_->updateLandscapeHeight(
				context,
				entry.position_, entry.size_);
		}
	}
}

float GroundMaps::getHeight(int w, int h)
{
	// Check if we are in the normal heightmap
	if (w < 0 || h < 0 || w > map_.getMapWidth() || h > map_.getMapHeight())
	{
		// Check for no surround
		if (defnCache_.getDefn()->surround->getType() == LandscapeDefnType::eNone)
		{
			return 0.0f;
		}

		// Check if we are in the surround
		if (w < -640.0f || h < -640.0f || 
			w > map_.getMapWidth() + 640 || 
			h > map_.getMapHeight() + 640)
		{
			return 0.0f;
		}

		// Return surround
		int a = (w + 640) / 16;
		int b = (h + 640) / 16;
		return smap_.getHeight(a, b);
	}
	return map_.getHeight(w, h);
}

float GroundMaps::getInterpHeight(float w, float h)
{
	if (w < 0 || h < 0 || w > map_.getMapWidth() || h > map_.getMapHeight())
	{
		return getHeight((int) w, (int) h);
	}
	return map_.getInterpHeight(w, h);
}

Vector &GroundMaps::getNormal(int w, int h)
{
	if (w < 0 || h < 0 || w > map_.getMapWidth() || h > map_.getMapHeight())
	{
		static Vector up(0.0f, 0.0f, 1.0f);
		return up;
	}
	return map_.getNormal(w, h);
}

void GroundMaps::getInterpNormal(float w, float h, Vector &normal)
{
	if (w < 0 || h < 0 || w > map_.getMapWidth() || h > map_.getMapHeight())
	{
		static Vector up(0.0f, 0.0f, 1.0f);
		normal = up;
	}
	map_.getInterpNormal(w, h, normal);
}

bool GroundMaps::getIntersect(Line &direction, Vector &intersect)
{
	return map_.getIntersect(direction, intersect);
}


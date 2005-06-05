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

#include <landscape/LandscapeObjects.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/LandscapePlace.h>
#include <landscape/LandscapeObjectsPlacement.h>
#include <client/ScorchedClient.h>
#include <tank/TankContainer.h>
#include <GLEXT/GLCamera.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>
#include <stdio.h>

static inline float approx_distance(float  dx, float dy)
{
   float approx = (dx * dx) + (dy * dy);
   return approx;
}

LandscapeObjects::LandscapeObjects()
{
}

LandscapeObjects::~LandscapeObjects()
{
}

void LandscapeObjects::draw()
{
	if (OptionsDisplay::instance()->getNoTrees()) return;

	GLState state(GLState::TEXTURE_ON | GLState::BLEND_ON | GLState::DEPTH_ON);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.00f);

	Vector &cameraPos = GLCamera::getCurrentCamera()->getCurrentPos();

	std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator 
		itor = entries_.begin();
	std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator 
		enditor = entries_.end();
	for (; itor != enditor; itor++)
	{
		LandscapeObjectsEntry *entry = (*itor).second;

		float distance = approx_distance(
				cameraPos[0] - entry->posX,
				cameraPos[1] - entry->posY);

		entry->render(distance);
	}		

	glDisable(GL_ALPHA_TEST);
}

static inline unsigned int pointToUInt(unsigned int x, unsigned int y)
{
	return (x << 16) | (y & 0xffff);
}

void LandscapeObjects::generate(RandomGenerator &generator, 
	LandscapePlace &place,
	ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Populating Landscape");

	// Start with a clean slate
	removeAllObjects();

	// TODO allow turning of this off during game
	if (OptionsDisplay::instance()->getNoTrees()) return;

	// Generate all the objects using the objects definitions
	for (unsigned int i=0; i<place.objects.size(); i++)
	{
		// Check which type of objects placement will be used
		std::string placementtype = place.objectstype[i];
		if (0 == strcmp(placementtype.c_str(), "trees"))
		{
			// Trees type placement
			LandscapeObjectPlacementTrees gen;
			LandscapePlaceObjectsPlacementTree *placement =
				(LandscapePlaceObjectsPlacementTree *)
					place.objects[i];
			gen.generateObjects(generator, *placement, counter);
		}
		else if (0 == strcmp(placementtype.c_str(), "mask"))
		{
			// Mask type placement
			LandscapeObjectPlacementMask gen;
			LandscapePlaceObjectsPlacementMask *placement =
				(LandscapePlaceObjectsPlacementMask *)
					place.objects[i];
			gen.generateObjects(generator, *placement, counter);
		}
		else
		{
			dialogExit("LandscapeObjects",
				"Error: Unknown placement type \"%s\"",
				placementtype.c_str());
		}
	}
}

void LandscapeObjects::addObject(unsigned int x, unsigned int y,
	LandscapeObjectsEntry *entry)
{
	// Add the entry
	unsigned int point = pointToUInt(x, y);
	entries_.insert(
		std::pair<unsigned int, LandscapeObjectsEntry*>(point, entry));	
}

void LandscapeObjects::removeAllObjects()
{
	// Clear any current trees
	std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator 
		itor = entries_.begin();
	std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator 
		enditor = entries_.end();
	for (; itor != enditor; itor++)
	{
		LandscapeObjectsEntry *entry = (*itor).second;
		delete entry;
	}	
	entries_.clear();
}

void LandscapeObjects::removeObjects(unsigned int x, unsigned int y)
{
	unsigned int point = pointToUInt((unsigned int)x, (unsigned int)y);

	std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator lower =
		entries_.lower_bound(point);
    std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator upper =
		entries_.upper_bound(point);
    std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator iter;
	for (iter = lower; iter != upper; iter++)
	{
		LandscapeObjectsEntry *entry = (*iter).second;
		delete entry;
	}
	entries_.erase(lower, upper);
}

void LandscapeObjects::burnObjects(unsigned int x, unsigned int y)
{
	unsigned int point = pointToUInt((unsigned int)x, (unsigned int)y);

	std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator lower =
		entries_.lower_bound(point);
    std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator upper =
		entries_.upper_bound(point);
    std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator iter;
	for (iter = lower; iter != upper; iter++)
	{
		LandscapeObjectsEntry *entry = (*iter).second;
		entry->burnt = true;
	}
}

void LandscapeObjects::removeAroundTanks()
{
	// Remove trees around tanks
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedClient::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		 itor != tanks.end();
		 itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getState().getState() == TankState::sNormal)
		{
			Vector &tankPos = tank->getPhysics().getTankPosition();
			for (int x=int(tankPos[0]) - 3; x<=int(tankPos[0])+3; x++)
			{
				for (int y=int(tankPos[1]) - 3; y<=int(tankPos[1])+3; y++)
				{
					removeObjects(x, y);
				}
			}
		}
	}
}

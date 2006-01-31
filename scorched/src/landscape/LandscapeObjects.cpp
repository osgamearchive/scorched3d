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
#include <tank/TankContainer.h>
#include <GLEXT/GLCamera.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>
#include <weapons/AccessoryStore.h>
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

void LandscapeObjects::clearGroups()
{
	std::map<std::string, LandscapeObjectsGroupEntry*>::iterator itor;
	for (itor = groups_.begin();
		itor != groups_.end();
		itor++)
	{
		LandscapeObjectsGroupEntry *entry = (*itor).second;
		delete entry;
	}
	groups_.clear();
}

LandscapeObjectsGroupEntry *LandscapeObjects::getGroup(
	const char *name, HeightMap *create)
{
	std::map<std::string, LandscapeObjectsGroupEntry*>::iterator findItor =
		groups_.find(name);
	if (findItor != groups_.end())
	{
		return (*findItor).second;
	}
	if (create)
	{
		LandscapeObjectsGroupEntry *entry = new LandscapeObjectsGroupEntry(*create);
		groups_[name] = entry;
		return entry;
	}
	return 0;
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
	ScorchedContext &context,
	ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Populating Landscape");

	// allow turning of this off during game
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
			gen.generateObjects(generator, *placement, context, counter);
		}
		else if (0 == strcmp(placementtype.c_str(), "mask"))
		{
			// Mask type placement
			LandscapeObjectPlacementMask gen;
			LandscapePlaceObjectsPlacementMask *placement =
				(LandscapePlaceObjectsPlacementMask *)
					place.objects[i];
			gen.generateObjects(generator, *placement, context, counter);
		}
		else if (0 == strcmp(placementtype.c_str(), "direct"))
		{
			// Direct type placement
			LandscapeObjectPlacementDirect gen;
			LandscapePlaceObjectsPlacementDirect *placement =
				(LandscapePlaceObjectsPlacementDirect *)
					place.objects[i];
			gen.generateObjects(generator, *placement, context, counter);
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
	// Clear any current objects
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

	// Make sure all groups are cleared
	clearGroups();
}

void LandscapeObjects::removeObjects(
	ScorchedContext &context,
	unsigned int x, unsigned int y, unsigned int r,
	unsigned int playerId)
{
	for (unsigned int a=x-r; a<=x+r; a++)
	{
		for (unsigned int b=y-r; b<=y+r; b++)
		{
			unsigned int point = pointToUInt(a, b);

			std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator lower =
				entries_.lower_bound(point);
			std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator upper =
				entries_.upper_bound(point);
			std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator iter;
			for (iter = lower; iter != upper; iter++)
			{
				LandscapeObjectsEntry *entry = (*iter).second;
				if (playerId != 0 &&
					entry->removeaction.c_str()[0] &&
					0 != strcmp(entry->removeaction.c_str(), "none"))
				{
					Accessory *accessory = context.accessoryStore->findByPrimaryAccessoryName(
						entry->removeaction.c_str());
					if (accessory && accessory->getAction()->getType() == 
						AccessoryPart::AccessoryWeapon)
					{
						Weapon *weapon = (Weapon *) accessory->getAction();
						Vector position(entry->posX, entry->posY, entry->posZ);
						weapon->fireWeapon(context, playerId, position, Vector::nullVector);
					}
				}

				if (entry->group) entry->group->removeObject(a, b);
				delete entry;
			}
			entries_.erase(lower, upper);
		}
	}
}

void LandscapeObjects::burnObjects(
	ScorchedContext &context,
	unsigned int x, unsigned int y, 
	unsigned int playerId)
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
		if (!entry->burnt)
		{
			entry->burnt = true;
			
			if (playerId != 0 &&
				entry->burnaction.c_str()[0] &&
				0 != strcmp(entry->burnaction.c_str(), "none"))
			{
				Accessory *accessory = context.accessoryStore->findByPrimaryAccessoryName(
					entry->burnaction.c_str());
				if (accessory && accessory->getAction()->getType() == 
					AccessoryPart::AccessoryWeapon)
				{
					Weapon *weapon = (Weapon *) accessory->getAction();
					Vector position(entry->posX, entry->posY, entry->posZ);
					weapon->fireWeapon(context, playerId, position, Vector::nullVector);
				}
			}
		}
	}
}

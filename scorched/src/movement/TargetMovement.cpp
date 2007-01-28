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

#include <movement/TargetMovement.h>
#include <movement/TargetMovementEntryShips.h>
#include <movement/TargetMovementEntryBoids.h>
#include <movement/TargetMovementEntrySpline.h>
#include <common/RandomGenerator.h>
#include <engine/ScorchedContext.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeDefn.h>

TargetMovement::TargetMovement()
{
}

TargetMovement::~TargetMovement()
{
}

void TargetMovement::generate(ScorchedContext &context)
{
	while (!movements_.empty())
	{
		TargetMovementEntry *movement = movements_.back();
		movements_.pop_back();
		delete movement;
	}

	LandscapeTex &tex = 
		*context.landscapeMaps->getDefinitions().getTex();
	LandscapeDefn &defn = 
		*context.landscapeMaps->getDefinitions().getDefn();
	RandomGenerator random;
	random.seed(context.landscapeMaps->getDefinitions().getSeed());

	addMovements(context, random, tex.texDefn.includes);
	addMovements(context, random, defn.texDefn.includes);
}

void TargetMovement::addMovements(ScorchedContext &context, 
	RandomGenerator &random, 
	std::vector<LandscapeInclude *> &movements)
{
	std::vector<LandscapeInclude *>::iterator itor;
	for (itor = movements.begin();
		itor != movements.end();
		itor++)
	{
		LandscapeInclude *movement = (*itor);
		addMovementType(context, random, movement->movements);
	}
}

void TargetMovement::addMovementType(ScorchedContext &context, 
	RandomGenerator &random, 
	std::vector<LandscapeMovementType *> &movementtypes)
{
	std::vector<LandscapeMovementType *>::iterator itor;
	for (itor = movementtypes.begin();
		itor != movementtypes.end();
		itor++)
	{
		LandscapeMovementType *movementtype = (*itor);

		TargetMovementEntry *entry = 0;
		switch(movementtype->getType())
		{
		case LandscapeMovementType::eBoids:
			entry = new TargetMovementEntryBoids();
			break;
		case LandscapeMovementType::eShips:
			entry = new TargetMovementEntryShips();
			break;
		case LandscapeMovementType::eSpline:
			entry = new TargetMovementEntrySpline();
			break;
		default:
			DIALOG_ASSERT(0);
			break;
		}
		entry->generate(context, random, movementtype);
		movements_.push_back(entry);
	}
}

void TargetMovement::simulate(float frameTime)
{
	std::vector<TargetMovementEntry *>::iterator itor;
	for (itor = movements_.begin();
		itor != movements_.end();
		itor++)
	{
		TargetMovementEntry *movementEntry = *itor;
		movementEntry->simulate(frameTime * 20.0f);
	}
}

void TargetMovement::draw()
{
	std::vector<TargetMovementEntry *>::iterator itor;
	for (itor = movements_.begin();
		itor != movements_.end();
		itor++)
	{
		TargetMovementEntry *movementEntry = *itor;
		movementEntry->draw();
	}
}

bool TargetMovement::writeMessage(NetBuffer &buffer)
{
	std::vector<TargetMovementEntry *>::iterator itor;
	for (itor = movements_.begin();
		itor != movements_.end();
		itor++)
	{
		TargetMovementEntry *movementEntry = *itor;
		if (!movementEntry->writeMessage(buffer)) return false;
	}
	return true;
}

bool TargetMovement::readMessage(NetBufferReader &reader)
{
	std::vector<TargetMovementEntry *>::iterator itor;
	for (itor = movements_.begin();
		itor != movements_.end();
		itor++)
	{
		TargetMovementEntry *movementEntry = *itor;
		if (!movementEntry->readMessage(reader)) return false;
	}
	return true;
}
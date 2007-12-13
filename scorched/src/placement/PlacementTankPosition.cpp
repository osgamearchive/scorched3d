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

#include <placement/PlacementTankPosition.h>
#include <landscapedef/LandscapeDefn.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapemap/DeformLandscape.h>
#include <tank/TankContainer.h>
#include <tank/TankPosition.h>
#include <tank/TankState.h>
#include <target/TargetLife.h>
#include <target/TargetSpace.h>
#include <GLEXT/GLImageFactory.h>

void PlacementTankPosition::flattenTankPositions(std::list<FixedVector> &tankPositions, 
	ScorchedContext &context)
{
	if (context.serverMode)
	{
		tankPositions.clear();

		LandscapeDefinitionCache &definitions = 
			context.landscapeMaps->getDefinitions();

		RandomGenerator generator;
		generator.seed(definitions.getSeed());

		std::map<unsigned int, Tank *> &tanks = 
			context.tankContainer->getPlayingTanks();
		std::map<unsigned int, Tank *>::iterator mainitor;
		for (mainitor = tanks.begin();
			mainitor != tanks.end();
			mainitor++)
		{
			Tank *tank = (*mainitor).second;

			if (!tank->getState().getSpectator() &&
				((tank->getState().getState() == TankState::sDead) ||
				tank->getState().getState() == TankState::sNormal))
			{
				FixedVector tankPos = PlacementTankPosition::placeTank(
					tank->getPlayerId(), tank->getTeam(), 
					context, generator);

				tank->getLife().setTargetPosition(tankPos);
				DeformLandscape::flattenArea(context, tankPos);

				tankPositions.push_back(tankPos);
			}
		}
	}
	else
	{
		std::list<FixedVector>::iterator itor;
		for (itor = tankPositions.begin();
			itor != tankPositions.end();
			itor++)
		{
			FixedVector &tankPos = *itor;
			DeformLandscape::flattenArea(context, tankPos);
		}
	}
}

FixedVector PlacementTankPosition::placeTank(unsigned int playerId, int team,
	ScorchedContext &context, RandomGenerator &generator)
{
	FixedVector tankPos;
	const int tankBorder = 10;

	fixed minHeight = -1000;
	fixed maxHeight = 1000;
	fixed tankCloseness = 0;
	fixed flatness = 0;
	GLImage *tankMask = 0;

	LandscapeDefnType *defn =
		context.landscapeMaps->getDefinitions().getDefn()->tankstart;
	if (defn->getType() == LandscapeDefnType::eStartHeight)
	{
		LandscapeDefnStartHeight *height = 
			(LandscapeDefnStartHeight *) defn;
		minHeight = height->heightmin;
		maxHeight = height->heightmax;
		tankCloseness = height->startcloseness;
		flatness = height->flatness;
		if (!height->startmask.empty())
		{
			tankMask = GLImageFactory::loadImage(
				getDataFile(height->startmask.c_str()));
			DIALOG_ASSERT(tankMask->getBits());
		}
	}
	else
	{
		dialogExit("ServerNewGameState",
			formatString("Failed to find tank start type \"%i\"",
			defn->getType()));
	}
    
	bool tooClose = true;
	fixed closeness = tankCloseness;
	while (tooClose)
	{
		tooClose = false;

		// Find a new position for the tank
		fixed posX = fixed (context.landscapeMaps->getDefinitions().
			getDefn()->landscapewidth - tankBorder * 2) * 
			generator.getRandFixed() + fixed(tankBorder);
		fixed posY = fixed (context.landscapeMaps->getDefinitions().
			getDefn()->landscapeheight - tankBorder * 2) * 
			generator.getRandFixed() + fixed(tankBorder);
		fixed height = context.landscapeMaps->getGroundMaps().
			getHeight(posX.asInt(), posY.asInt());
		FixedVector normal = context.landscapeMaps->getGroundMaps().
			getNormal(posX.asInt(), posY.asInt());
		tankPos = FixedVector(posX, posY, height);

		// Make sure not lower than water line
		if (tankPos[2] < minHeight ||
			tankPos[2] > maxHeight) 
		{
			tooClose = true;
			closeness -= fixed(true, 1000);
		}

		// Make sure normal is less than given
		if (normal[2] < flatness)
		{
			tooClose = true;
			closeness -= fixed(true, 1000);
		}
		
		// Make sure the mask allows the tank
		if (!tooClose)
		{
			if (tankMask)
			{
				// Find the mask position
				int maskX = (posX * fixed(tankMask->getWidth())).asInt() / 
					context.landscapeMaps->getDefinitions().getDefn()->landscapewidth;
				int maskY = (posY * fixed(tankMask->getHeight())).asInt() / 
					context.landscapeMaps->getDefinitions().getDefn()->landscapeheight;
				unsigned char *maskPos = tankMask->getBits() +
					maskX * 3 + maskY * tankMask->getWidth() * 3;
					
				if (maskPos[0] == 0 && maskPos[1] == 0 && maskPos[2] == 0)
				{
					// No tank is allowed on the black parts ot the mask
					// regardless of the team
					tooClose = true;
					closeness -= fixed(true, 1000);
				}
				else if (maskPos[0] == 255 && maskPos[1] == 255 && maskPos[2] == 255)
				{
					// All tanks are allowed on the white parts of the mask
					// regardless of the team
				}
				else if (team > 0)
				{
					// Check for team specific colors
					switch(team)
					{
					case 1:
						if (maskPos[0] != 255 || maskPos[1] != 0 || maskPos[2] != 0)
						{
							tooClose = true;
							closeness -= fixed(true, 1000);
						}
						break;
					case 2:
						if (maskPos[0] != 0 || maskPos[1] != 0 || maskPos[2] != 255)
						{
							tooClose = true;
							closeness -= fixed(true, 1000);
						}
						break;
					case 3:
						if (maskPos[0] != 0 || maskPos[1] != 255 || maskPos[2] != 0)
						{
							tooClose = true;
							closeness -= fixed(true, 1000);
						}
						break;
					case 4:
						if (maskPos[0] != 255 || maskPos[1] != 255 || maskPos[2] != 0)
						{
							tooClose = true;
							closeness -= fixed(true, 1000);
						}
						break;						
					}
				}
			}
		}

		// Make sure the tank is not too close to other tanks
		if (!tooClose)
		{
			std::map<unsigned int, Target *> targets;
			std::map<unsigned int, Target *>::iterator itor;
			context.targetSpace->getCollisionSet(tankPos, 4, targets);
			for (itor = targets.begin();
				itor != targets.end();
				itor++)
			{
				Target *thisTarget = (*itor).second;
				if (thisTarget->getPlayerId() == playerId) break;

				if ((tankPos - thisTarget->getLife().getTargetPosition()).Magnitude() < 
					MAX(closeness, thisTarget->getBorder())) 
				{
					tooClose = true;
					break;
				}
			}
		}
		
		// Ensure we never go inifinite
		if (closeness < 1) tooClose = false;
	}

	delete tankMask;

	// Get the height of the tank
	tankPos[2] = context.landscapeMaps->getGroundMaps().getInterpHeight(
		tankPos[0], tankPos[1]);

	return tankPos;
}
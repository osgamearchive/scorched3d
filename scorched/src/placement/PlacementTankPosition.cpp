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
#include <tank/TankContainer.h>
#include <tank/TankPosition.h>
#include <GLEXT/GLBitmap.h>

Vector PlacementTankPosition::placeTank(unsigned int playerId, int team,
	ScorchedContext &context, RandomGenerator &generator)
{
	Vector tankPos;
	const int tankBorder = 10;

	float minHeight = -1000.0f;
	float maxHeight = 1000.0f;
	float tankCloseness = 0.0f;
	GLBitmap *tankMask = 0;

	LandscapeDefnType *defn =
		context.landscapeMaps->getDefinitions().getDefn()->tankstart;
	if (defn->getType() == LandscapeDefnType::eStartHeight)
	{
		LandscapeDefnStartHeight *height = 
			(LandscapeDefnStartHeight *) defn;
		minHeight = height->heightmin;
		maxHeight = height->heightmax;
		tankCloseness = height->startcloseness;
		if (!height->startmask.empty())
		{
			tankMask = new GLBitmap();
			tankMask->loadFromFile(getDataFile(height->startmask.c_str()));
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
	float closeness = tankCloseness;
	while (tooClose)
	{
		tooClose = false;

		// Find a new position for the tank
		float posX = float (context.landscapeMaps->getDefinitions().
			getDefn()->landscapewidth - tankBorder * 2) * 
			generator.getRandFloat() + float(tankBorder);
		float posY = float (context.landscapeMaps->getDefinitions().
			getDefn()->landscapeheight - tankBorder * 2) * 
			generator.getRandFloat() + float(tankBorder);
		float height = context.landscapeMaps->getGroundMaps().
			getHeight((int) posX, (int) posY);
		tankPos = Vector(posX, posY, height);

		// Make sure not lower than water line
		if (tankPos[2] < minHeight ||
			tankPos[2] > maxHeight) 
		{
			tooClose = true;
			closeness -= 0.1f;
		}
		
		// Make sure the mask allows the tank
		if (!tooClose)
		{
			if (tankMask)
			{
				// Find the mask position
				int maskX = int(posX * float(tankMask->getWidth())) / 
					context.landscapeMaps->getDefinitions().getDefn()->landscapewidth;
				int maskY = int(posY * float(tankMask->getHeight())) / 
					context.landscapeMaps->getDefinitions().getDefn()->landscapeheight;
				unsigned char *maskPos = tankMask->getBits() +
					maskX * 3 + maskY * tankMask->getWidth() * 3;
					
				if (maskPos[0] == 0 && maskPos[1] == 0 && maskPos[2] == 0)
				{
					// No tank is allowed on the black parts ot the mask
					// regardless of the team
					tooClose = true;
					closeness -= 0.1f;				
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
							closeness -= 0.1f;
						}
						break;
					case 2:
						if (maskPos[0] != 0 || maskPos[1] != 0 || maskPos[2] != 255)
						{
							tooClose = true;
							closeness -= 0.1f;
						}
						break;
					case 3:
						if (maskPos[0] != 0 || maskPos[1] != 255 || maskPos[2] != 0)
						{
							tooClose = true;
							closeness -= 0.1f;
						}
						break;
					case 4:
						if (maskPos[0] != 255 || maskPos[1] != 255 || maskPos[2] != 0)
						{
							tooClose = true;
							closeness -= 0.1f;
						}
						break;						
					}
				}
			}
		}

		// Make sure the tank is not too close to other tanks
		if (!tooClose)
		{
			std::map<unsigned int, Tank *> &alltanks = 
				context.tankContainer->getAllTanks();
			std::map<unsigned int, Tank *>::iterator itor;
			for (itor = alltanks.begin();
				itor != alltanks.end();
				itor++)
			{
				Tank *thisTank = (*itor).second;
				if (thisTank->getPlayerId() == playerId) break;

				if ((tankPos - thisTank->getPosition().getTankPosition()).Magnitude() < 
					closeness) 
				{
					tooClose = true;
					closeness -= 1.0f;
					break;
				}
			}
		}
		
		// Ensure we never go inifinite
		if (closeness < 1.0f) tooClose = false;
	}

	delete tankMask;

	// Get the height of the tank
	tankPos[2] = context.landscapeMaps->getGroundMaps().getInterpHeight(
		tankPos[0], tankPos[1]);

	return tankPos;
}
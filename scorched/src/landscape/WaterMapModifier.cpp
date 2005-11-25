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

#include <landscape/WaterMapModifier.h>
#include <common/Defines.h>

void WaterMapModifier::addWaterVisibility(
	float waterHeight,
	HeightMap &hMap,
	WaterMap &wMap,
	ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Water Visibility");

	WaterMap::WaterEntry *currentEntry = wMap.heights_;
	for (int y=0; y<wMap.getMapHeight(); y++)
	{
		if (counter) counter->setNewPercentage((100.0f * float(y)) / float(wMap.getMapHeight()-1));
		for (int x=0; x<wMap.getMapWidth(); x++)
		{
			int posX = (x * (int) wMap.getMapWidthMult()) + wMap.getStartX();
			int posY = (y * (int) wMap.getMapHeightMult()) + wMap.getStartY();

			if (posX >= 0 && posY >= 0 &&
				posX < hMap.getMapWidth() && posY < hMap.getMapHeight())
			{
				int posX1 = posX >> 3;
				int posY1 = posY >> 3;
				int posX2 = MIN(hMap.getMapMinWidth(), posX1 + 1);
				int posY2 = posY1;
				int posX3 = posX1;
				int posY3 = MIN(hMap.getMapMinHeight(), posY1 + 1);
				int posX4 = MAX(0, posX1 - 1);
				int posY4 = posY1;
				int posX5 = posX1;
				int posY5 = MAX(0, posY1 - 1);

				float height = 
					MIN(
					MIN(
					MIN(
					MIN(hMap.getMinHeight(posX1, posY1),
						hMap.getMinHeight(posX2, posY2)),
						hMap.getMinHeight(posX3, posY3)),
						hMap.getMinHeight(posX4, posY4)),
						hMap.getMinHeight(posX5, posY5));
				if (height > waterHeight)
				{
					currentEntry->dontDraw = true;
					currentEntry->depth = 0.0f;
				}
				else
				{
					currentEntry->dontDraw = false;
					currentEntry->depth = 1.0f - height / waterHeight;
				}
			}
			else
			{
				currentEntry->dontDraw = false;
				currentEntry->depth = 1.0f;
			}

			currentEntry++;
		}
	}
}


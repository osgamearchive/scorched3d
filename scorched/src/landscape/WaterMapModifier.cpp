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

void WaterMapModifier::addWaterVisibility(HeightMap &hMap,
										WaterMap &wMap,
										ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Water Visibility");

	WaterMap::WaterEntry *currentEntry = wMap.heights_;
	for (int y=0; y<wMap.getWidth(); y++)
	{
		if (counter) counter->setNewPercentage((100.0f * float(y)) / float(wMap.getWidth()-1));
		for (int x=0; x<wMap.getWidth(); x++)
		{
			int posX = (x * (int) wMap.getWidthMult()) - 64;
			int posY = (y * (int) wMap.getWidthMult()) - 64;

			if (posX >= 0 && posY >= 0 &&
				posX < hMap.getWidth() && posY < hMap.getWidth())
			{
				float height = hMap.getHeight(posX, posY);
				if (height > wMap.getHeight() + 3.0f)
				{
					currentEntry->dontDraw = true;
				}
				else
				{
					currentEntry->dontDraw = false;
				}

				if (height > wMap.getHeight())
				{
					currentEntry->depth = 0.0f;
				}
				else
				{
					currentEntry->depth = 1.0f - height / wMap.getHeight();
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

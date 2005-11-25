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

#include <math.h>
#include <stdlib.h>
#include <landscape/HeightMapModifier.h>
#include <landscape/LandscapeDefn.h>
#include <common/Defines.h>
#include <GLEXT/GLBitmap.h>

void HeightMapModifier::levelSurround(HeightMap &hmap)
{
	for (int x=0; x<=hmap.getMapWidth(); x++)
	{	
		hmap.setHeight(x, 0, 0.0f);
		hmap.setHeight(x, 1, 0.0f);
		hmap.setHeight(x, hmap.getMapHeight(), 0.0f);
		hmap.setHeight(x, hmap.getMapHeight()-1, 0.0f);
	}

	for (int y=0; y<=hmap.getMapHeight(); y++)
	{
		hmap.setHeight(0, y, 0.0f);
		hmap.setHeight(1, y, 0.0f);
		hmap.setHeight(hmap.getMapWidth(), y, 0.0f);
		hmap.setHeight(hmap.getMapWidth()-1, y, 0.0f);
	}
}

void HeightMapModifier::smooth(HeightMap &hmap, 
							   LandscapeDefnHeightMapGenerate &defn,
							   ProgressCounter *counter)
{
	if (defn.landsmoothing == 0.0f) return;
	if (counter) counter->setNewOp("Smoothing");

	float *newhMap_ = new float[(hmap.getMapWidth()+1) * (hmap.getMapHeight()+1)];

	float matrix[5][5];
	for (int i=0; i<5; i++)
	{
		for (int j=0; j<5; j++)
		{
			matrix[i][j] = defn.landsmoothing; // How much smoothing is done (> is more)
			if (i==2 && j==2) matrix[i][j] = 1.0f;
		}
	}

	int x;
	for (x=0; x<=hmap.getMapWidth(); x++)
	{
		if (counter) counter->setNewPercentage((100.0f * float(x)) / float(hmap.getMapWidth()));
		for (int y=0; y<=hmap.getMapHeight(); y++)
		{
			// Total is used to catch corner cases
			float total = 0.0f;
			float inc = 0.0f;
			for (int i=0; i<5; i++)
			{
				for (int j=0; j<5; j++)
				{
					int newi = i + x - 2;
					int newj = j + y - 2;
					if (newi>=0 && newi <= hmap.getMapWidth() &&
						newj>=0 && newj <= hmap.getMapHeight())
					{
						inc += matrix[i][j] * hmap.getHeight(newi, newj);
						total += matrix[i][j];
					}
				}
			}

			newhMap_[(hmap.getMapWidth()+1) * y + x] = inc / total;
		}
	}

	float *start = newhMap_;
	for (int y=0; y<=hmap.getMapHeight(); y++)
	{
		for (x=0; x<=hmap.getMapWidth(); x++)
		{
			hmap.setHeight(x, y, *(start++));
		}
	}
	delete [] newhMap_;
}

void HeightMapModifier::scale(HeightMap &hmap, 
							  LandscapeDefnHeightMapGenerate &defn,
							  RandomGenerator &generator, 
							  ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Scaling Phase 1");

	float max = hmap.getHeight(0,0);
	float min = hmap.getHeight(0,0);

	int x;
	for (x=0; x<=hmap.getMapWidth(); x++)
	{
		if (counter) counter->setNewPercentage((100.0f * float(x)) / float(hmap.getMapWidth()));
		for (int y=0; y<=hmap.getMapHeight(); y++)
		{
			if (hmap.getHeight(x,y) > max) max = hmap.getHeight(x,y);
			if (hmap.getHeight(x,y) < min) min = hmap.getHeight(x,y);
		}
	}

	if (counter) counter->setNewOp("Scaling Phase 2");

	float realMax = ((defn.landheightmax - defn.landheightmin) * generator.getRandFloat()) + 
		defn.landheightmin;
	float per = realMax / max;

	for (x=0; x<=hmap.getMapWidth(); x++)
	{
		if (counter) counter->setNewPercentage((100.0f * float(x)) / float(hmap.getMapWidth()));
		for (int y=0; y<=hmap.getMapHeight(); y++)
		{
			float height = hmap.getHeight(x,y);
			hmap.setHeight(x,y, height * per);
		}
	}
}

void HeightMapModifier::addCirclePeak(HeightMap &hmap, Vector &start, 
									  float sizew, float sizew2, float sizeh,
									  RandomGenerator &offsetGenerator)
{
	float maxdist = MAX(sizew2, sizew);
	float sizewsq = sizew * sizew * 1.2f;
	int startx = MAX(0, int(start[0] - maxdist));
	int starty = MAX(0, int(start[1] - maxdist));
	int endx = MIN(hmap.getMapWidth(), int(start[0] + maxdist));
	int endy = MIN(hmap.getMapHeight(), int(start[1] + maxdist));

	float posX, posY;
	for (int x=startx; x<=endx; x++)
	{
		for (int y=starty; y<=endy; y++)
		{
			posX = float(x);
			posY = float(y);

			float distX = (posX - start[0]) * sizew2 / sizew;
			float distY = posY - start[1];
			float distsq = distX * distX + distY * distY;

			if (distsq < sizewsq)
			{
				float dist = sqrtf(distsq);
				float distRand = (dist / 20.0f) * 0.2f;
				if (distRand > 0.2f) distRand = 0.2f;
				dist *= offsetGenerator.getRandFloat() * distRand + 1.0f - (distRand / 2.0f);
				if (dist < sizew)
				{
					float newHeight = (float) cos(dist * PI / sizew) * sizeh / 4 + sizeh / 4;
					newHeight += hmap.getHeight(x,y);
					hmap.setHeight(x, y, newHeight);
				}
			}
		}
	}
}

void HeightMapModifier::generateTerrain(HeightMap &hmap, 
										LandscapeDefnHeightMapGenerate &defn,
										RandomGenerator &generator,
										RandomGenerator &offsetGenerator,
										ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Teraform Landscape");

	// Create a default mask that allows everything
	GLBitmap maskMap(256, 256);

	// Check if we need to load a new mask
	if (!defn.mask.empty())
	{
		const char *fileName = getDataFile(defn.mask.c_str());
		if (!maskMap.loadFromFile(fileName, false))
		{
			dialogExit("Landscape",
				"Error: Failed to find mask map \"%s\"",
				fileName);
		}
	}

	// Generate the landscape
	hmap.reset();
	float useWidthX = (float) hmap.getMapWidth(); // Use all the width
	float useWidthY = (float) hmap.getMapHeight();
	float useBorderX = (float(hmap.getMapWidth()) - useWidthX) / 2.0f;
	float useBorderY = (float(hmap.getMapHeight()) - useWidthY) / 2.0f;
	float maskMultX = float(maskMap.getWidth()) / float(hmap.getMapWidth());
	float maskMultY = float(maskMap.getHeight()) / float(hmap.getMapHeight());

	const int noItter = int((defn.landhillsmax - defn.landhillsmin) *
		generator.getRandFloat() + defn.landhillsmin);

	for (int i=0; i<noItter; i++)
	{
		if (counter) counter->setNewPercentage((100.0f * float(i)) / float(noItter));

		// Choose settings for a random hemisphere
		float sizew = (defn.landpeakwidthxmax - defn.landpeakwidthxmin) * generator.getRandFloat() 
			+ defn.landpeakwidthxmin;
		float sizew2 = (defn.landpeakwidthymax - defn.landpeakwidthymin) * generator.getRandFloat() 
			+ defn.landpeakwidthymin + sizew;
		float sizeh = ((defn.landpeakheightmax - defn.landpeakheightmin) * generator.getRandFloat() 
					   + defn.landpeakheightmin) * MAX(sizew, sizew2);

		// Choose a border around this hemisphere
		float bordersizex = 0.0f;
		float bordersizey = 0.0f;
		if (defn.levelsurround)
		{
			float bordersize = MAX(sizew, sizew2) * 1.2f;
			bordersizex = bordersize + useBorderX;
			bordersizey = bordersize + useBorderY;
		}

		// Choose a point for this hemisphere
		float sx = (generator.getRandFloat() * (float(hmap.getMapWidth()) - 
												(bordersizex * 2))) + bordersizex;
		float sy = (generator.getRandFloat() * (float(hmap.getMapHeight()) - 
												(bordersizey * 2))) + bordersizey;

		// Check if the point passes the mask
		bool ok = true;
		if (int(sx) >= 0 && int(sx) < hmap.getMapWidth() &&
			int(sy) >= 0 && int(sy) < hmap.getMapHeight())
		{
			int bx = int(sx * maskMultX);
			int by = maskMap.getWidth() - int(sy * maskMultY);
			if (bx >= 0 && bx < maskMap.getWidth() && 
				by >= 0 && by < maskMap.getHeight())
			{
			GLubyte maskPt = maskMap.getBits()[(bx * 3) + (by * maskMap.getWidth() * 3)];

			//printf("%i %i %i %s\n", maskPt, bx, by, defn.mask.c_str());
			ok = ((generator.getRandFloat() * 255.0f) < float(maskPt));
			}
		}

		if (ok)
		{
			// Actually add the hemisphere
			Vector start(sx, sy);
			addCirclePeak(hmap, start, sizew, sizew2, sizeh, offsetGenerator);
		}
	}

	scale(hmap, defn, generator, counter);
	if (defn.levelsurround) levelSurround(hmap);
	smooth(hmap, defn, counter);
	if (defn.levelsurround) levelSurround(hmap);
	hmap.generateNormals(0, hmap.getMapWidth(), 0, hmap.getMapHeight(), counter);
}


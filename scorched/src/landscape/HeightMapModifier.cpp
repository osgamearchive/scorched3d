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
#include <landscape/LandscapeDefinitions.h>
#include <common/Defines.h>
#include <GLEXT/GLBitmap.h>

void HeightMapModifier::levelSurround(HeightMap &hmap)
{
	for (int x=0; x<=hmap.getWidth(); x++)
	{
		hmap.getHeight(0, x) = 0.0f;
		hmap.getHeight(1, x) = 0.0f;
		hmap.getHeight(hmap.getWidth(), x) = 0.0f;
		hmap.getHeight(hmap.getWidth()-1, x) = 0.0f;
		hmap.getHeight(x, 0) = 0.0f;
		hmap.getHeight(x, 1) = 0.0f;
		hmap.getHeight(x, hmap.getWidth()) = 0.0f;
		hmap.getHeight(x, hmap.getWidth()-1) = 0.0f;
	}
}

void HeightMapModifier::smooth(HeightMap &hmap, 
							   LandscapeDefinition &defn,
							   ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Smoothing");

	float *newhMap_ = new float[(hmap.getWidth()+1) * (hmap.getWidth()+1)];

	float matrix[5][5];
	for (int i=0; i<5; i++)
	{
		for (int j=0; j<5; j++)
		{
			matrix[i][j] = defn.landSmoothing; // How much smoothing is done (> is more)
			if (i==2 && j==2) matrix[i][j] = 1.0f;
		}
	}

	int x;
	for (x=0; x<=hmap.getWidth(); x++)
	{
		if (counter) counter->setNewPercentage((100.0f * float(x)) / float(hmap.getWidth()));
		for (int y=0; y<=hmap.getWidth(); y++)
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
					if (newi>=0 && newi <= hmap.getWidth() &&
						newj>=0 && newj <= hmap.getWidth())
					{
						inc += matrix[i][j] * hmap.getHeight(newi, newj);
						total += matrix[i][j];
					}
				}
			}

			newhMap_[(hmap.getWidth()+1) * y + x] = inc / total;
		}
	}

	float *start = newhMap_;
	for (x=0; x<=hmap.getWidth(); x++)
	{
		for (int y=0; y<=hmap.getWidth(); y++)
		{
			hmap.getHeight(x, y) = *(start++);
		}
	}
	delete [] newhMap_;
}

void HeightMapModifier::scale(HeightMap &hmap, 
							  LandscapeDefinition &defn,
							  RandomGenerator &generator, 
							  ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Scaling Phase 1");

	float max = hmap.getHeight(0,0);
	float min = hmap.getHeight(0,0);

	int x;
	for (x=0; x<=hmap.getWidth(); x++)
	{
		if (counter) counter->setNewPercentage((100.0f * float(x)) / float(hmap.getWidth()));
		for (int y=0; y<=hmap.getWidth(); y++)
		{
			if (hmap.getHeight(x,y) > max) max = hmap.getHeight(x,y);
			if (hmap.getHeight(x,y) < min) min = hmap.getHeight(x,y);
		}
	}

	if (counter) counter->setNewOp("Scaling Phase 2");

	float realMax = ((defn.landHeightMax - defn.landHeightMin) * generator.getRandFloat()) + 
		defn.landHeightMin;
	float per = realMax / max;

	for (x=0; x<=hmap.getWidth(); x++)
	{
		if (counter) counter->setNewPercentage((100.0f * float(x)) / float(hmap.getWidth()));
		for (int y=0; y<=hmap.getWidth(); y++)
		{
			hmap.getHeight(x,y) *= per;
		}
	}
}

void HeightMapModifier::addCirclePeak(HeightMap &hmap, Vector &start, 
									  float sizew, float sizew2, float sizeh,
									  RandomGenerator &offsetGenerator)
{
	float sizewsq = sizew * sizew * 1.2f;
	float posX, posY;
	int width = hmap.getWidth();
	for (int x=0; x<=width; x++)
	{
		for (int y=0; y<=width; y++)
		{
			posX = float(x) * sizew2 / sizew;
			posY = float(y);

			float distX = posX - start[0];
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
					hmap.getHeight(x, y) += newHeight;
				}
			}
		}
	}
}

void HeightMapModifier::generateTerrain(HeightMap &hmap, 
										LandscapeDefinition &defn,
										RandomGenerator &generator,
										RandomGenerator &offsetGenerator,
										ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Teraform Landscape");

	// Create a default mask that allows everything
	GLBitmap maskMap(256, 256);
	memset(maskMap.getBits(), 255, 256 * 256 * 3);

	// Check if we need to load a new mask
	if (!defn.heightMaskFile.empty())
	{
		char fileName[256];
		sprintf(fileName, PKGDIR "data/landscapes/%s", 
			defn.heightMaskFile.c_str());
		if (!maskMap.loadFromFile(fileName , false))
		{
			dialogMessage("Landscape",
						  "Error: Failed to find mask map \"%s\"",
						  fileName);
			exit(1);
		}
	}

	// Generate the landscape
	hmap.reset();
	float useWidthX = defn.landWidthX;
	float useWidthY = defn.landWidthY;
	float useBorderX = float(hmap.getWidth() - defn.landWidthX) / 2.0f;
	float useBorderY = float(hmap.getWidth() - defn.landWidthY) / 2.0f;
	float maskMultX = float(maskMap.getWidth()) / float(hmap.getWidth());
	float maskMultY = float(maskMap.getHeight()) / float(hmap.getWidth());

	const int noItter = int((defn.landHillsMax - defn.landHillsMin) *
		generator.getRandFloat() + defn.landHillsMin);

	for (int i=0; i<noItter; i++)
	{
		if (counter) counter->setNewPercentage((100.0f * float(i)) / float(noItter));

		// Choose settings for a random hemisphere
		float sizew = (defn.landPeakWidthXMax - defn.landPeakWidthXMin) * generator.getRandFloat() 
			+ defn.landPeakWidthXMin;
		float sizew2 = (defn.landPeakWidthYMax - defn.landPeakWidthYMin) * generator.getRandFloat() 
			+ defn.landPeakWidthYMin + sizew;
		float sizeh = ((defn.landPeakHeightMax - defn.landPeakHeightMin) * generator.getRandFloat() 
					   + defn.landPeakHeightMin) * MAX(sizew, sizew2);

		// Choose a border around this hemisphere
		float bordersize = MAX(sizew, sizew2) * 1.2f;
		float bordersizex = bordersize + useBorderX;
		float bordersizey = bordersize + useBorderY;

		// Choose a point for this hemisphere
		float sx = (generator.getRandFloat() * (float(hmap.getWidth()) - 
												(bordersizex * 2))) + bordersizex;
		float sy = (generator.getRandFloat() * (float(hmap.getWidth()) - 
												(bordersizey * 2))) + bordersizey;

		// Check if the point passes the mask
		bool ok = true;
		if (int(sx) >= 0 && int(sx) < hmap.getWidth() &&
			int(sy) >= 0 && int(sy) < hmap.getWidth())
		{
			int bx = int(sx * maskMultX);
			int by = int(sy * maskMultY);
			GLubyte maskPt = maskMap.getBits()[(bx * 3) + (by * maskMap.getWidth() * 3)];

			//printf("%i %i %i %s\n", maskPt, bx, by, defn.heightMaskFile.c_str());
			ok = ((generator.getRandFloat() * 255.0f) < float(maskPt));
		}

		if (ok)
		{
			// Actually add the hemisphere
			Vector start(sx, sy);
			addCirclePeak(hmap, start, sizew, sizew2, sizeh, offsetGenerator);
		}
	}

	scale(hmap, defn, generator, counter);
	levelSurround(hmap);
	smooth(hmap, defn, counter);
	levelSurround(hmap);
	hmap.generateNormals(0, hmap.getWidth(), 0, hmap.getWidth(), counter);
}

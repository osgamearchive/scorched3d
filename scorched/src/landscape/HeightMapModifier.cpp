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


// HeightMapModifier.cpp: implementation of the HeightMapModifier class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include <landscape/HeightMapModifier.h>
#include <common/Defines.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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

void HeightMapModifier::smooth(HeightMap &hmap, ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Smoothing");

	float *newhMap_ = new float[(hmap.getWidth()+1) * (hmap.getWidth()+1)];

	float matrix[5][5];
	for (int i=0; i<5; i++)
	{
		for (int j=0; j<5; j++)
		{
			matrix[i][j] = 0.04f; // How much smoothing is done (> is more)
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

void HeightMapModifier::scale(HeightMap &hmap, int maxHeight, 
							  RandomGenerator &generator, ProgressCounter *counter)
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

	float maxH = (float) maxHeight;
	float realMax = generator.getRandFloat() * maxH + 20.0f;

	if (max > realMax)
	{
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
										int noHills, int maxHeight, 
										RandomGenerator &generator,
										RandomGenerator &offsetGenerator,
										ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Teraform Landscape");

	hmap.reset();
	float width = (float) hmap.getWidth();
	const int noItter = noHills;

	for (int i=0; i<noItter; i++)
	{
		if (counter) counter->setNewPercentage((100.0f * float(i)) / float(noItter));

		float sizew = generator.getRandFloat() * 40.0f + 10.0f;
		float sizew2 = generator.getRandFloat() * 20.0f - 10.0f + sizew;
		float bordersize = MAX(sizew, sizew2) * 1.2f;
		float sizeh = (generator.getRandFloat() * 1.0f + 0.5f) * sizew;

		// NOTE: This has been changed as I386/SPARC have different calling ordering
		// symantics (see CVS diff for changes)
		float sx = (generator.getRandFloat() * (width - (bordersize * 2))) + bordersize;
		float sy = (generator.getRandFloat() * (width - (bordersize * 2))) + bordersize;
		Vector start(sx, sy);

		addCirclePeak(hmap, start, sizew, sizew2, sizeh, offsetGenerator);
	}

	scale(hmap, maxHeight, generator, counter);
	levelSurround(hmap);
	smooth(hmap, counter);
	levelSurround(hmap);
	hmap.generateNormals(0, hmap.getWidth(), 0, hmap.getWidth(), counter);
}



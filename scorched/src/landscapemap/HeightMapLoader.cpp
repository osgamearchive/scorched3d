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

#include <landscapemap/HeightMapLoader.h>
#include <common/RandomGenerator.h>
#include <common/Defines.h>

void HeightMapLoader::saveTerrain(HeightMap &hmap, GLBitmap &bitmap)
{
	bitmap.createBlank(hmap.getMapWidth(), hmap.getMapHeight());

	unsigned char *bits = bitmap.getBits();
	for (int y=0; y<hmap.getMapHeight(); y++)
	{
		for (int x=0; x<hmap.getMapWidth(); x++)
		{
			float height = hmap.getHeight(x,y);
			bits[0] = unsigned char(height);
			bits[1] = unsigned char(height);
			bits[2] = unsigned char(height);

			bits+=3;
		}
	}
}

void HeightMapLoader::loadTerrain(HeightMap &hmap, 
	GLBitmap &bitmap, 
	bool levelSurround,
	ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Loading Landscape");
	hmap.reset();

	float dhx =  (float) bitmap.getWidth() / (float) (hmap.getMapWidth()+1);
	float dhy =  (float) bitmap.getHeight() / (float) (hmap.getMapHeight()+1);
	unsigned char *bits = bitmap.getBits();

	int bwidth = 3 * bitmap.getWidth();
	bwidth   = (bwidth + 3) & ~3;

	float hy = 0.0f;
	for (int by=0; by<=hmap.getMapHeight(); by++, hy+=dhy)
	{
		if (counter) counter->setNewPercentage((100.0f * float(by)) / float(hmap.getMapHeight()));

		int ihy = (int) hy;
		int ihy2 = ihy + 1; if (ihy2 >= bitmap.getHeight()) ihy2--;
		unsigned char *posYA = (unsigned char*) (bitmap.getBits() + (ihy  * bwidth));
		unsigned char *posYB = (unsigned char*) (bitmap.getBits() + (ihy2 * bwidth));

		float hx = 0.0f;
		for (int bx=0; bx<=hmap.getMapWidth(); bx++, hx+=dhx)
		{
			int ihx = (int) hx;
			int ihx2 = ihx + 1; if (ihx2 >= bitmap.getWidth()) ihx2--;
			unsigned char *posXA1 = posYA + ihx * 3;
			unsigned char *posXA2 = posYA + ihx2 * 3;
			unsigned char *posXB1 = posYB + ihx * 3;
			unsigned char *posXB2 = posYB + ihx2 * 3;

			float heightXA1 = float(posXA1[0]);
			float heightXA2 = float(posXA2[0]);
			float heightXB1 = float(posXB1[0]);
			float heightXB2 = float(posXB2[0]);

			float XA = ((heightXA2 - heightXA1) * (hx - ihx)) + heightXA1;
			float XB = ((heightXB2 - heightXB1) * (hx - ihx)) + heightXB1;

			float h = ((XB - XA) * (hy - ihy)) + XA;

			hmap.setHeight(bx, by, h / 2.5f);
		}
	}

	if (levelSurround) HeightMapModifier::levelSurround(hmap);
	hmap.generateNormals(0, hmap.getMapWidth(), 0, hmap.getMapHeight(), counter);
}

bool HeightMapLoader::generateTerrain(
	unsigned int seed,
	LandscapeDefnType *defn,
	HeightMap &hmap,
	bool &levelSurround,
	ProgressCounter *counter)
{
	// Do we generate or load the landscape
	if (defn->getType() == LandscapeDefnType::eHeightMapFile)
	{
		LandscapeDefnHeightMapFile *file = 
			(LandscapeDefnHeightMapFile *) defn;

		// Load the landscape
		levelSurround = file->levelsurround;
		GLBitmap bitmap;
		const char *fileName = getDataFile(file->file.c_str());
		if (!bitmap.loadFromFile(fileName, false))
		{
			dialogMessage("HeightMapLoader", formatString(
				"Error: Unable to find landscape map \"%s\"",
				fileName));
			return false;
		}
		else
		{
			HeightMapLoader::loadTerrain(
				hmap,
				bitmap, 
				file->levelsurround,
				counter);
		}
	}
	else if (defn->getType() == LandscapeDefnType::eHeightMapGenerate)
	{
		LandscapeDefnHeightMapGenerate *generate = 
			(LandscapeDefnHeightMapGenerate *) defn;

		// Seed the generator and generate the landscape
		levelSurround = generate->levelsurround;
		RandomGenerator generator;
		RandomGenerator offsetGenerator;
		generator.seed(seed);
		offsetGenerator.seed(seed);

		HeightMapModifier::generateTerrain(
			hmap, 
			*generate, 
			generator, 
			offsetGenerator, 
			counter);
	}
	else 
	{
		dialogMessage("HeightMapLoader", formatString(
			"Error: Unkown generate type %i",
			defn->getType()));
		return false;
	}
	
	return true;
}

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
#include <GLEXT/GLImageFactory.h>
#include <GLEXT/GLBitmap.h>
#include <common/RandomGenerator.h>
#include <common/Defines.h>
#include <common/Logger.h>

void HeightMapLoader::loadTerrain(HeightMap &hmap, 
	GLImage &bitmap,
	bool levelSurround,
	ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Loading Landscape");
	hmap.reset();

	fixed dhx = fixed(bitmap.getWidth()) / fixed(hmap.getMapWidth()+1);
	fixed dhy = fixed(bitmap.getHeight()) / fixed(hmap.getMapHeight()+1);
	unsigned char *bits = bitmap.getBits();

	int bwidth = 3 * bitmap.getWidth();
	bwidth   = (bwidth + 3) & ~3;

	fixed hy = fixed(0);
	for (int by=0; by<=hmap.getMapHeight(); by++, hy+=dhy)
	{
		if (counter) counter->setNewPercentage((100.0f * float(by)) / float(hmap.getMapHeight()));

		fixed bh = fixed(bitmap.getHeight());
		fixed ihy = hy.floor();
		fixed ihy2 = ihy + fixed(1); if (ihy2 >= bh) --ihy2;
		unsigned char *posYA = (unsigned char*) (bitmap.getBits() + (ihy  * fixed(bwidth)).asInt());
		unsigned char *posYB = (unsigned char*) (bitmap.getBits() + (ihy2 * fixed(bwidth)).asInt());

		fixed hx = fixed(0);
		fixed scale(true, 25000);
		for (int bx=0; bx<=hmap.getMapWidth(); bx++, hx+=dhx)
		{
			fixed ihx = fixed(hx);
			fixed bw = fixed(bitmap.getWidth());
			fixed ihx2 = ihx + fixed(1); if (ihx2 >= bw) --ihx2;

			unsigned char *posXA1 = posYA + ihx.asInt() * 3;
			unsigned char *posXA2 = posYA + ihx2.asInt() * 3;
			unsigned char *posXB1 = posYB + ihx.asInt() * 3;
			unsigned char *posXB2 = posYB + ihx2.asInt() * 3;

			fixed heightXA1 = fixed(posXA1[0]);
			fixed heightXA2 = fixed(posXA2[0]);
			fixed heightXB1 = fixed(posXB1[0]);
			fixed heightXB2 = fixed(posXB2[0]);

			fixed XA = ((heightXA2 - heightXA1) * (hx - ihx)) + heightXA1;
			fixed XB = ((heightXB2 - heightXB1) * (hx - ihx)) + heightXB1;

			fixed h = ((XB - XA) * (hy - ihy)) + XA;
			
			hmap.setHeight(bx, by, h / scale);
		}
	}

	if (levelSurround) HeightMapModifier::levelSurround(hmap);
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
		
		const char *fileName = getDataFile(file->file.c_str());
		GLImageHandle image = GLImageFactory::loadImageHandle(fileName);
		if (!image.getBits())
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
				image, 
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

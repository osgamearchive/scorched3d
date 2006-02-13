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

#include <landscape/HeightMapLoader.h>
#include <common/RandomGenerator.h>
#include <common/Defines.h>

void HeightMapLoader::saveTerrain(HeightMap &hmap, GLBitmap &bitmap)
{
	bitmap.createBlank(hmap.getMapWidth(), hmap.getMapHeight());

	GLubyte *bits = bitmap.getBits();
	for (int y=0; y<hmap.getMapHeight(); y++)
	{
		for (int x=0; x<hmap.getMapWidth(); x++)
		{
			float height = hmap.getHeight(x,y);
			bits[0] = GLubyte(height);
			bits[1] = GLubyte(height);
			bits[2] = GLubyte(height);

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

	GLfloat dhx =  (GLfloat) bitmap.getWidth() / (GLfloat) (hmap.getMapWidth()+1);
	GLfloat dhy =  (GLfloat) bitmap.getHeight() / (GLfloat) (hmap.getMapHeight()+1);
	GLubyte *bits = bitmap.getBits();

	int bwidth = 3 * bitmap.getWidth();
	bwidth   = (bwidth + 3) & ~3;

	GLfloat hy = 0.0f;
	for (int by=0; by<=hmap.getMapHeight(); by++, hy+=dhy)
	{
		if (counter) counter->setNewPercentage((100.0f * float(by)) / float(hmap.getMapHeight()));

		int ihy = (int) hy;
		int ihy2 = ihy + 1; if (ihy2 >= bitmap.getHeight()) ihy2--;
		GLubyte *posYA = (GLubyte*) (bitmap.getBits() + (ihy  * bwidth));
		GLubyte *posYB = (GLubyte*) (bitmap.getBits() + (ihy2 * bwidth));

		GLfloat hx = 0.0f;
		for (int bx=0; bx<=hmap.getMapWidth(); bx++, hx+=dhx)
		{
			int ihx = (int) hx;
			int ihx2 = ihx + 1; if (ihx2 >= bitmap.getWidth()) ihx2--;
			GLubyte *posXA1 = posYA + ihx * 3;
			GLubyte *posXA2 = posYA + ihx2 * 3;
			GLubyte *posXB1 = posYB + ihx * 3;
			GLubyte *posXB2 = posYB + ihx2 * 3;

			GLfloat heightXA1 = GLfloat(posXA1[0]);
			GLfloat heightXA2 = GLfloat(posXA2[0]);
			GLfloat heightXB1 = GLfloat(posXB1[0]);
			GLfloat heightXB2 = GLfloat(posXB2[0]);

			GLfloat XA = ((heightXA2 - heightXA1) * (hx - ihx)) + heightXA1;
			GLfloat XB = ((heightXB2 - heightXB1) * (hx - ihx)) + heightXB1;

			GLfloat h = ((XB - XA) * (hy - ihy)) + XA;

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

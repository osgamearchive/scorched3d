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

void HeightMapLoader::saveTerrain(HeightMap &hmap, GLBitmap &bitmap)
{
	bitmap.createBlank(hmap.getWidth(), hmap.getWidth());

	GLubyte *bits = bitmap.getBits();
	for (int y=0; y<hmap.getWidth(); y++)
	{
		for (int x=0; x<hmap.getWidth(); x++)
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

	GLfloat dhx =  (GLfloat) bitmap.getWidth() / (GLfloat) (hmap.getWidth()+1);
	GLfloat dhy =  (GLfloat) bitmap.getHeight() / (GLfloat) (hmap.getWidth()+1);
	GLubyte *bits = bitmap.getBits();

	int bwidth = 3 * bitmap.getWidth();
	bwidth   = (bwidth + 3) & ~3;

	GLfloat hy = 0.0f;
	for (int by=0; by<=hmap.getWidth(); by++, hy+=dhy)
	{
		if (counter) counter->setNewPercentage((100.0f * float(by)) / float(hmap.getWidth()));

		int ihy = (int) hy;
		int ihy2 = ihy + 1; if (ihy2 >= bitmap.getHeight()) ihy2--;
		GLubyte *posYA = (GLubyte*) (bitmap.getBits() + (ihy  * bwidth));
		GLubyte *posYB = (GLubyte*) (bitmap.getBits() + (ihy2 * bwidth));

		GLfloat hx = 0.0f;
		for (int bx=0; bx<=hmap.getWidth(); bx++, hx+=dhx)
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
	hmap.generateNormals(0, hmap.getWidth(), 0, hmap.getWidth(), counter);
}

bool HeightMapLoader::generateTerrain(
	unsigned int seed,
	LandscapeDefnType *defn,
	const char *defnType,
	HeightMap &hmap,
	ProgressCounter *counter)
{
	// Do we generate or load the landscape
	if (0 == strcmp(defnType, "file"))
	{
		LandscapeDefnHeightMapFile *file = 
			(LandscapeDefnHeightMapFile *) defn;

		// Load the landscape
		GLBitmap bitmap;
		const char *fileName = getDataFile(file->file.c_str());
		if (!bitmap.loadFromFile(fileName, false))
		{
			dialogMessage("HeightMapLoader",
				"Error: Unabled to find landscape map \"%s\"",
				fileName);
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
	else if (0 == strcmp(defnType, "generate"))
	{
		LandscapeDefnHeightMapGenerate *generate = 
			(LandscapeDefnHeightMapGenerate *) defn;

		// Seed the generator and generate the landscape
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
		dialogMessage("HeightMapLoader", 
			"Error: Unkown generate type %s",
			defnType);
		return false;
	}
	
	return true;
}

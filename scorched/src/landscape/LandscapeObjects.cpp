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

#include <landscape/LandscapeObjects.h>
#include <landscape/Landscape.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLBitmapModifier.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>
#include <common/Resources.h>
#include <client/ScorchedClient.h>
#include <stdio.h>

LandscapeObjects::LandscapeObjects() : pine_(false)
{

}

LandscapeObjects::~LandscapeObjects()
{
}

void LandscapeObjects::draw()
{
}

void LandscapeObjects::drawItem(float distance, GLOrderedItemRenderer::OrderedEntry &oentry)
{
	if (OptionsDisplay::instance()->getNoTrees()) return;

	LandscapeObjects::LandscapeObjectOrderedEntry &entry = 
		(LandscapeObjects::LandscapeObjectOrderedEntry &) oentry;

	static Vector point;
	point[0] = entry.posX;
	point[1] = entry.posY;
	point[2] = entry.posZ;
	if (!GLCameraFrustum::instance()->sphereInFrustum(point, 2.0f)) return;

	texture_.draw();
	glColor4f(entry.treeColor, entry.treeColor, entry.treeColor, 1.0f);

	glDepthMask(GL_TRUE);
	glPushMatrix();
		glTranslatef(entry.posX, entry.posY, entry.posZ);
		glRotatef(entry.treeRotation, 0.0f, 0.0f, 1.0f);
		glScalef(entry.treeSize, entry.treeSize, entry.treeSize);
		glCallList(entry.treeType);
	glPopMatrix();
	glDepthMask(GL_FALSE);
}

static void drawPineLevel(float centerX, float centerY,
		float width, float height, float lowheight)
{
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(centerX, centerY);
		glVertex3f(0.0f, 0.0f, height);
		for (float i=360.0f; i>=0.0f; i-=360.0f / 5.0f)
		{
			glTexCoord2f(
				centerX + (sinf(i/180.0f * PI) * 0.125f), 
				centerY + (cosf(i/180.0f * PI) * 0.125f));
			glVertex3f(
				sinf(i/180.0f * PI) * width, 
				cosf(i/180.0f * PI) * width, 
				lowheight);
		}
	glEnd();
}

static void drawPineTrunc(float width, float height, float lowheight )
{
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0.875f, 0.0f);
		glVertex3f(0.0f, 0.0f, height);
		for (float i=360.0f; i>=0.0f; i-=360.0f / 3.0f)
		{
			glTexCoord2f(0.875f + 0.125f*(i/360.0f), 0.1f);
			glVertex3f(
				sinf(i/180.0f * PI) * width, 
				cosf(i/180.0f * PI) * width, 
				lowheight);
		}
	glEnd();
}

static void drawPalmTrunc(float width, float height)
{
	bool tex = false;
	glBegin(GL_QUAD_STRIP);
		for (float i=360.0f; i>=0.0f; i-=360.0f / 5.0f)
		{
			if (tex) glTexCoord2f(0.0f, 0.0f);
			else glTexCoord2f(0.0f, 0.125f);
			glVertex3f(
				sinf(i/180.0f * PI) * (width - 0.1f), 
				cosf(i/180.0f * PI) * (width - 0.1f), 
				height);
			if (tex) glTexCoord2f(1.0f, 0.0f);
			else glTexCoord2f(1.0f, 0.125f);
			glVertex3f(
				sinf(i/180.0f * PI) * width, 
				cosf(i/180.0f * PI) * width, 
				0.0f);
			tex = !tex;
		}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, height + 0.05f);
		for (float i=360.0f; i>=0.0f; i-=360.0f / 5.0f)
		{
			if (tex) glTexCoord2f(1.0f, 0.0f);
			else glTexCoord2f(1.0f, 0.125f);
			glVertex3f(
				sinf(i/180.0f * PI) * 0.03f, 
				cosf(i/180.0f * PI) * 0.03f, 
				height - 0.01f);
			tex = !tex;
		}
	glEnd();
}

static void drawPalmLevel(float centerX, float centerY,
		float width1, float width2, float height, float height2)
{
	glBegin(GL_QUADS);
		glTexCoord2f(centerX, centerY);
		for (float i=360.0f; i>=0.0f;)
		{
			glTexCoord2f(0.0f, 0.365f);
			glVertex3f(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height);
			glTexCoord2f(0.0f, 0.488f);
			glVertex3f(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height2 + RAND * 0.1f - 0.05f);
			glTexCoord2f(0.37f, 0.488f);
			glVertex3f(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height2 + RAND * 0.1f - 0.05f);
			glTexCoord2f(0.37f,  0.365f);
			glVertex3f(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height);

			glTexCoord2f(0.37f, 0.365f);
			glVertex3f(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height);
			glTexCoord2f(0.37f, 0.488f);
			glVertex3f(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height2 + RAND * 0.1f - 0.05f);
			glTexCoord2f(0.0f, 0.488f);
			glVertex3f(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height2 + RAND * 0.1f - 0.05f);
			glTexCoord2f(0.0f, 0.365f);
			glVertex3f(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height);

			i-= (360.0f / (7.0f + 6.0f * RAND));
		}
	glEnd();
}

static inline unsigned int pointToUInt(unsigned int x, unsigned int y)
{
	return (x << 16) | (y & 0xffff);
}

void LandscapeObjects::generate(RandomGenerator &generator, ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Populating Landscape");

	// Create the tree textures and models
	if (!texture_.textureValid())
	{
		GLBitmap map( PKGDIR "data/textures/pine.bmp", 
				PKGDIR "data/textures/pinea.bmp", false);
		DIALOG_ASSERT(map.getBits());
		texture_.create(map, GL_RGBA, true);

		glNewList(treePine = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.625f, 0.875f, 0.7f, 0.3f, 0.1f);
			drawPineLevel(0.375f, 0.875f, 0.5f, 0.7f, 0.2f);
			drawPineLevel(0.125f, 0.875f, 0.3f, 1.1f, 0.5f);
		glEndList();
		glNewList(treePineSnow = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.625f, 0.625f, 0.7f, 0.3f, 0.1f);
			drawPineLevel(0.375f, 0.625f, 0.5f, 0.7f, 0.2f);
			drawPineLevel(0.125f, 0.625f, 0.3f, 1.1f, 0.5f);
		glEndList();
		glNewList(treePineBurnt = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.875f, 0.875f, 0.7f, 0.3f, 0.1f);
			drawPineLevel(0.875f, 0.875f, 0.5f, 0.7f, 0.2f);
			drawPineLevel(0.875f, 0.875f, 0.3f, 1.1f, 0.5f);
		glEndList();
		glNewList(treePalm = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f);
			drawPalmLevel(0.0f, 0.0f, 0.0f, 0.6f, 0.6f, 0.8f);
		glEndList();
		glNewList(treePalmBurnt = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f);
		glEndList();
	}

	// Start with a clean slate
	removeAllTrees();

	// TODO allow turning of this off during game
	if (OptionsDisplay::instance()->getNoTrees()) return;

	// Generate a map of where the trees should go
	unsigned char objectMap[64 * 64];
	memset(objectMap, 0, sizeof(unsigned char) * 64 * 64);

	// A few points where trees will be clustered around
	for (int i=0; i<25; i++)
	{
		// Get a random point
		int x = int(generator.getRandFloat() * 64.0f);
		int y = int(generator.getRandFloat() * 64.0f);

		// Check point is in the correct height band
		float height = 
			ScorchedClient::instance()->getLandscapeMaps().
				getHMap().getHeight(x * 4, y * 4);
		Vector &normal =
			ScorchedClient::instance()->getLandscapeMaps().
				getHMap().getNormal(x * 4, y * 4);
		if (height > 8.0f && height < 20.0f && normal[2] > 0.7f)
		{
			// Group other areas around this point that are likely to get trees
			// Do a few groups
			int n = int(generator.getRandFloat() * 10) + 5;
			for (int j=0; j<n; j++)
			{
				// Check groups is within bounds
				int newX = x + int(generator.getRandFloat() * 8.0f) - 4;
				int newY = y + int(generator.getRandFloat() * 8.0f) - 4;
				if (newX >= 0 && newX < 64 &&
					newY >= 0 && newY < 64)
				{
					Vector &normal =
						ScorchedClient::instance()->getLandscapeMaps().
						getHMap().getNormal(newX * 4, newY * 4);
					height = 
						ScorchedClient::instance()->getLandscapeMaps().
						getHMap().getHeight(newX * 4, newY *4);
					if (height > 8.0f && height < 30.0f && normal[2] > 0.7f)
					{
						objectMap[newX + 64 * newY] = 64;
					}
				}
			}

			// Add the original point
			objectMap[x + 64 * y] = 255;
		}
	}

	// Smooth the treemap
	unsigned char objectMapCopy[64 * 64];
	memcpy(objectMapCopy, objectMap, sizeof(unsigned char) * 64 * 64);
	float matrix[3][3];
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
		{
			matrix[i][j] = 0.7f; // How much smoothing is done (> is more)
			if (i==1 && j==1) matrix[i][j] = 4.0f;
		}
	}
	for (int y=0; y<64; y++)
	{
		for (int x=0; x<64; x++)
		{
			// Total is used to catch corner cases
			float total = 0.0f;
			float inc = 0.0f;
			for (int i=0; i<3; i++)
			{
				for (int j=0; j<3; j++)
				{
					int newX = i + x - 1;
					int newY = j + y - 1;
					
					if (newX >= 0 && newX < 64 &&
						newY >= 0 && newY < 64)
					{
						inc += objectMapCopy[newX + 64 * newY];
						total += matrix[i][j];
					}
				}
			}
	 
			objectMap[x + 64 * y] = GLubyte(inc/total);
		}
	}

	// Add lots of trees, more chance of adding a tree where
	// the map is stongest
	pine_ = (strcmp(Resources::stringResource("objects"), "pine") == 0);
	float snowHeight = Resources::floatResource("objectsnowheight");
	const int NoIterations = 1000000;
	for (int i=0; i<NoIterations; i++)
	{
		if (i % 1000 == 0) if (counter) 
			counter->setNewPercentage(float(i)/float(NoIterations)*100.0f);

		int x = int(RAND * 1023.0f);
		int y = int(RAND * 1023.0f);
		int nx = x / 16;
		int ny = y / 16;
		int r = objectMap[nx + 64 * ny];
		int nr = int (RAND * 512.0f);

		if (nr < r)
		{
			float lx = float(x) / 4.0f;
			float ly = float(y) / 4.0f;
			float height = 
				ScorchedClient::instance()->getLandscapeMaps().
					getHMap().getInterpHeight(lx, ly);

			if (height > 5.5f)
			{
				float mult = (float) Landscape::instance()->getMainMap().getWidth() / 256.0f;
				GLBitmapModifier::addCircle(Landscape::instance()->getMainMap(),
					lx * mult, ly * mult, 2.5f * mult, 1.0f);

				LandscapeObjectOrderedEntry *entry = new LandscapeObjectOrderedEntry;
				entry->provider_ = this;
				entry->posX = lx;
				entry->posY = ly;
				entry->posZ = height;

				if (pine_)
				{
					if (height > snowHeight + (RAND * 10.0f) - 5.0f)
					{
						entry->treeType = treePineSnow;
					}
					else
					{
						entry->treeType = treePine;
					}
				}
				else entry->treeType = treePalm;

				entry->treeRotation = RAND * 360.0f;
				entry->treeColor = RAND * 0.5f + 0.5f;
				entry->treeSize =  RAND * 2.0f + 1.0f;

				// Add the entry
				GLOrderedItemRenderer::instance()->addEntry(entry);
				unsigned int point = pointToUInt((unsigned int)lx, (unsigned int) ly);
				entries_.insert(
					std::pair<unsigned int, LandscapeObjectOrderedEntry*>(point, entry));
			}
		}
	}

	// Uncomment to see the treemap put onto the landscape
	/*GLBitmap newMap(
		Landscape::instance()->getMainMap().getWidth(),
		Landscape::instance()->getMainMap().getHeight());
	GLubyte *dest = newMap.getBits();
	for (int y=0; y<newMap.getHeight(); y++)
	{
		for (int x=0; x<newMap.getWidth(); x++)
		{
			int xn = int(float(x) / (float(newMap.getWidth() / 64.0f)));
			int yn = int(float(y) / (float(newMap.getHeight() / 64.0f)));

			dest[0] = objectMap[xn + 64 * yn];
			dest[1] = objectMap[xn + 64 * yn];
			dest[2] = objectMap[xn + 64 * yn];

			dest+=3;
		}
	}
	Landscape::instance()->getMainTexture().replace(newMap, GL_RGB, false);*/
}

void LandscapeObjects::removeAllTrees()
{
	// Clear any current trees
	std::multimap<unsigned int, LandscapeObjectOrderedEntry*>::iterator 
		itor = entries_.begin();
	std::multimap<unsigned int, LandscapeObjectOrderedEntry*>::iterator 
		enditor = entries_.end();
	for (; itor != enditor; itor++)
	{
		LandscapeObjectOrderedEntry *entry = (*itor).second;
		GLOrderedItemRenderer::instance()->rmEntry(entry);
	}	
	entries_.clear();
}

void LandscapeObjects::removeTrees(unsigned int x, unsigned int y)
{
	unsigned int point = pointToUInt((unsigned int)x, (unsigned int)y);

	std::multimap<unsigned int, LandscapeObjectOrderedEntry*>::iterator lower =
		entries_.lower_bound(point);
    std::multimap<unsigned int, LandscapeObjectOrderedEntry*>::iterator upper =
		entries_.upper_bound(point);
    std::multimap<unsigned int, LandscapeObjectOrderedEntry*>::iterator iter;
	for (iter = lower; iter != upper; iter++)
	{
		LandscapeObjectOrderedEntry *entry = (*iter).second;
		GLOrderedItemRenderer::instance()->rmEntry(entry);
	}
	entries_.erase(lower, upper);
}

void LandscapeObjects::burnTrees(unsigned int x, unsigned int y)
{
	unsigned int point = pointToUInt((unsigned int)x, (unsigned int)y);

	std::multimap<unsigned int, LandscapeObjectOrderedEntry*>::iterator lower =
		entries_.lower_bound(point);
    std::multimap<unsigned int, LandscapeObjectOrderedEntry*>::iterator upper =
		entries_.upper_bound(point);
    std::multimap<unsigned int, LandscapeObjectOrderedEntry*>::iterator iter;
	for (iter = lower; iter != upper; iter++)
	{
		LandscapeObjectOrderedEntry *entry = (*iter).second;
		if (pine_) entry->treeType = treePineBurnt;
		else entry->treeType = treePalmBurnt;
	}
}

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
#include <3dsparse/MSFile.h>
#include <GLEXT/GLVertexSetGroup.h>
#include <GLEXT/GLBitmap.h>
#include <common/Defines.h>
#include <client/ScorchedClient.h>
#include <stdio.h>

LandscapeObjects::LandscapeObjects()
{
	MSFile *newFile = new MSFile(PKGDIR "data/accessories/smallpine/smallpine.txt");
	DIALOG_ASSERT(newFile->getSuccess());

	newFile->centre();
	newFile->scale(0.1f);

	GLVertexSetGroup *arraySet = new GLVertexSetGroup();
	std::list<Model *>::iterator itor;
	for (itor = newFile->getModels().begin();
			itor != newFile->getModels().end();
			itor++)
	{
		Model *currentModel = *itor;
		GLVertexArray *array = currentModel->getArray(true);
		arraySet->addToGroup(*array);
	}
	vertexSet_ = arraySet;
}

LandscapeObjects::~LandscapeObjects()
{
}

void LandscapeObjects::draw()
{
	std::list<Entry*>::iterator itor = entries_.begin();
	std::list<Entry*>::iterator enditor = entries_.end();
	for (; itor != enditor; itor++)
	{
		Entry *entry = *itor;
		GLOrderedItemRenderer::instance()->addEntry(entry);
	}
}

void LandscapeObjects::drawItem(float distance, GLOrderedItemRenderer::OrderedEntry &oentry)
{
	LandscapeObjects::Entry &entry = (LandscapeObjects::Entry &) oentry;

	// Add shadow
	Landscape::instance()->getShadowMap().
		addCircle(entry.posX, entry.posY, 5.0f, 1.0f);

	// Draw Trees
	glDepthMask(GL_TRUE);
	glPushMatrix();
		glTranslatef(entry.posX, entry.posY, entry.posZ);
		vertexSet_->draw();
	glPopMatrix();
	glDepthMask(GL_FALSE);
}

void LandscapeObjects::generate(ProgressCounter *counter)
{
	return; // No trees for now ;)

	if (counter) counter->setNewOp("Populating Landscape");

	// Clear any current trees
	entries_.clear();

	// Generate a map of where the trees should go
	unsigned char objectMap[64 * 64];
	memset(objectMap, 0, sizeof(unsigned char) * 64 * 64);

	// A few points where trees will be clustered around
	for (int i=0; i<50; i++)
	{
		// Get a random point
		int x = int(RAND * 64.0f);
		int y = int(RAND * 64.0f);

		// Check point is in the correct height band
		float height = 
			ScorchedClient::instance()->getLandscapeMaps().
				getHMap().getHeight(x * 4, y *4);
		if (height > 10.0f && height < 20.0f)
		{
			// Group other areas around this point that are likely to get trees
			// Do a few groups
			int n = int(RAND * 10) + 5;
			for (int j=0; j<n; j++)
			{
				// Check groups is within bounds
				int newX = x + int(RAND * 8.0f) - 4;
				int newY = y + int(RAND * 8.0f) - 4;
				if (newX >= 0 && newX < 64 &&
					newY >= 0 && newY < 64)
				{
					height = 
						ScorchedClient::instance()->getLandscapeMaps().
						getHMap().getHeight(newX * 4, newY *4);
					if (height > 6.0f)
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
	const int NoIterations = 500000;
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

			Entry *entry = new Entry;
			entry->provider_ = this;
			entry->posX = lx;
			entry->posY = ly;
			entry->posZ = height + 1.0f;
			entries_.push_back(entry);
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

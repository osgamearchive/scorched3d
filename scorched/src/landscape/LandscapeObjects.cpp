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
#include <common/Defines.h>
#include <client/ScorchedClient.h>
#include <stdio.h>

LandscapeObjects::LandscapeObjects()
{
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

void LandscapeObjects::drawItem(GLOrderedItemRenderer::OrderedEntry &oentry)
{
	LandscapeObjects::Entry &entry = (LandscapeObjects::Entry &) oentry;

	GLState state(GLState::TEXTURE_OFF | GLState::BLEND_OFF);
	glPointSize(4.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
		glVertex3f(entry.posX, entry.posY, entry.posZ);
	glEnd();
	glPointSize(1.0f);
}

void LandscapeObjects::generate(ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Populating Landscape");

	return;

	unsigned char objectMap[64 * 64];
	memset(objectMap, 0, sizeof(unsigned char) * 64 * 64);
	for (int i=0; i<12; i++)
	{
		int x = int(RAND * 64.0f);
		int y = int(RAND * 64.0f);

		float height = 
			ScorchedClient::instance()->getLandscapeMaps().
				getHMap().getHeight(x * 4, y *4);
		if (height > 10.0f && height < 20.0f)
		{
			int n = int(RAND * 5);
			for (int j=0; j<n; j++)
			{
				int newX = x + int(RAND * 4.0f); - 2;
				int newY = y + int(RAND * 4.0f); - 2;
				if (newX >= 0 && newX < 64 &&
					newY >= 0 && newY < 64)
				{
					objectMap[x + 64 * y] = 128;
				}
			}
			objectMap[x + 64 * y] = 255;
		}
	}

	for (int i=0; i<256; i++)
	{
		int x = int(RAND * 1024.0f);
		int y = int(RAND * 1024.0f);
		int nx = x / 16;
		int ny = y / 16;
		int r = objectMap[nx + 64 * ny];
		int nr = int (RAND * 255.0f);

		//if (nr < r)
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
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <landscape/SkyRoof.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <client/ScorchedClient.h>
#include <engine/ScorchedContext.h>

SkyRoof::SkyRoof() : list_(0)
{
}

SkyRoof::~SkyRoof()
{
}

void SkyRoof::generate()
{
	if (list_) glDeleteLists(list_, 1);
	list_ = 0;
}
	
void SkyRoof::makeList()
{	
	HeightMap &rmap = ScorchedClient::instance()->
		getLandscapeMaps().getRMap();
	HeightMap &hmap = ScorchedClient::instance()->
		getLandscapeMaps().getHMap();

	float mult = float(hmap.getWidth()) / float(rmap.getWidth());
	float texmult = mult / 4.0f;
	
	glNewList(list_ = glGenLists(1), GL_COMPILE);
		for (int j=0; j<rmap.getWidth(); j++)
		{
			glBegin(GL_QUAD_STRIP);
			for (int i=0; i<=rmap.getWidth(); i++)
			{
				glColor3f(1.0f, 1.0f, 1.0f);
				
				glTexCoord2f(i * texmult, j * texmult);
				glVertex3f(i * mult, j * mult, 
					rmap.getHeight(i, j));
				
				glTexCoord2f(i * texmult, (j+1) * texmult);
				glVertex3f(i * mult, (j + 1) * mult, 
					rmap.getHeight(i, j + 1));
			}
			glEnd();
		}
	glEndList();
}

void SkyRoof::draw()
{
	if (!list_) makeList();
	
	GLState state(GLState::TEXTURE_OFF);
	glCallList(list_);
}


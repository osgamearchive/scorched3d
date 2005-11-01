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


#include <landscape/HeightMapRenderer.h>
#include <common/OptionsDisplay.h>
#include <GLEXT/GLState.h>

void HeightMapRenderer::drawHeightMap(HeightMap &map)
{
	/*static GLuint displayList_ = 0;
	if (!displayList_)
	{
		glNewList(displayList_ = glGenLists(1), GL_COMPILE_AND_EXECUTE);*/

		float width = map.getWidth();
		float height = map.getWidth();

		// Draw the triangles
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		int x;
		for (x=0; x<map.getWidth()-1; x++)
		{
			for (int y=0; y<map.getWidth()-1; y++)
			{
				glTexCoord2f(float(x) / width, float(y) / height);
				glNormal3fv(map.getNormal(x, y));
				glVertex3f((float)x, (float)y, map.getHeight(x, y));
				glTexCoord2f(float(x+1) / width, float(y) / height);
				glNormal3fv(map.getNormal(x + 1, y));
				glVertex3f((float)x + 1.0f, (float)y, map.getHeight(x + 1, y));
				glTexCoord2f(float(x+1) / width, float(y+1) / height);
				glNormal3fv(map.getNormal(x + 1, y + 1));
				glVertex3f((float)x + 1.0f, (float)y + 1.0f, map.getHeight(x + 1, y + 1));

				glTexCoord2f(float(x+1) / width, float(y+1) / height);
				glNormal3fv(map.getNormal(x+1, y+1));
				glVertex3f((float)x + 1.0f, (float)y + 1.0f, map.getHeight(x + 1, y + 1));
				glTexCoord2f(float(x) / width, float(y+1) / height);
				glNormal3fv(map.getNormal(x, y+1));
				glVertex3f((float)x, (float)y + 1.0f, map.getHeight(x, y + 1));
				glTexCoord2f(float(x) / width, float(y) / height);
				glNormal3fv(map.getNormal(x, y));
				glVertex3f((float)x, (float)y, map.getHeight(x, y));
			}
		}
		glEnd();

		if (OptionsDisplay::instance()->getDrawNormals())
		{
			// Draw the normals
			glColor3f(0.0f, 1.0f, 0.0f);
			glBegin(GL_LINES);
			for (x=0; x<=map.getWidth(); x++)
			{
				for (int y=0; y<=map.getWidth(); y++)
				{
					Vector &Normal = map.getNormal(x, y);
					Vector Position((float)x, (float)y, map.getHeight(x, y));

					glVertex3fv(Position);
					glVertex3fv(Position + Normal * 2.0f);
				}
			}			
			glEnd();
		}

		/*glEndList();
	}
	else
	{
		glCallList(displayList_);
	}*/
}

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


// Hemisphere.cpp: implementation of the Hemisphere class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include <GLEXT/GLState.h>
#include <common/Vector.h>
#include <landscape/Hemisphere.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Hemisphere::Hemisphere()
{

}

Hemisphere::~Hemisphere()
{

}

void Hemisphere::draw(float radius, float radius2, 
					  int heightSlices, int rotationSlices,
					  int startHeightSlice, int startRotationSlice,
					  bool inverse, bool xy, float xvalue, float yvalue)
{
	const float maxTexCoord = 1.0f;

	for (int j=startHeightSlice; j<heightSlices; j++) 
	{
		float theta1 = j * HALFPI / float(heightSlices);
		float theta2 = (j + 1) * HALFPI / float(heightSlices);

		glBegin(GL_QUAD_STRIP);
		for (int i=startRotationSlice;i<=rotationSlices;i++) 
		{
			float theta3 = i * TWOPI / float(rotationSlices);
			float c = theta3 / TWOPI * maxTexCoord;

			Vector e1, p1;
			e1[0] = float(cos(theta1) * cos(theta3));
			e1[2] = float(sin(theta1));
			e1[1] = float(cos(theta1) * sin(theta3));
			p1[0] = radius * e1[0];
			p1[2] = radius2 * e1[2];
			p1[1] = radius * e1[1];

			Vector e2, p2;
			e2[0] = float(cos(theta2) * cos(theta3));
			e2[2] = float(sin(theta2));
			e2[1] = float(cos(theta2) * sin(theta3));
			p2[0] = radius * e2[0];
			p2[2] = radius2 * e2[2];
			p2[1] = radius * e2[1];
			
			if (xy)
			{
				glTexCoord2f(((p1[0] + radius) / (2 * radius)) + xvalue, 
					((p1[1] + radius) / (2 * radius)) + yvalue);		
				glVertex3fv(p1);

				glTexCoord2f(((p2[0] + radius) / (2 * radius)) + xvalue, 
					((p2[1] + radius) / (2 * radius)) + yvalue);		
				glVertex3fv(p2);			
			}
			else
			{
				if (!inverse)
				{
					glTexCoord2f(c, float(j) / float(heightSlices));		
					glVertex3fv(p1);

					glTexCoord2f(c, float(j+1) / float(heightSlices));
					glVertex3fv(p2);
				}
				else
				{
					glTexCoord2f(c, float(j+1) / float(heightSlices));
					glVertex3fv(p2);

					glTexCoord2f(c, float(j) / float(heightSlices));		
					glVertex3fv(p1);
				}
			}
		}
		glEnd();
	}
}

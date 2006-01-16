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

#include <GLEXT/GLBitmap.h>
#include <common/Vector.h>
#include <landscape/Hemisphere.h>
#include <common/Defines.h>
#include <vector>
#include <math.h>

void Hemisphere::draw(float radius, float radius2, 
	int heightSlices, int rotationSlices,
	int startHeightSlice, int startRotationSlice,
	bool inverse)
{
	glBegin(GL_QUAD_STRIP);
	const float maxTexCoord = 1.0f;
	for (int j=startHeightSlice; j<heightSlices; j++) 
	{
		float theta1 = j * HALFPI / float(heightSlices);
		float theta2 = (j + 1) * HALFPI / float(heightSlices);

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

void Hemisphere::createXY(
	std::list<HemispherePoint> &points,
	float radius, float radius2, 
	int heightSlices, int rotationSlices,
	int startHeightSlice, int startRotationSlice,
	float tx, float ty)
{
	const float maxTexCoord = 1.0f;
	HemispherePoint point;

	for (int j=startHeightSlice; j<heightSlices; j++) 
	{
		float theta1 = sinf(HALFPI * j / float(heightSlices)) * HALFPI;
		float theta2 = sinf(HALFPI * (j + 1) / float(heightSlices)) * HALFPI;

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
			
			point.x = p1[0]; point.y = p1[1]; point.z = p1[2];
			point.tx = (p1[0] + radius) / (2 * radius) * tx;
			point.ty = (p1[1] + radius) / (2 * radius) * ty;
			points.push_back(point);
			if (i == startRotationSlice)
			{
				points.push_back(point);
				points.push_back(point);
			}

			point.x = p2[0]; point.y = p2[1]; point.z = p2[2];
			point.tx = (p2[0] + radius) / (2 * radius) * tx;
			point.ty = (p2[1] + radius) / (2 * radius) * ty;
			points.push_back(point);
		}
	}
}

void Hemisphere::createColored(std::list<HemispherePoint> &points,
	float radius, float radius2, 
	int heightSlices, int rotationSlices,
	GLBitmap &colors, Vector &sunDir, int daytime)
{
	const float maxTexCoord = 1.0f;
	HemispherePoint point;

	GLubyte *bits = colors.getBits();
	for (int j=0; j<heightSlices; j++) 
	{
		float theta1 = j * HALFPI / float(heightSlices);
		if (j<0) theta1 *= 0.75f;
		float theta2 = (j + 1) * HALFPI / float(heightSlices);

		int colorJ = MAX(j - 1, 0);
		int colorIndexA = int(float(colorJ) / float(heightSlices) * 15.0f);
		int colorIndexB = int(float(j) / float(heightSlices) * 15.0f);
		int bitmapIndexA = daytime * 3 + (16 * 3) * colorIndexA;
		int bitmapIndexB = daytime * 3 + (16 * 3) * colorIndexB;

		DIALOG_ASSERT(bitmapIndexA >= 0);
		DIALOG_ASSERT(bitmapIndexB + 2 < 
			colors.getWidth() * colors.getHeight() * colors.getComponents());

		for (int i=0;i<=rotationSlices;i++) 
		{
			float theta3 = i * TWOPI / float(rotationSlices);
			float c = theta3 / TWOPI * maxTexCoord;

			{
				Vector e1, p1;
				e1[0] = float(cos(theta1) * cos(theta3));
				e1[2] = float(sin(theta1));
				e1[1] = float(cos(theta1) * sin(theta3));
				p1[0] = radius * e1[0];
				p1[2] = radius2 * e1[2];
				p1[1] = radius * e1[1];

				float dotP = e1.Normalize().dotP(sunDir);
				dotP = (dotP + 1.0f) / 4.0f;
				float colorR = MIN(float(bits[bitmapIndexA]) / 255.0f + dotP, 1.0f);
				float colorG = MIN(float(bits[bitmapIndexA + 1]) / 255.0f + dotP, 1.0f);
				float colorB = MIN(float(bits[bitmapIndexA + 2]) / 255.0f + dotP, 1.0f);

				point.x = p1[0]; point.y = p1[1]; point.z = p1[2];
				point.tx = (p1[0] + radius) / (2 * radius);
				point.ty = (p1[1] + radius) / (2 * radius);
				point.r = colorR; point.g = colorG; point.b = colorB;
				points.push_back(point);

				if (i == 0)
				{
					points.push_back(point);
					points.push_back(point);
				}
			}

			{
				Vector e2, p2;
				e2[0] = float(cos(theta2) * cos(theta3));
				e2[2] = float(sin(theta2));
				e2[1] = float(cos(theta2) * sin(theta3));
				p2[0] = radius * e2[0];
				p2[2] = radius2 * e2[2];
				p2[1] = radius * e2[1];

				float dotP = e2.Normalize().dotP(sunDir);
				dotP = (dotP + 1.0f) / 4.0f;
				float colorR = MIN(float(bits[bitmapIndexB]) / 255.0f + dotP, 1.0f);
				float colorG = MIN(float(bits[bitmapIndexB + 1]) / 255.0f + dotP, 1.0f);
				float colorB = MIN(float(bits[bitmapIndexB + 2]) / 255.0f + dotP, 1.0f);

				point.x = p2[0]; point.y = p2[1]; point.z = p2[2];
				point.tx = (p2[0] + radius) / (2 * radius);
				point.ty = (p2[1] + radius) / (2 * radius);
				point.r = colorR; point.g = colorG; point.b = colorB;
				points.push_back(point);
			}
		}	
	}
}

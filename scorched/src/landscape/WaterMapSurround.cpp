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

#include <math.h>
#include <common/Vector.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLStateExtension.h>
#include <landscape/WaterMapSurround.h>

WaterMapSurround::WaterMapSurround(int width, int widthMult, float height) :
	width_(width), widthMult_(widthMult), height_(height)
{

}

WaterMapSurround::~WaterMapSurround()
{

}

void WaterMapSurround::draw()
{
	static GLuint listNo = 0;
	if (listNo)
	{
		glCallList(listNo);
	}
	else
	{
		glNewList(listNo = glGenLists(1), GL_COMPILE);
			generateList();
		glEndList();
	}
}

void WaterMapSurround::drawPoint(Vector &start, Vector &diff, 
								 int numberX, int maxX, 
								 int numberY, int maxY)
{
	const float centerX = 128.0f;
	const float centerY = 128.0f;

	float numberYf = (float) numberY;
	float maxYf = (float) maxY;
	float numberXf = (float) numberX;
	float maxXf = (float) maxX;

	float multiplier = float(sin((numberXf / maxXf) * 3.14));

	multiplier *= 0.3f + (0.4f * numberXf / maxXf);
	multiplier += 1.0f;
	numberYf *= multiplier;

	Vector pos = start + (diff * numberYf / maxYf);

	/*Vector centerpos(centerX, centerY, pos[2]);
	float dist = (pos - centerpos).Magnitude();
	const float startDist = (float) sqrt((128 + 64) * (128 + 64) + (128 + 64) * (128 + 64)) + 1.0f;
	if (dist > startDist)
	{
		float leftDist = dist - startDist;
		pos[2] -= 10.0f * ((float) pow(1.001, leftDist) - 1.0f);
	}*/
	//pos[2] -= height_ * (numberYf / maxYf);

	if (GLStateExtension::getTextureUnits() > 2)
	{
		GLStateExtension::glMultiTextCoord2fARB()
			(GL_TEXTURE2_ARB, pos[0] / 32.0f, pos[1] / 32.0f); 
	}

	glVertex3fv(pos);
}

void WaterMapSurround::generateList()
{
	const float largeMin = -64 -1024;
	const float largeMax = 64 + 1024 + 256;
	const float largeLen = largeMax - largeMin;
	const float largeMult = largeLen / width_;
	
	glNormal3f(0.0f, 1.0f, 0.0f);
	glColor4f(0.7f, 0.7f, 0.7f, 0.9f);

	int maxSquares = 10;
	for (int j=0; j<maxSquares; j++)
	{
		float pointX = -64;
		float pointY = -64;
		float lpointX = largeMin;
		float lpointY = largeMin;

		glBegin(GL_QUAD_STRIP);
		int i;
		for (i=0; i<width_-1; i++)
		{
			Vector start(pointX, pointY, height_);
			Vector end(lpointX, lpointY, height_);
			Vector diff = end - start;

			drawPoint(start, diff, i, width_ - 1, j, maxSquares);
			drawPoint(start, diff, i, width_ - 1, j + 1, maxSquares);

			pointX += widthMult_;
			lpointX += largeMult;
		}
		for (i=0; i<width_ - 1; i++)
		{
			Vector start(pointX, pointY, height_);
			Vector end(lpointX, lpointY, height_);
			Vector diff = end - start;

			drawPoint(start, diff, i, width_ - 1, j, maxSquares);
			drawPoint(start, diff, i, width_ - 1, j + 1, maxSquares);

			pointY += widthMult_;
			lpointY += largeMult;		
		}
		for (i=0; i<width_ - 1; i++)
		{
			Vector start(pointX, pointY, height_);
			Vector end(lpointX, lpointY, height_);
			Vector diff = end - start;

			drawPoint(start, diff, i, width_ - 1, j, maxSquares);
			drawPoint(start, diff, i, width_ - 1, j + 1, maxSquares);

			pointX -= widthMult_;
			lpointX -= largeMult;	
		}
		for (i=0; i<width_; i++)
		{
			Vector start(pointX, pointY, height_);
			Vector end(lpointX, lpointY, height_);
			Vector diff = end - start;

			drawPoint(start, diff, i, width_ - 1, j, maxSquares);
			drawPoint(start, diff, i, width_ - 1, j + 1, maxSquares);

			pointY -= widthMult_;
			lpointY -= largeMult;		
		}
		glEnd();
	}
}

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
#include <GLEXT/GLInfo.h>
#include <landscape/WaterMapSurround.h>
#include <landscape/LandscapeMaps.h>
#include <client/ScorchedClient.h>

WaterMapSurround::WaterMapSurround() :
	triangles_(0), listNo_(0)
{

}

WaterMapSurround::~WaterMapSurround()
{

}

void WaterMapSurround::generate(int mapWidth, int mapHeight,
	int startX, int startY,
	int mapWidthMult, int mapHeightMult)
{
	startX_ = (float) startX; startY_ = (float) startY;
	mapWidth_  = mapWidth;
	mapWidthMult_ = mapWidthMult;
	mapHeight_ = mapHeight;
	mapHeightMult_ = mapHeightMult;
	triangles_ = 0;

	if (listNo_ != 0)
	{
		glDeleteLists(listNo_, 1);
		listNo_ = 0;
	}
}

void WaterMapSurround::draw()
{
	if (listNo_ == 0)
	{
		glNewList(listNo_ = glGenLists(1), GL_COMPILE);
			generateList();
		glEndList();
	}
	glCallList(listNo_);

	GLInfo::addNoTriangles(triangles_);
}

void WaterMapSurround::drawPoint(Vector &start, Vector &diff, 
	int numberX, int maxX, 
	int numberY, int maxY)
{
	float numberYf = (float) numberY;
	float maxYf = (float) maxY;
	float numberXf = (float) numberX;
	float maxXf = (float) maxX;

	float multiplier = float(sin((numberXf / maxXf) * 3.14));

	multiplier *= 0.3f + (0.4f * numberXf / maxXf);
	multiplier += 1.0f;
	numberYf *= multiplier;

	Vector pos = start + (diff * numberYf / maxYf);
	if (GLStateExtension::glMultiTextCoord2fARB()) 
	{
		GLStateExtension::glMultiTextCoord2fARB()
			(GL_TEXTURE1_ARB, pos[0] / 32.0f, pos[1] / 32.0f); 
	}

	glVertex3fv(pos);
	triangles_ ++;
}

void WaterMapSurround::generateList()
{
	const float mapWidth = (float)
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapWidth();
	const float mapHeight = (float)
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapHeight();
	const float largeMinWidth = startX_ - 1024;
	const float largeMaxWidth = -startX_ + 1024 + mapWidth;
	const float largeMinHeight = startY_ - 1024;
	const float largeMaxHeight = -startY_ + 1024 + mapHeight;
	const float largeLenWidth = largeMaxWidth - largeMinWidth;
	const float largeLenHeight = largeMaxHeight - largeMinHeight;
	const float largeMultWidth = largeLenWidth / mapWidth_;
	const float largeMultHeight = largeLenHeight / mapHeight_;
	
	glNormal3f(0.0f, 1.0f, 0.0f);
	glColor4f(0.7f, 0.7f, 0.7f, 0.9f);

	int maxSquares = 10;
	for (int j=0; j<maxSquares; j++)
	{
		float pointX = startX_;
		float pointY = startY_;
		float lpointX = largeMinWidth;
		float lpointY = largeMinHeight;

		glBegin(GL_QUAD_STRIP);
		int i;
		for (i=0; i<mapWidth_-1; i++)
		{
			Vector start(pointX, pointY, 0.0f);
			Vector end(lpointX, lpointY, 0.0f);
			Vector diff = end - start;

			drawPoint(start, diff, i, mapWidth_ - 1, j, maxSquares);
			drawPoint(start, diff, i, mapWidth_ - 1, j + 1, maxSquares);

			pointX += mapWidthMult_;
			lpointX += largeMultWidth;
		}
		for (i=0; i<mapHeight_ - 1; i++)
		{
			Vector start(pointX, pointY, 0.0f);
			Vector end(lpointX, lpointY, 0.0f);
			Vector diff = end - start;

			drawPoint(start, diff, i, mapHeight_ - 1, j, maxSquares);
			drawPoint(start, diff, i, mapHeight_ - 1, j + 1, maxSquares);

			pointY += mapHeightMult_;
			lpointY += largeMultHeight;		
		}
		for (i=0; i<mapWidth_ - 1; i++)
		{
			Vector start(pointX, pointY, 0.0f);
			Vector end(lpointX, lpointY, 0.0f);
			Vector diff = end - start;

			drawPoint(start, diff, i, mapWidth_ - 1, j, maxSquares);
			drawPoint(start, diff, i, mapWidth_ - 1, j + 1, maxSquares);

			pointX -= mapWidthMult_;
			lpointX -= largeMultWidth;	
		}
		for (i=0; i<mapHeight_; i++)
		{
			Vector start(pointX, pointY, 0.0f);
			Vector end(lpointX, lpointY, 0.0f);
			Vector diff = end - start;

			drawPoint(start, diff, i, mapHeight_ - 1, j, maxSquares);
			drawPoint(start, diff, i, mapHeight_ - 1, j + 1, maxSquares);

			pointY -= mapHeightMult_;
			lpointY -= largeMultHeight;		
		}
		glEnd();
	}
}

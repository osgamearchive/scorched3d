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


// ShadowMap.cpp: implementation of the ShadowMap class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include <string.h>
#include <GLEXT/GLStateExtension.h>
#include <landscape/ShadowMap.h>
#include <common/Defines.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ShadowMap::ShadowMap() : size_(256), sizeSq_(256 *256)
{
	shadowBytes_ = new GLubyte[sizeSq_];
	memset(shadowBytes_, 255, sizeSq_);
	shadowTexture_.create(shadowBytes_, size_, size_, 1, 4, GL_LUMINANCE, false);
}

ShadowMap::~ShadowMap()
{
	delete [] shadowBytes_;
}

void ShadowMap::setTexture()
{
	// Set the shadow map texture
	shadowTexture_.draw(true);

	// Update with the contents from the stored bytes
	glTexSubImage2D(GL_TEXTURE_2D, 0, 
					0, 0, 
					size_, size_, 
					GL_LUMINANCE, GL_UNSIGNED_BYTE, 
					shadowBytes_);

	// Reset the stored bytes
	memset(shadowBytes_, 255, sizeSq_);

	// Debuging stripe
	//for (int i=0; i<size_; i++) shadowBytes_[i + i * size_] = 0;
}

void ShadowMap::addSquare(float sx, float sy, float sw, float opacity)
{
	if (!GLStateExtension::glActiveTextureARB()) return;

	const GLubyte minNum = 64;
	int decrement = int(opacity * 125.0f);
	float halfW = sw / 2.0f;

	float minX = sx - halfW;
	float minY = sy - halfW;
	float maxX = sx + halfW;
	float maxY = sy + halfW;
	/*minX /= 2.0f;
	minY /= 2.0f;
	maxX /= 2.0f;
	maxY /= 2.0f;*/

	minX = MAX(minX, 0.0f);
	minY = MAX(minY, 0.0f);
	maxX = MIN(maxX, size_ - 1.0f);
	maxY = MIN(maxY, size_ - 1.0f);

	int iSize = int(size_);
	int xStart = int(minX);
	int yStart = int(minY);
	int xWidth = int(maxX - minX);
	int yWidth = int(maxY - minY);
	int yInc = size_ - xWidth;
	if (xWidth <= 0 || yWidth <= 0) return;

	GLubyte *start = &shadowBytes_[(yStart * iSize) + xStart];
	for (int y=0; y<yWidth; y++, start += yInc)
	{
		for (int x=0; x<xWidth; x++, start++)
		{
			if (*start > decrement + minNum) (*start) -= (GLubyte) decrement;
			else (*start = minNum);
		}
	}
}

void ShadowMap::addCircle(float sx, float sy, float sw, float opacity)
{
	if (!GLStateExtension::glActiveTextureARB()) return;

	const GLubyte minNum = 64;

	int decrement = int(opacity * 125.0f);
	float halfW = sw / 2.0f;

	float minX = sx - halfW;
	float minY = sy - halfW;
	float maxX = sx + halfW;
	float maxY = sy + halfW;
	/*minX /= 2.0f;
	minY /= 2.0f;
	maxX /= 2.0f;
	maxY /= 2.0f;*/

	minX = MAX(minX, 0.0f);
	minY = MAX(minY, 0.0f);
	maxX = MIN(maxX, size_ - 1.0f);
	maxY = MIN(maxY, size_ - 1.0f);

	int iSize = int(size_);
	int xStart = int(minX);
	int yStart = int(minY);
	int xWidth = int(maxX - minX);
	int yWidth = int(maxY - minY);
	int yInc = size_ - xWidth;

	if (xWidth <= 0 || yWidth <= 0) return;
	double degMult = (1 / double(yWidth)) * 3.14;

	GLubyte *start = &shadowBytes_[(yStart * iSize) + xStart];
	for (int y=0; y<yWidth; y++, start += yInc)
	{
		double deg = double(y) * degMult;
		int realXSize = int(sin(deg) * double(xWidth));
		int halfSize = (xWidth - realXSize) / 2;

		start+=halfSize;
		int x;
		for (x=0; x<realXSize; x++, start++)
		{
			if (*start > decrement + minNum) (*start) -= (GLubyte) (decrement);
			else (*start = minNum);
		}
		start+=xWidth - (halfSize + x);
	}
}

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

#include <memory.h>
#include <math.h>
#include <common/OptionsDisplay.h>
#include <common/Resources.h>
#include <landscape/WaterMap.h>
#include <landscape/Landscape.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLCameraFrustum.h>

#define SQRT_OF_TWO_INV 0.707106781188f
#define CALC_FORCE_ENTRY(x) f = currentEntry->height - x.height; \
	currentEntry->force -= f; x.force += f;
#define CALC_FORCE_ENTRY_SQ(x) f = (currentEntry->height - x.height) \
	* SQRT_OF_TWO_INV; currentEntry->force -= f; x.force += f;

WaterMap::WaterMap(int width, int squareSize, int texSize) : 
	width_(width), squareSize_(squareSize),
	heights_(0), widthMult_(6),
	height_(5.0f),
	drawNormals_(false), drawVisiblePoints_(false),
	surround_(width, (int) widthMult_, 5.0f)
{
	// Create water entry structs
	heights_ = new WaterEntry[width_ * width_];
	memset(heights_, 0, sizeof(WaterEntry) * width_ * width_);

	// Create visiblity grid
	noVisiblesWidth_ = width_ / squareSize_;
	visible_ = new VisibleEntry[noVisiblesWidth_ * noVisiblesWidth_];
	
	// Set the visiblity grid positions and default visiblity
	for (int y=0; y<noVisiblesWidth_; y++)
	for (int x=0; x<noVisiblesWidth_; x++)
	{
		float xPoint = -64 + (widthMult_ * (x+1) * squareSize_) - (widthMult_ * squareSize_ / 2);
		float yPoint = -64 + (widthMult_ * (y+1) * squareSize_) - (widthMult_ * squareSize_ / 2);

		visible_[x + y * noVisiblesWidth_].notVisible = false;
		visible_[x + y * noVisiblesWidth_].pos = Vector(xPoint, yPoint, height_);
	}

	// Set the visiblity flags in the water entry structs to point
	// to correct visiblity grid entry the represents them
	// Normally one visiblity grid entry will represent many
	// square entries
	WaterEntry *currentEntry = heights_;
	for (int j=0; j<width_; j++)
	for (int i=0; i<width_; i++)
	{
		int x = i / squareSize_;
		int y = j / squareSize_;
		currentEntry->notVisible = &visible_[x + y * noVisiblesWidth_].notVisible;
		currentEntry->texX = float(i) / float(width_);
		currentEntry->texY = float(j) / float(width_);

		float scale = 1.0f;
		if (i<4) scale = float(i) / 4.0f;
		else if (j<4) scale = float(j) / 4.0f;
		else if (i>width_-4) scale = float(i - (width_ - 4)) / 4.0f;
		else if (j>width_-4) scale = float(j - (width_ - 4)) / 4.0f;
		currentEntry->scaling = scale;

		currentEntry++;
	}
}

WaterMap::~WaterMap()
{

}

WaterMap::WaterEntry &WaterMap::getNearestWaterPoint(Vector &point)
{
	// Not efficient and should only be called during init

	float pointY = -64.0f;
	WaterEntry *currentEntry = heights_;
	WaterEntry *result = currentEntry;
	float dist = 1000.0f;
	for (int j=0; j<width_-1; j++)
	{
		float pointX = -64.0f;
		for (int i=0; i<width_; i++)
		{
			Vector pt(pointX, pointY, currentEntry->height + height_);
			float currentDist = (pt-point).Magnitude();
			if(currentDist < dist)
			{
				result = currentEntry;
				dist = currentDist;
			}

			currentEntry++;
			pointX += widthMult_;
		}
		pointY += widthMult_;
	}

	return *result;
}

void WaterMap::draw()
{	
	glColor3f(1.0f, 1.0f, 1.0f);
	if (drawNormals_)
	{
		GLState currentState(GLState::TEXTURE_OFF);
		drawNormals();
	}

	//glPolygonOffset(10.0f, 10.0f);
	glPushAttrib(GL_TEXTURE_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_ONE, GL_SRC_COLOR);

		// Turn on the coordinate generation
		glEnable(GL_TEXTURE_GEN_S); 
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);

		if (GLStateExtension::glActiveTextureARB())
		{
			GLStateExtension::glActiveTextureARB()(GL_TEXTURE1_ARB);
			glEnable(GL_TEXTURE_2D);

			Landscape::instance()->getWaterDetail().draw(true);

			GLStateExtension::glActiveTextureARB()(GL_TEXTURE0_ARB);
		}

		if (GLStateExtension::hasCubeMap())
		{
			GLState currentState(GLState::TEXTURE_OFF | GLState::BLEND_ON | GLState::CUBEMAP_ON);

			// Set up texture coordinate generation for reflections
			glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
			glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
			glTexGenf(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);

			// Draw the water
			drawWater();
			surround_.draw();
		}
		else
		{
			GLState currentState(GLState::TEXTURE_ON | GLState::BLEND_ON);
			Landscape::instance()->getWaterTexture().draw();

			glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGenf(GL_R, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

			// Draw the water
			drawWater();
			surround_.draw();
		}

		if (GLStateExtension::glActiveTextureARB())
		{
			GLStateExtension::glActiveTextureARB()(GL_TEXTURE1_ARB);
			glDisable(GL_TEXTURE_2D);

			GLStateExtension::glActiveTextureARB()(GL_TEXTURE0_ARB);
		}

	glPopAttrib();
	//glPolygonOffset(0.0f, 0.0f);
}

void WaterMap::refreshVisiblity()
{
	float width = float(squareSize_ + squareSize_ + squareSize_ + squareSize_);

	GLCameraFrustum *frustum = GLCameraFrustum::instance();
	VisibleEntry *currentEntry = visible_;

	if (drawVisiblePoints_)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glPointSize(10.0f);
		glBegin(GL_POINTS);
	}
	for (int y=0; y<noVisiblesWidth_; y++)
	for (int x=0; x<noVisiblesWidth_; x++)
	{
		if (drawVisiblePoints_) glVertex3fv(currentEntry->pos);
		currentEntry->notVisible = !frustum->sphereInFrustum(
			currentEntry->pos,
			width);	

		currentEntry ++;
	}

	if (drawVisiblePoints_)
	{
		glEnd();
		glPointSize(1.0f);
	}
}

void WaterMap::drawNormals()
{
	float pointY = -64.0f;
	WaterEntry *currentEntry = heights_;
	glBegin(GL_LINES);
	for (int j=0; j<width_-1; j++)
	{
		float pointX = -64.0f;
			for (int i=0; i<width_; i++)
			{
				Vector pt(pointX, pointY, currentEntry->height + height_);
				glVertex3fv(pt);
				Vector normal(
					currentEntry->normal[2], 
					currentEntry->normal[0], 
					currentEntry->normal[1]);
				glVertex3fv(pt + normal * 2.0f);

				currentEntry++;
				pointX += widthMult_;
			}
		pointY += widthMult_;
	}
	glEnd();
}

void WaterMap::drawWater()
{
	refreshVisiblity();

	float pointY = -64.0f;
	WaterEntry *currentEntry = heights_;
	for (int j=0; j<width_-1; j++)
	{
		bool dontDraw = true;
		int glbegin = false;
		float pointX = -64.0f;
			for (int i=0; i<width_; i++)
			{
				WaterEntry *otherEntry = currentEntry + width_;
				bool thisDontDraw = true; 
				if (!*currentEntry->notVisible)
					thisDontDraw = currentEntry->dontDraw && otherEntry->dontDraw;

				if (thisDontDraw && dontDraw)
				{
					if (glbegin) { glEnd(); glbegin = false; }
				}
				else
				{
					if (!glbegin) { glBegin(GL_QUAD_STRIP); glbegin = true; };

					float otherHeight = otherEntry->height + height_;
					float height = currentEntry->height + height_;

					// Make the heights at the side of water
					// The actual side height
					if (i==0 || i==width_-1)
					{
						otherHeight = height_;
						height = height_;
					}
					if (j==0) height = height_;
					else if (j==width_ -2) otherHeight = height_;

					if (otherEntry->depth < 0.2f) 
					{
						otherHeight = height_;
					}
					if (currentEntry->depth < 0.2f)
					{
						height = height_;
					}

					// Make the shallows a lighter color
					float otherColor = 0.7f;
					float otherAlpha = 0.9f;
					if (otherEntry->depth < 1.0f)
					{
						otherColor = 1.0f;
						otherAlpha = otherEntry->depth * 0.4f + 0.5f;
					}
					glColor4f(otherColor, otherColor, otherColor, otherAlpha);

					// Set the normal
					glNormal3f(otherEntry->normal[0], otherEntry->normal[1], otherEntry->normal[2]);
					// Set tex coord
					if (GLStateExtension::glMultiTextCoord2fARB()) 
					{
						GLStateExtension::glMultiTextCoord2fARB()
							(GL_TEXTURE1_ARB, otherEntry->texX * 16.0f, otherEntry->texY * 16.0f); 
					}
					// Draw pt
					glVertex3f(pointX, pointY + widthMult_, otherHeight);

					// Make the shallows a lighter color
					float currentColor = 0.7f;
					float currentAlpha = 0.9f;
					if (currentEntry->depth < 1.0f)
					{
						currentColor = 1.0f;
						currentAlpha = currentEntry->depth * 0.4f + 0.5f;
					}
					glColor4f(currentColor, currentColor, currentColor, currentAlpha);

					// Set the normal
					glNormal3f(currentEntry->normal[0], currentEntry->normal[1], currentEntry->normal[2]);
					// Set tex coord
					if (GLStateExtension::glMultiTextCoord2fARB()) 
					{
						GLStateExtension::glMultiTextCoord2fARB()
							(GL_TEXTURE1_ARB, currentEntry->texX * 16.0f, currentEntry->texY * 16.0f); 
					}
					// Draw pt
					glVertex3f(pointX, pointY, height);
				}

				currentEntry++;
				pointX += widthMult_;
				dontDraw = thisDontDraw;
			}
		if (glbegin) { glEnd(); glbegin = false; }
		pointY += widthMult_;
	}
}

void WaterMap::simulate(float frameTime)
{
	if (OptionsDisplay::instance()->getNoWaterMovement()) return;

	static float waveTime = 0.0f;
	waveTime += frameTime;

	// Hack code to get around differing simulation rates
	// on different machines
	// No matter how many simulation points a second we
	// still calculate the ripples 20 times a second
	if (waveTime > 1.0f) waveTime = 1.0f;
	while (waveTime >= 0.05f)
	{
		generateSplash();
		calculateRipples();

		waveTime -= 0.05f;
	}
}

void WaterMap::generateSplash()
{
	// Generate random position withing the boundaries of the water surface
	int iWavePosX = (unsigned int) (rand() / (float) RAND_MAX * (width_ - 2));
	int iWavePosY = (unsigned int) (rand() / (float) RAND_MAX * (width_ - 2));

	addWave(iWavePosX, iWavePosY, 0.2f);
}

void WaterMap::addWave(int iWavePosX, int iWavePosY, float height)
{
	if (OptionsDisplay::instance()->getNoWaterMovement()) return;

	if (iWavePosX>0 && iWavePosY>0 &&
		iWavePosX<width_-2 && iWavePosY<width_-2)
	{
		// Place the wave
		WaterEntry *currentEntry = &getEntry(iWavePosX, iWavePosY);
		
		currentEntry->height = height;
		(currentEntry+1)->height = height;
		(currentEntry+width_)->height = height;
		(currentEntry+width_+1)->height = height;
	}
}

void WaterMap::calculateRipples()
{
	float f;
	Vector crossA, crossB;
	WaterEntry *currentEntry = heights_;
	const int widthPlusOne = width_ + 1;
	const int widthMinusOne = width_ - 1;
	int i,j;
	for (i=0; i<width_; i++)
	{
		for (j=0; j<width_; j++)
		{
			if (i > 0 && j > 0 && i < width_-1 && j <width_-1)
			{
				// Load values from array
				WaterEntry &currentEntryD = *(currentEntry - width_);
				WaterEntry &currentEntryU = *(currentEntry + width_);
				WaterEntry &currentEntryR = *(currentEntry + 1);
				WaterEntry &currentEntryRU = *(currentEntry + widthPlusOne);
				WaterEntry &currentEntryDL = *(currentEntry - widthPlusOne);
				WaterEntry &currentEntryRD = *(currentEntry + widthMinusOne);

				// Calculate new force values
				CALC_FORCE_ENTRY(currentEntryD);
				CALC_FORCE_ENTRY(currentEntryU);
				CALC_FORCE_ENTRY(currentEntryR);
				CALC_FORCE_ENTRY_SQ(currentEntryRU);
				CALC_FORCE_ENTRY_SQ(currentEntryDL);
				CALC_FORCE_ENTRY_SQ(currentEntryRD);

				// Calculate normal vectors
				crossA[0] = currentEntryU.height - currentEntry->height;
				crossA[1] = 0.0f;
				crossA[2] = widthMult_;

				crossB[0] = widthMult_;
				crossB[1] = currentEntryR.height - currentEntry->height;
				crossB[2] = 0.0f;

				// Calculate the normal
				currentEntry->normal = (crossA * crossB).Normalize();
			}
			else
			{
				// Calculate the normal
				currentEntry->normal[1] = 1.0f;
			}
	
			currentEntry++;
		}
	}

	currentEntry = heights_;
	for (j=0; j<width_; j++)
	for (i=0; i<width_; i++)
	{
		// Change the height of the water with the new force values
		currentEntry->velocity += currentEntry->force * 0.003f;
		currentEntry->height += currentEntry->velocity;
		currentEntry->height -= currentEntry->height / float(width_ * width_);
		currentEntry->height *= currentEntry->scaling;
		currentEntry->force = 0.0f;

		currentEntry++;
	}
}

void WaterMap::reset()
{
	WaterEntry *currentEntry = heights_;
	for (int j=0; j<width_; j++)
	for (int i=0; i<width_; i++)
	{
		currentEntry->height = 0.0f;
		currentEntry->velocity = 0.0f;
		currentEntry->force = 0.0f;

		currentEntry++;
	}
}

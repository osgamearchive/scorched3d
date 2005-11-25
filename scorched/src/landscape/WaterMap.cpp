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
#include <landscape/WaterMap.h>
#include <landscape/Landscape.h>
#include <GLEXT/GLInfo.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLCameraFrustum.h>

#define SQRT_OF_TWO_INV 0.707106781188f
#define CALC_FORCE_ENTRY(x) f = currentEntry->height - x.height; \
	currentEntry->force -= f; x.force += f;
#define CALC_FORCE_ENTRY_SQ(x) f = (currentEntry->height - x.height) \
	* SQRT_OF_TWO_INV; currentEntry->force -= f; x.force += f;

WaterMap::WaterMap() :
	heights_(0), visible_(0),
	drawNormals_(false), drawVisiblePoints_(false),
	waterTexture_(0)
{
}

void WaterMap::generate(int width, int height, 
	int startX, int startY,
	int mapMult, int squareWidth)
{
	startX_ = (float) startX; startY_ = (float) startY;
	visibilitySquareSize_ = squareWidth;
	mapWidth_ = width; mapHeight_ = height;
	mapWidthMult_ = mapMult; mapHeightMult_ = mapMult;
	surround_.generate(mapWidth_, mapHeight_, 
		startX, startY,
		mapWidthMult_, mapHeightMult_);

	// Create water entry structs
	delete [] heights_;
	heights_ = new WaterEntry[mapWidth_ * mapHeight_];
	memset(heights_, 0, sizeof(WaterEntry) * mapWidth_ * mapHeight_);

	// Create visiblity grid
	delete [] visible_;
	noVisiblesWidth_ = mapWidth_ / visibilitySquareSize_;
	noVisiblesHeight_ = mapHeight_ / visibilitySquareSize_;
	visible_ = new VisibleEntry[noVisiblesWidth_ * noVisiblesHeight_];
	
	// Set the visiblity grid positions and default visiblity
	for (int y=0; y<noVisiblesHeight_; y++)
	for (int x=0; x<noVisiblesWidth_; x++)
	{
		float xPoint = float(startX_ + (mapWidthMult_ * (x+1) * visibilitySquareSize_) - 
			(mapWidthMult_ * visibilitySquareSize_ / 2));
		float yPoint = float(startY_ + (mapHeightMult_ * (y+1) * visibilitySquareSize_) - 
			(mapHeightMult_ * visibilitySquareSize_ / 2));

		visible_[x + y * noVisiblesWidth_].notVisible = false;
		visible_[x + y * noVisiblesWidth_].pos = Vector(xPoint, yPoint, 0.0f);
	}

	// Set the visiblity flags in the water entry structs to point
	// to correct visiblity grid entry the represents them
	// Normally one visiblity grid entry will represent many
	// square entries
	WaterEntry *currentEntry = heights_;
	for (int j=0; j<mapHeight_; j++)
	for (int i=0; i<mapWidth_; i++)
	{
		int x = i / visibilitySquareSize_;
		int y = j / visibilitySquareSize_;
		currentEntry->notVisible = &visible_[x + y * noVisiblesWidth_].
			notVisible;
		currentEntry->texX = float(i) / float(mapWidth_);
		currentEntry->texY = float(j) / float(mapHeight_);

		float scale = 1.0f;
		if (i<4) scale = float(i) / 4.0f;
		else if (j<4) scale = float(j) / 4.0f;
		else if (i>mapWidth_ - 4) scale = float(i - (mapWidth_ - 4)) / 4.0f;
		else if (j>mapHeight_ - 4) scale = float(j - (mapHeight_ - 4)) / 4.0f;
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

	float pointY = float(startY_);
	WaterEntry *currentEntry = heights_;
	WaterEntry *result = currentEntry;
	float dist = 1000.0f;
	for (int j=0; j<mapHeight_-1; j++)
	{
		float pointX = float(startX_);
		for (int i=0; i<mapWidth_; i++)
		{
			Vector pt(pointX, pointY, currentEntry->height);
			float currentDist = (pt-point).Magnitude();
			if(currentDist < dist)
			{
				result = currentEntry;
				dist = currentDist;
			}

			currentEntry++;
			pointX += mapWidthMult_;
		}
		pointY += mapHeightMult_;
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

	glPushAttrib(GL_TEXTURE_BIT);
		if (GLStateExtension::glActiveTextureARB())
		{
			GLStateExtension::glActiveTextureARB()(GL_TEXTURE1_ARB);
			glEnable(GL_TEXTURE_2D);

			waterDetail_.draw(true);

			GLStateExtension::glActiveTextureARB()(GL_TEXTURE0_ARB);
		}

		if (OptionsDisplay::instance()->getNoGLSphereMap())
		{
			GLState currentState(GLState::TEXTURE_ON | GLState::BLEND_ON);
			waterTexture_->draw();

			// Set up texture coordinate generation for reflections
			float PlaneS[] = { 0.0f, 1.0f / 20.0f, 0.0f, 0.0f };
			float PlaneT[] = { 1.0f / 20.0f, 0.0f, 0.0f, 0.0f };

			glEnable(GL_TEXTURE_GEN_S); 
			glEnable(GL_TEXTURE_GEN_T);
			glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
			glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
			glTexGenfv(GL_S, GL_OBJECT_PLANE, PlaneS);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, PlaneT);

			// Draw the water
			surround_.draw();
			drawWater();
		}
		else if (GLStateExtension::hasCubeMap())
		{
			GLState currentState(GLState::TEXTURE_OFF | GLState::BLEND_ON | GLState::CUBEMAP_ON);

			// Set up texture coordinate generation for reflections
			glEnable(GL_TEXTURE_GEN_S); 
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);
			glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
			glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
			glTexGenf(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);

			// Draw the water
			surround_.draw();
			drawWater();
		}
		else
		{
			GLState currentState(GLState::TEXTURE_ON | GLState::BLEND_ON);
			waterTexture_->draw();

			glEnable(GL_TEXTURE_GEN_S); 
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);
			glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGenf(GL_R, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

			// Draw the water
			surround_.draw();
			drawWater();
		}

		if (GLStateExtension::glActiveTextureARB())
		{
			GLStateExtension::glActiveTextureARB()(GL_TEXTURE1_ARB);
			glDisable(GL_TEXTURE_2D);

			GLStateExtension::glActiveTextureARB()(GL_TEXTURE0_ARB);
		}

	glPopAttrib();
}

void WaterMap::refreshVisiblity()
{
	float width = float(visibilitySquareSize_ + visibilitySquareSize_ + 
		visibilitySquareSize_ + visibilitySquareSize_);

	GLCameraFrustum *frustum = GLCameraFrustum::instance();
	VisibleEntry *currentEntry = visible_;

	if (drawVisiblePoints_)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glPointSize(10.0f);
		glBegin(GL_POINTS);
	}
	for (int y=0; y<noVisiblesHeight_; y++)
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
	float pointY = float(startY_);
	WaterEntry *currentEntry = heights_;
	glBegin(GL_LINES);
	for (int j=0; j<mapHeight_ -1; j++)
	{
		float pointX = float(startX_);
			for (int i=0; i<mapWidth_; i++)
			{
				Vector pt(pointX, pointY, currentEntry->height);
				glVertex3fv(pt);
				Vector normal(
					currentEntry->normal[2], 
					currentEntry->normal[0], 
					currentEntry->normal[1]);
				glVertex3fv(pt + normal * 2.0f);

				currentEntry++;
				pointX += mapWidthMult_;
			}
		pointY += mapHeightMult_;
	}
	glEnd();
}

void WaterMap::drawWater()
{
	if (OptionsDisplay::instance()->getNoWaterMovement())
	{
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glColor4f(0.7f, 0.7f, 0.7f, 0.9f);

			if (GLStateExtension::glMultiTextCoord2fARB()) 
			{
				GLStateExtension::glMultiTextCoord2fARB()
					(GL_TEXTURE1_ARB, 0.0f, 0.0f); 
			}
			glVertex3f(startX_, startY_, 0.0f);
			if (GLStateExtension::glMultiTextCoord2fARB()) 
			{
				GLStateExtension::glMultiTextCoord2fARB()
					(GL_TEXTURE1_ARB, 32.0f, 0.0f); 
			}
			glVertex3f(startX_ + mapWidthMult_ * (mapWidth_ - 1), 
				startY_, 0.0f);
			if (GLStateExtension::glMultiTextCoord2fARB()) 
			{
				GLStateExtension::glMultiTextCoord2fARB()
					(GL_TEXTURE1_ARB, 32.0f, 32.0f); 
			}
			glVertex3f(startX_ + mapWidthMult_ * (mapWidth_ - 1), 
				startY_ + mapHeightMult_ * (mapHeight_ - 1), 0.0f);
				if (GLStateExtension::glMultiTextCoord2fARB()) 
			{
				GLStateExtension::glMultiTextCoord2fARB()
					(GL_TEXTURE1_ARB, 0.0f, 32.0f); 
			}
			glVertex3f(startX_, startY_ + mapHeightMult_ * (mapHeight_ - 1), 0.0f);
		glEnd();
		return;
	}

	refreshVisiblity();

	float pointY = startY_;
	WaterEntry *currentEntry = heights_;
	for (int j=0; j<mapHeight_-1; j++)
	{
		bool dontDraw = true;
		int glbegin = false;
		float pointX = startX_;
			for (int i=0; i<mapWidth_; i++)
			{
				WaterEntry *otherEntry = currentEntry + mapWidth_;
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

					float otherHeight = otherEntry->height + 0.0f;
					float height = currentEntry->height + 0.0f;

					// Make the heights at the side of water
					// The actual side height
					if (i==0 || i==mapWidth_-1)
					{
						otherHeight = 0.0f;
						height = 0.0f;
					}
					if (j==0) height = 0.0f;
					else if (j==mapHeight_-2) otherHeight = 0.0f;

					if (otherEntry->depth < 0.2f) 
					{
						otherHeight = 0.0f;
					}
					if (currentEntry->depth < 0.2f)
					{
						height = 0.0f;
					}

					// Make the shallows a lighter color
					float otherColor = 0.7f;
					float otherAlpha = 0.9f;
					if (otherEntry->depth < 1.0f)
					{
						otherColor = 1.0f;
						otherAlpha = otherEntry->depth * 0.5f + 0.4f;
					}
					glColor4f(otherColor, otherColor, otherColor, otherAlpha);

					// Set the normal
					glNormal3f(otherEntry->normal[0], 
						otherEntry->normal[1], 
						otherEntry->normal[2]);
					// Set tex coord
					if (GLStateExtension::glMultiTextCoord2fARB()) 
					{
						GLStateExtension::glMultiTextCoord2fARB()
							(GL_TEXTURE1_ARB, otherEntry->texX * 16.0f, otherEntry->texY * 16.0f); 
					}
					// Draw pt
					glVertex3f(pointX, pointY + mapHeightMult_, otherHeight);

					// Make the shallows a lighter color
					float currentColor = 0.7f;
					float currentAlpha = 0.9f;
					if (currentEntry->depth < 1.0f)
					{
						currentColor = 1.0f;
						currentAlpha = currentEntry->depth * 0.5f + 0.4f;
					}
					glColor4f(currentColor, currentColor, currentColor, currentAlpha);

					// Set the normal
					glNormal3f(currentEntry->normal[0], 
						currentEntry->normal[1], 
						currentEntry->normal[2]);
					// Set tex coord
					if (GLStateExtension::glMultiTextCoord2fARB()) 
					{
						GLStateExtension::glMultiTextCoord2fARB()
							(GL_TEXTURE1_ARB, currentEntry->texX * 16.0f, currentEntry->texY * 16.0f); 
					}
					// Draw pt
					glVertex3f(pointX, pointY, height);

					GLInfo::addNoTriangles(2);
				}

				currentEntry++;
				pointX += mapWidthMult_;
				dontDraw = thisDontDraw;
			}
		if (glbegin) { glEnd(); glbegin = false; }
		pointY += mapHeightMult_;
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
	int iWavePosX = (unsigned int) (rand() / (float) RAND_MAX * (mapWidth_ - 2));
	int iWavePosY = (unsigned int) (rand() / (float) RAND_MAX * (mapHeight_ - 2));

	addWave(iWavePosX, iWavePosY, 0.2f);
}

void WaterMap::addWave(int iWavePosX, int iWavePosY, float height)
{
	if (OptionsDisplay::instance()->getNoWaterMovement()) return;

	if (iWavePosX>0 && iWavePosY>0 &&
		iWavePosX<mapWidth_-2 && iWavePosY<mapHeight_-2)
	{
		// Place the wave
		WaterEntry *currentEntry = &getEntry(iWavePosX, iWavePosY);
		
		currentEntry->height = height;
		(currentEntry+1)->height = height;
		(currentEntry+mapWidth_)->height = height;
		(currentEntry+mapWidth_+1)->height = height;
	}
}

void WaterMap::calculateRipples()
{
	float f;
	Vector crossA, crossB;
	WaterEntry *currentEntry = heights_;
	const int widthPlusOne = mapWidth_ + 1;
	const int widthMinusOne = mapWidth_ - 1;
	int i,j;
	for (j=0; j<mapHeight_; j++)
	{
		for (i=0; i<mapWidth_; i++)
		{
			if (!currentEntry->dontDraw &&
				i > 0 && j > 0 && 
				i < mapWidth_-1 && j < mapHeight_-1)
			{
				// Load values from array
				WaterEntry &currentEntryD = *(currentEntry - mapWidth_);
				WaterEntry &currentEntryU = *(currentEntry + mapWidth_);
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
				crossA[2] = (float) mapHeightMult_;

				crossB[0] = (float) mapWidthMult_;
				crossB[1] = currentEntryR.height - currentEntry->height;
				crossB[2] = 0.0f;

				// Calculate the normal
				crossA *= crossB;
				crossA.StoreNormalize();
				currentEntry->normal = crossA;
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
	for (j=0; j<mapHeight_; j++)
	for (i=0; i<mapWidth_; i++)
	{
		if (!currentEntry->dontDraw)
		{
			// Change the height of the water with the new force values
			currentEntry->velocity += currentEntry->force * 0.003f;
			currentEntry->height += currentEntry->velocity;
			currentEntry->height -= currentEntry->height / float(mapWidth_ * mapHeight_);
			currentEntry->height *= currentEntry->scaling;
			currentEntry->force = 0.0f;
		}

		currentEntry++;
	}
}

void WaterMap::reset()
{
	WaterEntry *currentEntry = heights_;
	for (int j=0; j<mapHeight_; j++)
	for (int i=0; i<mapWidth_; i++)
	{
		currentEntry->height = 0.0f;
		currentEntry->velocity = 0.0f;
		currentEntry->force = 0.0f;

		currentEntry++;
	}
}

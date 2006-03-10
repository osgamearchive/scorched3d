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

#include <landscape/LandscapeObjectsEntryTree.h>
#include <common/OptionsDisplay.h>
#include <common/Defines.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLInfo.h>
#include <math.h>

GLTexture LandscapeObjectsEntryTree::texture_;
GLuint LandscapeObjectsEntryTree::treePine = 0;
GLuint LandscapeObjectsEntryTree::treePineBurnt = 0;
GLuint LandscapeObjectsEntryTree::treePineSnow = 0;
GLuint LandscapeObjectsEntryTree::treePineSmall = 0;
GLuint LandscapeObjectsEntryTree::treePineBurntSmall = 0;
GLuint LandscapeObjectsEntryTree::treePineSnowSmall = 0;
GLuint LandscapeObjectsEntryTree::treePalm = 0;
GLuint LandscapeObjectsEntryTree::treePalmBurnt = 0;
GLuint LandscapeObjectsEntryTree::treePalmSmall = 0;
GLuint LandscapeObjectsEntryTree::treePalmBurntSmall = 0;

static void drawPineLevel(float centerX, float centerY,
		float width, float height, float lowheight)
{
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(centerX, centerY);
		glVertex3f(0.0f, 0.0f, height);
		for (float i=360.0f; i>=0.0f; i-=360.0f / 5.0f)
		{
			glTexCoord2f(
				centerX + (sinf(i/180.0f * PI) * 0.125f), 
				centerY + (cosf(i/180.0f * PI) * 0.125f));
			glVertex3f(
				sinf(i/180.0f * PI) * width, 
				cosf(i/180.0f * PI) * width, 
				lowheight);
		}
	glEnd();
}

static void drawPineTrunc(float width, float height, float lowheight )
{
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0.875f, 0.0f);
		glVertex3f(0.0f, 0.0f, height);
		for (float i=360.0f; i>=0.0f; i-=360.0f / 3.0f)
		{
			glTexCoord2f(0.875f + 0.125f*(i/360.0f), 0.1f);
			glVertex3f(
				sinf(i/180.0f * PI) * width, 
				cosf(i/180.0f * PI) * width, 
				lowheight);
		}
	glEnd();
}

static void drawPalmTrunc(float width, float height, float count)
{
	bool tex = false;
	glBegin(GL_QUAD_STRIP);
		for (float i=360.0f; i>=0.0f; i-=360.0f / count)
		{
			if (tex) glTexCoord2f(0.0f, 0.0f);
			else glTexCoord2f(0.0f, 0.125f);
			glVertex3f(
				sinf(i/180.0f * PI) * (width - 0.1f), 
				cosf(i/180.0f * PI) * (width - 0.1f), 
				height);
			if (tex) glTexCoord2f(1.0f, 0.0f);
			else glTexCoord2f(1.0f, 0.125f);
			glVertex3f(
				sinf(i/180.0f * PI) * width, 
				cosf(i/180.0f * PI) * width, 
				0.0f);
			tex = !tex;
		}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, height + 0.05f);
		for (float i=360.0f; i>=0.0f; i-=360.0f / count)
		{
			if (tex) glTexCoord2f(1.0f, 0.0f);
			else glTexCoord2f(1.0f, 0.125f);
			glVertex3f(
				sinf(i/180.0f * PI) * 0.03f, 
				cosf(i/180.0f * PI) * 0.03f, 
				height - 0.01f);
			tex = !tex;
		}
	glEnd();
}

static void drawPalmLevel(float centerX, float centerY,
		float width1, float w2, float height, float height2,
		float count)
{
	glBegin(GL_QUADS);
		glTexCoord2f(centerX, centerY);
		for (float i=360.0f; i>=0.0f;)
		{
			float diff = 0.5f * RAND - 0.25f;
			float width2 = (w2 * RAND * 0.3f) + (0.7f * w2);
			glTexCoord2f(0.0f, 0.365f);
			glVertex3f(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height);
			glTexCoord2f(0.0f, 0.488f);
			glVertex3f(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height2);
			glTexCoord2f(0.37f, 0.488f);
			glVertex3f(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height2 + diff);
			glTexCoord2f(0.37f,  0.365f);
			glVertex3f(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height + diff);

			glTexCoord2f(0.37f, 0.365f);
			glVertex3f(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height + diff);
			glTexCoord2f(0.37f, 0.488f);
			glVertex3f(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height2 + diff);
			glTexCoord2f(0.0f, 0.488f);
			glVertex3f(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height2);
			glTexCoord2f(0.0f, 0.365f);
			glVertex3f(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height);

			i-= (360.0f / (count + (count-1) * RAND));
		}
	glEnd();
}

void LandscapeObjectsEntryTree::render(float distance)
{
	// Create the tree textures and models
	if (!texture_.textureValid())
	{
		std::string file1 = getDataFile("data/textures/pine.bmp");
		std::string file2 = getDataFile("data/textures/pinea.bmp");
		GLBitmap map(file1.c_str(), file2.c_str(), false);
		DIALOG_ASSERT(map.getBits());
		texture_.create(map, GL_RGBA, true);

		glNewList(treePine = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.625f, 0.875f, 0.7f, 0.3f, 0.1f);
			drawPineLevel(0.375f, 0.875f, 0.5f, 0.7f, 0.2f);
			drawPineLevel(0.125f, 0.875f, 0.3f, 1.1f, 0.5f);
		glEndList();
		glNewList(treePineSmall = glGenLists(1), GL_COMPILE);
			drawPineLevel(0.625f, 0.875f, 0.7f, 1.1f, 0.1f);
		glEndList();
		glNewList(treePineSnow = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.625f, 0.625f, 0.7f, 0.3f, 0.1f);
			drawPineLevel(0.375f, 0.625f, 0.5f, 0.7f, 0.2f);
			drawPineLevel(0.125f, 0.625f, 0.3f, 1.1f, 0.5f);
		glEndList();
		glNewList(treePineSnowSmall = glGenLists(1), GL_COMPILE);
			drawPineLevel(0.625f, 0.625f, 0.7f, 1.1f, 0.1f);
		glEndList();
		glNewList(treePineBurnt = glGenLists(1), GL_COMPILE);
			glColor3f(0.3f, 0.3f, 0.3f);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.875f, 0.875f, 0.7f, 0.3f, 0.1f);
			drawPineLevel(0.875f, 0.875f, 0.5f, 0.7f, 0.2f);
			drawPineLevel(0.875f, 0.875f, 0.3f, 1.1f, 0.5f);
		glEndList();
		glNewList(treePineBurntSmall = glGenLists(1), GL_COMPILE);
			glColor3f(0.3f, 0.3f, 0.3f);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.875f, 0.875f, 0.7f, 1.1f, 0.1f);
		glEndList();
		glNewList(treePalm = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f, 5.0f);
			drawPalmLevel(0.0f, 0.0f, 0.0f, 0.6f, 0.6f, 0.8f, 7.0f);
		glEndList();
		glNewList(treePalmSmall = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f, 3.0f);
			drawPalmLevel(0.0f, 0.0f, 0.0f, 0.6f, 0.6f, 0.8f, 3.0f);
		glEndList();
		glNewList(treePalmBurnt = glGenLists(1), GL_COMPILE);
			glColor3f(0.3f, 0.3f, 0.3f);
			drawPalmTrunc(0.07f, 0.7f, 5.0f);
		glEndList();
		glNewList(treePalmBurntSmall = glGenLists(1), GL_COMPILE);
			glColor3f(0.3f, 0.3f, 0.3f);
			drawPalmTrunc(0.07f, 0.7f, 3.0f);
		glEndList();
	}

	if (OptionsDisplay::instance()->getNoTrees()) return;

	static Vector point;
	point[0] = posX;
	point[1] = posY;
	point[2] = posZ;
	if (!GLCameraFrustum::instance()->sphereInFrustum(
		point, 2.0f)) return;

	GLuint treeType = treePalm;
	GLuint smallTreeType = treePalmSmall;
	if (pine)
	{
		if (burnt)
		{
			treeType = treePineBurnt;
			smallTreeType = treePineBurntSmall;
		}
		else if (snow)
		{
			treeType = treePineSnow;
			smallTreeType = treePineSnowSmall;
		}
		else
		{
			treeType = treePine;
			smallTreeType = treePineSmall;
		}
	}
	else if (burnt)
	{
		treeType = treePalmBurnt;
		smallTreeType = treePalmBurntSmall;
	}

	texture_.draw();
	glPushMatrix();
		glTranslatef(posX, posY, posZ);
		glRotatef(rotation, 0.0f, 0.0f, 1.0f);
		glScalef(size, size, size);

		glColor4f(color, color, color, 1.0f);
		if (OptionsDisplay::instance()->getLowTreeDetail() || distance > 16000)
		{
			glCallList(smallTreeType);
			GLInfo::addNoTriangles(20);
		}
		else 
		{
			glCallList(treeType);
			GLInfo::addNoTriangles(10);
		}
	glPopMatrix();
}

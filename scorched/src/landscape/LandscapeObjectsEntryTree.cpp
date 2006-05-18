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

GLTexture LandscapeObjectsEntryTree::pineTextureA_;
GLTexture LandscapeObjectsEntryTree::palmTextureA_;
GLuint LandscapeObjectsEntryTree::treePineList = 0;
GLuint LandscapeObjectsEntryTree::treePineSmallList = 0;
GLuint LandscapeObjectsEntryTree::treePineBurntList = 0;
GLuint LandscapeObjectsEntryTree::treePineBurntSmallList = 0;
GLuint LandscapeObjectsEntryTree::treePineSnowList = 0;
GLuint LandscapeObjectsEntryTree::treePineSnowSmallList = 0;
GLuint LandscapeObjectsEntryTree::treePineYellowList = 0;
GLuint LandscapeObjectsEntryTree::treePineYellowSmallList = 0;
GLuint LandscapeObjectsEntryTree::treePineLightList = 0;
GLuint LandscapeObjectsEntryTree::treePineLightSmallList = 0;

GLuint LandscapeObjectsEntryTree::treePalmList = 0;
GLuint LandscapeObjectsEntryTree::treePalmSmallList = 0;
GLuint LandscapeObjectsEntryTree::treePalmBurntList = 0;
GLuint LandscapeObjectsEntryTree::treePalmBurntSmallList = 0;

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

static void drawPalmLevel(
		float width1, float w2, float height, float height2,
		float count, float texX, float texY)
{
	glBegin(GL_QUADS);
		for (float i=360.0f; i>=0.0f;)
		{
			float diff = 0.5f * RAND - 0.25f;
			float width2 = (w2 * RAND * 0.3f) + (0.7f * w2);
			glTexCoord2f(texX, texY);
			glVertex3f(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height);
			glTexCoord2f(texX, texY + 0.123f);
			glVertex3f(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height2);
			glTexCoord2f(texX + 0.37f, texY + 0.123f);
			glVertex3f(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height2 + diff);
			glTexCoord2f(texX + 0.37f, texY);
			glVertex3f(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height + diff);

			glTexCoord2f(texX + 0.37f, texY);
			glVertex3f(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height + diff);
			glTexCoord2f(texX + 0.37f, texY + 0.123f);
			glVertex3f(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height2 + diff);
			glTexCoord2f(texX, texY + 0.123f);
			glVertex3f(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height2);
			glTexCoord2f(texX, texY);
			glVertex3f(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height);

			i-= (360.0f / (count + (count-1) * RAND));
		}
	glEnd();
}

void LandscapeObjectsEntryTree::setup(const char *type, bool snow)
{
	normalType_ = (snow?ePineSnow:ePineNormal);
	burntType_ = ePineBurnt;
	
	if (0 == strcmp(type, "burntpine"))
	{
		normalType_ = ePineBurnt;
	}
	else if (0 == strcmp(type, "yellowpine"))
	{
		normalType_ = (snow?ePineSnow:ePineYellow);
	}
	else if (0 == strcmp(type, "lightpine"))
	{
		normalType_ = (snow?ePineSnow:ePineLight);
	}
	else if (0 == strcmp(type, "palm"))
	{
		normalType_ = ePalmNormal;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "burntpalm"))
	{
		normalType_ = ePalmBurnt;
		burntType_ = ePalmBurnt;
	}
}

void LandscapeObjectsEntryTree::render(float distance)
{
	// Create the tree textures and models
	if (!pineTextureA_.textureValid())
	{
		{
			std::string file1 = getDataFile("data/textures/pine2.bmp");
			std::string file2 = getDataFile("data/textures/pine2a.bmp");
			GLBitmap map(file1.c_str(), file2.c_str(), false);
			DIALOG_ASSERT(map.getBits());
			pineTextureA_.create(map, GL_RGBA, true);
		}
		{
			std::string file1 = getDataFile("data/textures/pine.bmp");
			std::string file2 = getDataFile("data/textures/pinea.bmp");
			GLBitmap map(file1.c_str(), file2.c_str(), false);
			DIALOG_ASSERT(map.getBits());
			palmTextureA_.create(map, GL_RGBA, true);
		}

		glNewList(treePineList = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.625f, 0.875f, 0.7f, 0.3f, 0.1f);
			drawPineLevel(0.375f, 0.875f, 0.5f, 0.7f, 0.2f);
			drawPineLevel(0.125f, 0.875f, 0.3f, 1.1f, 0.5f);
		glEndList();
		glNewList(treePineSmallList = glGenLists(1), GL_COMPILE);
			drawPineLevel(0.625f, 0.875f, 0.7f, 1.1f, 0.1f);
		glEndList();
		glNewList(treePineSnowList = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.625f, 0.625f, 0.7f, 0.3f, 0.1f);
			drawPineLevel(0.375f, 0.625f, 0.5f, 0.7f, 0.2f);
			drawPineLevel(0.125f, 0.625f, 0.3f, 1.1f, 0.5f);
		glEndList();
		glNewList(treePineSnowSmallList = glGenLists(1), GL_COMPILE);
			drawPineLevel(0.625f, 0.625f, 0.7f, 1.1f, 0.1f);
		glEndList();
		glNewList(treePineYellowList = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.625f, 0.375f, 0.7f, 0.3f, 0.1f);
			drawPineLevel(0.375f, 0.375f, 0.5f, 0.7f, 0.2f);
			drawPineLevel(0.125f, 0.375f, 0.3f, 1.1f, 0.5f);
		glEndList();
		glNewList(treePineYellowSmallList = glGenLists(1), GL_COMPILE);
			drawPineLevel(0.625f, 0.375f, 0.7f, 1.1f, 0.1f);
		glEndList();
		glNewList(treePineLightList = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.625f, 0.125f, 0.7f, 0.3f, 0.1f);
			drawPineLevel(0.375f, 0.125f, 0.5f, 0.7f, 0.2f);
			drawPineLevel(0.125f, 0.125f, 0.3f, 1.1f, 0.5f);
		glEndList();
		glNewList(treePineLightSmallList = glGenLists(1), GL_COMPILE);
			drawPineLevel(0.625f, 0.125f, 0.7f, 1.1f, 0.1f);
		glEndList();
		glNewList(treePineBurntList = glGenLists(1), GL_COMPILE);
			glColor3f(0.3f, 0.3f, 0.3f);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.875f, 0.875f, 0.7f, 0.3f, 0.1f);
			drawPineLevel(0.875f, 0.875f, 0.5f, 0.7f, 0.2f);
			drawPineLevel(0.875f, 0.875f, 0.3f, 1.1f, 0.5f);
		glEndList();
		glNewList(treePineBurntSmallList = glGenLists(1), GL_COMPILE);
			glColor3f(0.3f, 0.3f, 0.3f);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.875f, 0.875f, 0.7f, 1.1f, 0.1f);
		glEndList();
		glNewList(treePalmList = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f, 5.0f);
			drawPalmLevel(0.0f, 0.6f, 0.6f, 0.8f, 7.0f, 0.0f, 0.365f);
		glEndList();
		glNewList(treePalmSmallList = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f, 3.0f);
			drawPalmLevel(0.0f, 0.6f, 0.6f, 0.8f, 3.0f, 0.0f, 0.365f);
		glEndList();
		glNewList(treePalmBurntList = glGenLists(1), GL_COMPILE);
			glColor3f(0.3f, 0.3f, 0.3f);
			drawPalmTrunc(0.07f, 0.7f, 5.0f);
		glEndList();
		glNewList(treePalmBurntSmallList = glGenLists(1), GL_COMPILE);
			glColor3f(0.3f, 0.3f, 0.3f);
			drawPalmTrunc(0.07f, 0.7f, 3.0f);
		glEndList();
	}

	if (OptionsDisplay::instance()->getNoTrees()) return;

	if (!GLCameraFrustum::instance()->sphereInFrustum(
		position, boundingsize.Max() / 2.0f)) return;

	GLuint treeList = 0;
	GLuint smallTreeList = 0;
	switch(burnt?burntType_:normalType_)
	{
	case ePineNormal:
		pineTextureA_.draw();
		treeList = treePineList;
		smallTreeList = treePineSmallList;
		break;
	case ePineBurnt:
		pineTextureA_.draw();
		treeList = treePineBurntList;
		smallTreeList = treePineBurntSmallList;
		break;
	case ePineSnow:
		pineTextureA_.draw();
		treeList = treePineSnowList;
		smallTreeList = treePineSnowSmallList;
		break;
	case ePineYellow:
		pineTextureA_.draw();
		treeList = treePineYellowList;
		smallTreeList = treePineYellowSmallList;
		break;
	case ePineLight:
		pineTextureA_.draw();
		treeList = treePineLightList;
		smallTreeList = treePineLightSmallList;
		break;
	case ePalmNormal:
		palmTextureA_.draw();
		treeList = treePalmList;
		smallTreeList = treePalmSmallList;
		break;
	case ePalmBurnt:
		palmTextureA_.draw();
		treeList = treePalmBurntList;
		smallTreeList = treePalmBurntSmallList;
		break;
	};

	DIALOG_ASSERT(treeList && smallTreeList);

	glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		glRotatef(rotation, 0.0f, 0.0f, 1.0f);
		glScalef(modelscale, modelscale, modelscale);

		glColor4f(color, color, color, 1.0f);
		if (OptionsDisplay::instance()->getLowTreeDetail() || distance > 16000)
		{
			glCallList(smallTreeList);
			GLInfo::addNoTriangles(20);
		}
		else 
		{
			glCallList(treeList);
			GLInfo::addNoTriangles(10);
		}
	glPopMatrix();
}

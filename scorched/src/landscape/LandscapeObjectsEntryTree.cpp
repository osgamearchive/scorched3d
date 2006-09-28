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
#include <common/Vector4.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLInfo.h>
#include <math.h>

// Pine
GLTexture LandscapeObjectsEntryTree::pineTextureA_;
GLTexture LandscapeObjectsEntryTree::pineTextureB_;
GLuint LandscapeObjectsEntryTree::treePineList = 0;
GLuint LandscapeObjectsEntryTree::treePineSmallList = 0;
GLuint LandscapeObjectsEntryTree::treePineSnowList = 0;
GLuint LandscapeObjectsEntryTree::treePineSnowSmallList = 0;
GLuint LandscapeObjectsEntryTree::treePine2List = 0;
GLuint LandscapeObjectsEntryTree::treePine2SmallList = 0;
GLuint LandscapeObjectsEntryTree::treePine2SnowList = 0;
GLuint LandscapeObjectsEntryTree::treePine2SnowSmallList = 0;
GLuint LandscapeObjectsEntryTree::treePine3List = 0;
GLuint LandscapeObjectsEntryTree::treePine3SmallList = 0;
GLuint LandscapeObjectsEntryTree::treePine3SnowList = 0;
GLuint LandscapeObjectsEntryTree::treePine3SnowSmallList = 0;
GLuint LandscapeObjectsEntryTree::treePine4List = 0;
GLuint LandscapeObjectsEntryTree::treePine4SmallList = 0;
GLuint LandscapeObjectsEntryTree::treePine4SnowList = 0;
GLuint LandscapeObjectsEntryTree::treePine4SnowSmallList = 0;
GLuint LandscapeObjectsEntryTree::treePineBurntList = 0;
GLuint LandscapeObjectsEntryTree::treePineBurntSmallList = 0;
GLuint LandscapeObjectsEntryTree::treePineYellowList = 0;
GLuint LandscapeObjectsEntryTree::treePineYellowSmallList = 0;
GLuint LandscapeObjectsEntryTree::treePineLightList = 0;
GLuint LandscapeObjectsEntryTree::treePineLightSmallList = 0;

// Palm
GLTexture LandscapeObjectsEntryTree::palmTextureA_;
GLTexture LandscapeObjectsEntryTree::palmTextureB_;
GLuint LandscapeObjectsEntryTree::treePalmList = 0;
GLuint LandscapeObjectsEntryTree::treePalmSmallList = 0;
GLuint LandscapeObjectsEntryTree::treePalm2List = 0;
GLuint LandscapeObjectsEntryTree::treePalm2SmallList = 0;
GLuint LandscapeObjectsEntryTree::treePalm3List = 0;
GLuint LandscapeObjectsEntryTree::treePalm3SmallList = 0;
GLuint LandscapeObjectsEntryTree::treePalm4List = 0;
GLuint LandscapeObjectsEntryTree::treePalm4SmallList = 0;
GLuint LandscapeObjectsEntryTree::treePalmBList = 0;
GLuint LandscapeObjectsEntryTree::treePalmB2List = 0;
GLuint LandscapeObjectsEntryTree::treePalmB3List = 0;
GLuint LandscapeObjectsEntryTree::treePalmB4List = 0;
GLuint LandscapeObjectsEntryTree::treePalmB5List = 0;
GLuint LandscapeObjectsEntryTree::treePalmB6List = 0;
GLuint LandscapeObjectsEntryTree::treePalmB7List = 0;
GLuint LandscapeObjectsEntryTree::treePalmBurntList = 0;
GLuint LandscapeObjectsEntryTree::treePalmBurntSmallList = 0;

// Oak
GLTexture LandscapeObjectsEntryTree::oakTextureA_;
GLuint LandscapeObjectsEntryTree::treeOakList = 0;
GLuint LandscapeObjectsEntryTree::treeOakSmallList = 0;
GLuint LandscapeObjectsEntryTree::treeOak2List = 0;
GLuint LandscapeObjectsEntryTree::treeOak2SmallList = 0;
GLuint LandscapeObjectsEntryTree::treeOak3List = 0;
GLuint LandscapeObjectsEntryTree::treeOak3SmallList = 0;
GLuint LandscapeObjectsEntryTree::treeOak4List = 0;
GLuint LandscapeObjectsEntryTree::treeOak4SmallList = 0;

static void drawPineLevel(float texX, float texY,
		float width, float height, float lowheight, 
		float texWidth = 0.125f, float count = 5.0f, bool doubleSide = false,
		float angOffset = 0.0f)
{
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(texX, texY);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, height);
		for (float i=360.0f; i>=0.0f; i-=360.0f / count)
		{
			float diff = RAND * 40.0f - 20.0f;
			glTexCoord2f(
				texX + (sinf((i + angOffset)/180.0f * PI) * texWidth), 
				texY + (cosf((i + angOffset)/180.0f * PI) * texWidth));
			glNormal3f(
				sinf((i+diff)/180.0f * PI) * (height - lowheight),
				cosf((i+diff)/180.0f * PI) * (height - lowheight),
				width);
			glVertex3f(
				sinf(i/180.0f * PI) * width, 
				cosf(i/180.0f * PI) * width, 
				lowheight);
		}
	glEnd();

	if (doubleSide)
	{
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(texX, texY);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, height);
		for (float i=0.0f; i<=360.0f; i+=360.0f / count)
		{
			float diff = RAND * 40.0f - 20.0f;
			glTexCoord2f(
				texX + (sinf((i + angOffset)/180.0f * PI) * texWidth), 
				texY + (cosf((i + angOffset)/180.0f * PI) * texWidth));
			glNormal3f(
				-sinf((i+diff)/180.0f * PI) * (height - lowheight),
				-cosf((i+diff)/180.0f * PI) * (height - lowheight),
				-width);
			glVertex3f(
				sinf(i/180.0f * PI) * width, 
				cosf(i/180.0f * PI) * width, 
				lowheight);
		}
	glEnd();
	}
}

static void drawPineTrunc(float width, float height, float lowheight,
	float x = 0.875f, float y = 0.0f, float w = 0.125f, float h = 0.1f,
	float steps = 3.0f)
{
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(x , y);
		glNormal3f(0.0, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, height);
		for (float i=360.0f; i>=0.0f; i-=360.0f / steps)
		{
			glTexCoord2f(x + w*(float(int(i*5.0f)%360)/360.0f), y + h);
			glNormal3f(
				sinf(i/180.0f * PI),
				cosf(i/180.0f * PI),
				0.0f);
			glVertex3f(
				sinf(i/180.0f * PI) * width, 
				cosf(i/180.0f * PI) * width, 
				lowheight);
		}
	glEnd();
}

static void drawPalmTrunc(float width, float height, float count,
	float x = 0.0f, float y = 0.0f,
	float w = 1.0f, float h = 0.125f)
{
	bool tex = false;
	glBegin(GL_QUAD_STRIP);
		for (float i=360.0f; i>=0.0f; i-=360.0f / count)
		{
			glNormal3f(
				sinf(i/180.0f * PI), 
				cosf(i/180.0f * PI), 
				0.0f);
			if (tex) glTexCoord2f(x, y);
			else glTexCoord2f(x, y + h);
			glVertex3f(
				sinf(i/180.0f * PI) * (width - 0.1f), 
				cosf(i/180.0f * PI) * (width - 0.1f), 
				height);
			if (tex) glTexCoord2f(x + w, y);
			else glTexCoord2f(x + w, y + h);
			glVertex3f(
				sinf(i/180.0f * PI) * width, 
				cosf(i/180.0f * PI) * width, 
				0.0f);
			tex = !tex;
		}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(x + w, y);
		glNormal3f(0.0, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, height + 0.05f);
		for (float i=360.0f; i>=0.0f; i-=360.0f / count)
		{
			if (tex) glTexCoord2f(x + w, y);
			else glTexCoord2f(x + w, y + h);
			glNormal3f(
				sinf(i/180.0f * PI), 
				cosf(i/180.0f * PI), 
				0.0f);
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
			
			Vector A1(
				sinf((i-15.0f)/180.0f * PI) * width1, 
				cosf((i-15.0f)/180.0f * PI) * width1, 
				height);
			Vector A2(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height2);
			Vector A3(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height2 + diff);
			Vector A4(
				sinf((i-15.0f)/180.0f * PI) * width2, 
				cosf((i-15.0f)/180.0f * PI) * width2, 
				height + diff);

			Vector AN = ((A3 - A4) * (A3 - A2));
			glNormal3fv(AN);
			glTexCoord2f(texX, texY);
			glVertex3fv(A1);
			glTexCoord2f(texX, texY + 0.123f);
			glVertex3fv(A2);
			glTexCoord2f(texX + 0.37f, texY + 0.123f);
			glVertex3fv(A3);
			glTexCoord2f(texX + 0.37f, texY);
			glVertex3fv(A4);

			Vector B1(
				sinf((i-15.0f)/180.0f * PI) * width2, 
				cosf((i-15.0f)/180.0f * PI) * width2, 
				height + diff);
			Vector B2(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height2 + diff);
			Vector B3(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height2);
			Vector B4(
				sinf((i-15.0f)/180.0f * PI) * width1, 
				cosf((i-15.0f)/180.0f * PI) * width1, 
				height);
			Vector BN = ((B1 - B3) * (B2 - B1));
			glNormal3fv(BN);
			glTexCoord2f(texX + 0.37f, texY);
			glVertex3fv(B1);
			glTexCoord2f(texX + 0.37f, texY + 0.123f);
			glVertex3fv(B2);
			glTexCoord2f(texX, texY + 0.123f);
			glVertex3fv(B3);
			glTexCoord2f(texX, texY);
			glVertex3fv(B4);

			Vector C1(
				sinf((i+15.0f)/180.0f * PI) * width2, 
				cosf((i+15.0f)/180.0f * PI) * width2, 
				height + diff);
			Vector C2(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height2 + diff);
			Vector C3(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height2);
			Vector C4(
				sinf((i+15.0f)/180.0f * PI) * width1, 
				cosf((i+15.0f)/180.0f * PI) * width1, 
				height);

			Vector CN = ((C2 - C1) * (C3 - C2));
			glNormal3fv(CN);
			glTexCoord2f(texX + 0.37f, texY);
			glVertex3fv(C1);
			glTexCoord2f(texX + 0.37f, texY + 0.123f);
			glVertex3fv(C2);
			glTexCoord2f(texX, texY + 0.123f);
			glVertex3fv(C3);
			glTexCoord2f(texX, texY);
			glVertex3fv(C4);

			Vector D1(
				sinf((i+15.0f)/180.0f * PI) * width1, 
				cosf((i+15.0f)/180.0f * PI) * width1, 
				height);
			Vector D2(
				sinf(i/180.0f * PI) * width1, 
				cosf(i/180.0f * PI) * width1, 
				height2);
			Vector D3(
				sinf(i/180.0f * PI) * width2, 
				cosf(i/180.0f * PI) * width2, 
				height2 + diff);
			Vector D4(
				sinf((i+15.0f)/180.0f * PI) * width2, 
				cosf((i+15.0f)/180.0f * PI) * width2, 
				height + diff);

			Vector DN = ((D4 - D3) * (D2 - D4));
			glNormal3fv(DN);
			glTexCoord2f(texX, texY);
			glVertex3fv(D1);
			glTexCoord2f(texX, texY + 0.123f);
			glVertex3fv(D2);
			glTexCoord2f(texX + 0.37f, texY + 0.123f);
			glVertex3fv(D3);
			glTexCoord2f(texX + 0.37f, texY);
			glVertex3fv(D4);

			i-= (360.0f / (count + (count-1) * RAND));
		}
	glEnd();

}

bool LandscapeObjectsEntryTree::setup(const char *type, bool snow)
{
	if (0 == strcmp(type, "pine"))
	{
		normalType_ = (snow?ePineSnow:ePineNormal);
		burntType_ = ePineBurnt;
	}
	else if (0 == strcmp(type, "pine2"))
	{
		normalType_ = (snow?ePine2Snow:ePine2);
		burntType_ = ePineBurnt;
	}
	else if (0 == strcmp(type, "pine3"))
	{
		normalType_ = (snow?ePine3Snow:ePine3);;
		burntType_ = ePineBurnt;
	}
	else if (0 == strcmp(type, "pine4"))
	{
		normalType_ = (snow?ePine4Snow:ePine4);;
		burntType_ = ePineBurnt;
	}
	else if (0 == strcmp(type, "burntpine"))
	{
		normalType_ = ePineBurnt;
		burntType_ = ePineBurnt;
	}
	else if (0 == strcmp(type, "yellowpine"))
	{
		normalType_ = (snow?ePineSnow:ePineYellow);
		burntType_ = ePineBurnt;
	}
	else if (0 == strcmp(type, "lightpine"))
	{
		normalType_ = (snow?ePineSnow:ePineLight);
		burntType_ = ePineBurnt;
	}
	else if (0 == strcmp(type, "palm"))
	{
		normalType_ = ePalmNormal;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palm2"))
	{
		normalType_ = ePalm2;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palm3"))
	{
		normalType_ = ePalm3;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palm4"))
	{
		normalType_ = ePalm4;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb"))
	{
		normalType_ = ePalmB;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb2"))
	{
		normalType_ = ePalmB2;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb3"))
	{
		normalType_ = ePalmB3;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb4"))
	{
		normalType_ = ePalmB4;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb5"))
	{
		normalType_ = ePalmB5;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb6"))
	{
		normalType_ = ePalmB6;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb7"))
	{
		normalType_ = ePalmB7;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "burntpalm"))
	{
		normalType_ = ePalmBurnt;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "oak"))
	{
		normalType_ = eOak;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "oak2"))
	{
		normalType_ = eOak2;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "oak3"))
	{
		normalType_ = eOak3;
		burntType_ = ePalmBurnt;
	}
	else if (0 == strcmp(type, "oak4"))
	{
		normalType_ = eOak4;
		burntType_ = ePalmBurnt;
	}
	else return false;

	return true;
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
			std::string file1 = getDataFile("data/textures/pine3.bmp");
			std::string file2 = getDataFile("data/textures/pine3a.bmp");
			GLBitmap map(file1.c_str(), file2.c_str(), false);
			DIALOG_ASSERT(map.getBits());
			pineTextureB_.create(map, GL_RGBA, true);
		}
		{
			std::string file1 = getDataFile("data/textures/pine.bmp");
			std::string file2 = getDataFile("data/textures/pinea.bmp");
			GLBitmap map(file1.c_str(), file2.c_str(), false);
			DIALOG_ASSERT(map.getBits());
			palmTextureA_.create(map, GL_RGBA, true);
		}
		{
			std::string file1 = getDataFile("data/textures/palm2.bmp");
			std::string file2 = getDataFile("data/textures/palm2a.bmp");
			GLBitmap map(file1.c_str(), file2.c_str(), false);
			DIALOG_ASSERT(map.getBits());
			palmTextureB_.create(map, GL_RGBA, true);
		}
		{
			std::string file1 = getDataFile("data/textures/oak.bmp");
			std::string file2 = getDataFile("data/textures/oaka.bmp");
			GLBitmap map(file1.c_str(), file2.c_str(), false);
			DIALOG_ASSERT(map.getBits());
			oakTextureA_.create(map, GL_RGBA, true);
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

		glNewList(treePine2List = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.18f, 0.836f, 0.7f, 0.3f, 0.1f, 0.18f, 8.0f);
			drawPineLevel(0.18f, 0.836f, 0.5f, 0.7f, 0.2f, 0.18f, 8.0f);
			drawPineLevel(0.18f, 0.836f, 0.3f, 1.1f, 0.5f, 0.18f, 8.0f);
		glEndList();
		glNewList(treePine2SmallList = glGenLists(1), GL_COMPILE);
			drawPineLevel(0.18f, 0.836f, 0.7f, 1.1f, 0.1f, 0.18f, 8.0f);
		glEndList();

		glNewList(treePine3List = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.18f, 0.5f, 0.7f, 0.3f, 0.1f, 0.18f, 8.0f);
			drawPineLevel(0.18f, 0.5f, 0.5f, 0.7f, 0.2f, 0.18f, 8.0f);
			drawPineLevel(0.18f, 0.5f, 0.3f, 1.1f, 0.5f, 0.18f, 8.0f);
		glEndList();
		glNewList(treePine3SmallList = glGenLists(1), GL_COMPILE);
			drawPineLevel(0.18f, 0.5f, 0.7f, 1.1f, 0.1f, 0.18f, 8.0f);
		glEndList();

		glNewList(treePine4List = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.18f, 0.172f, 0.7f, 0.3f, 0.1f, 0.18f, 8.0f);
			drawPineLevel(0.18f, 0.172f, 0.5f, 0.7f, 0.2f, 0.18f, 8.0f);
			drawPineLevel(0.18f, 0.172f, 0.3f, 1.1f, 0.5f, 0.18f, 8.0f);
		glEndList();
		glNewList(treePine4SmallList = glGenLists(1), GL_COMPILE);
			drawPineLevel(0.18f, 0.172f, 0.7f, 1.1f, 0.1f, 0.18f, 8.0f);
		glEndList();

		glNewList(treePine2SnowList = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.52f, 0.836f, 0.7f, 0.3f, 0.1f, 0.18f, 8.0f);
			drawPineLevel(0.52f, 0.836f, 0.5f, 0.7f, 0.2f, 0.18f, 8.0f);
			drawPineLevel(0.52f, 0.836f, 0.3f, 1.1f, 0.5f, 0.18f, 8.0f);
		glEndList();
		glNewList(treePine2SnowSmallList = glGenLists(1), GL_COMPILE);
			drawPineLevel(0.52f, 0.836f, 0.7f, 1.1f, 0.1f, 0.18f, 8.0f);
		glEndList();

		glNewList(treePine3SnowList = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.52f, 0.5f, 0.7f, 0.3f, 0.1f, 0.18f, 8.0f);
			drawPineLevel(0.52f, 0.5f, 0.5f, 0.7f, 0.2f, 0.18f, 8.0f);
			drawPineLevel(0.52f, 0.5f, 0.3f, 1.1f, 0.5f, 0.18f, 8.0f);
		glEndList();
		glNewList(treePine3SnowSmallList = glGenLists(1), GL_COMPILE);
			drawPineLevel(0.52f, 0.5f, 0.7f, 1.1f, 0.1f, 0.18f, 8.0f);
		glEndList();

		glNewList(treePine4SnowList = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.1f, 1.1f, 0.0f);
			drawPineLevel(0.52f, 0.172f, 0.7f, 0.3f, 0.1f, 0.18f, 8.0f);
			drawPineLevel(0.52f, 0.172f, 0.5f, 0.7f, 0.2f, 0.18f, 8.0f);
			drawPineLevel(0.52f, 0.172f, 0.3f, 1.1f, 0.5f, 0.18f, 8.0f);
		glEndList();
		glNewList(treePine4SnowSmallList = glGenLists(1), GL_COMPILE);
			drawPineLevel(0.52f, 0.172f, 0.7f, 1.1f, 0.1f, 0.18f, 8.0f);
		glEndList();

		glNewList(treePalmList = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f, 5.0f);
			drawPalmLevel(0.0f, 0.6f, 0.6f, 0.8f, 7.0f, 0.0f, 0.365f);
		glEndList();
		glNewList(treePalmSmallList = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f, 3.0f);
			drawPalmLevel(0.0f, 0.6f, 0.6f, 0.8f, 3.0f, 0.0f, 0.365f);
		glEndList();

		glNewList(treePalm2List = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f, 5.0f);
			drawPalmLevel(0.0f, 0.6f, 0.6f, 0.8f, 7.0f, 0.39f, 0.365f);
		glEndList();
		glNewList(treePalm2SmallList = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f, 3.0f);
			drawPalmLevel(0.0f, 0.6f, 0.6f, 0.8f, 3.0f, 0.39f, 0.365f);
		glEndList();

		glNewList(treePalm3List = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f, 5.0f);
			drawPalmLevel(0.0f, 0.6f, 0.6f, 0.8f, 7.0f, 0.0f, 0.25f);
		glEndList();
		glNewList(treePalm3SmallList = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f, 3.0f);
			drawPalmLevel(0.0f, 0.6f, 0.6f, 0.8f, 3.0f, 0.0f, 0.25f);
		glEndList();

		glNewList(treePalm4List = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f, 5.0f);
			drawPalmLevel(0.0f, 0.6f, 0.6f, 0.8f, 7.0f, 0.39f, 0.25f);
		glEndList();
		glNewList(treePalm4SmallList = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.7f, 3.0f);
			drawPalmLevel(0.0f, 0.6f, 0.6f, 0.8f, 3.0f, 0.39f, 0.25f);
		glEndList();

		glNewList(treePalmBList = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.5f, 5.0f, 0.664f, 0.0f, 0.172f, 0.656f);
			drawPineLevel(0.164f, 0.836f, 0.5f, 0.4f, 0.3f, 0.164f, 8.0f, true, 15.0f);
			drawPineLevel(0.164f, 0.836f, 0.4f, 0.5f, 0.4f, 0.164f, 8.0f, false);
		glEndList();
		glNewList(treePalmB2List = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.5f, 5.0f, 0.664f, 0.0f, 0.172f, 0.656f);
			drawPineLevel(0.5f, 0.836f, 0.5f, 0.4f, 0.3f, 0.164f, 8.0f, true, 15.0f);
			drawPineLevel(0.5f, 0.836f, 0.4f, 0.5f, 0.4f, 0.164f, 8.0f, false);
		glEndList();
		glNewList(treePalmB3List = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.5f, 5.0f, 0.664f, 0.0f, 0.172f, 0.656f);
			drawPineLevel(0.836f, 0.836f, 0.5f, 0.4f, 0.3f, 0.164f, 8.0f, true, 15.0f);
			drawPineLevel(0.836f, 0.836f, 0.4f, 0.5f, 0.4f, 0.164f, 8.0f, false);
		glEndList();
		glNewList(treePalmB4List = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.5f, 5.0f, 0.664f, 0.0f, 0.172f, 0.656f);
			drawPineLevel(0.164f, 0.5f, 0.5f, 0.4f, 0.3f, 0.164f, 8.0f, true, 15.0f);
			drawPineLevel(0.164f, 0.5f, 0.4f, 0.5f, 0.4f, 0.164f, 8.0f, false);
		glEndList();
		glNewList(treePalmB5List = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.5f, 5.0f, 0.664f, 0.0f, 0.172f, 0.656f);
			drawPineLevel(0.5f, 0.5f, 0.5f, 0.4f, 0.3f, 0.164f, 8.0f, true, 15.0f);
			drawPineLevel(0.5f, 0.5f, 0.4f, 0.5f, 0.4f, 0.164f, 8.0f, false);
		glEndList();
		glNewList(treePalmB6List = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.5f, 5.0f, 0.664f, 0.0f, 0.172f, 0.656f);
			drawPineLevel(0.164f, 0.164f, 0.5f, 0.4f, 0.3f, 0.164f, 8.0f, true, 15.0f);
			drawPineLevel(0.164f, 0.164f, 0.4f, 0.5f, 0.4f, 0.164f, 8.0f, false);
		glEndList();
		glNewList(treePalmB7List = glGenLists(1), GL_COMPILE);
			drawPalmTrunc(0.07f, 0.5f, 5.0f, 0.664f, 0.0f, 0.172f, 0.656f);
			drawPineLevel(0.164f, 0.5f, 0.5f, 0.4f, 0.3f, 0.164f, 8.0f, true, 15.0f);
			drawPineLevel(0.164f, 0.5f, 0.4f, 0.5f, 0.4f, 0.164f, 8.0f, false);
		glEndList();

		glNewList(treePalmBurntList = glGenLists(1), GL_COMPILE);
			glColor3f(0.3f, 0.3f, 0.3f);
			drawPalmTrunc(0.07f, 0.7f, 5.0f);
		glEndList();
		glNewList(treePalmBurntSmallList = glGenLists(1), GL_COMPILE);
			glColor3f(0.3f, 0.3f, 0.3f);
			drawPalmTrunc(0.07f, 0.7f, 3.0f);
		glEndList();

		glNewList(treeOakList = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.02f, 0.75f, 0.0f, 0.836f, 0.0f, 0.164f, 0.164f, 8.0f);
			drawPineLevel(0.5f, 0.836f, 0.4f, 0.5f, 0.3f, 0.164f, 8.0f, false, 15.0f);
			drawPineLevel(0.164f, 0.836f, 0.3f, 0.6f, 0.5f, 0.164f, 8.0f, false);
			drawPineLevel(0.5f, 0.836f, 0.2f, 0.8f, 0.6f, 0.164f, 8.0f, true, 15.0f);
		glEndList();
		glNewList(treeOakSmallList = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.02f, 0.75f, 0.0f, 0.836f, 0.0f, 0.164f, 0.164f, 8.0f);
			drawPineLevel(0.164f, 0.836f, 0.3f, 0.6f, 0.5f, 0.164f, 8.0f, true);
		glEndList();	

		glNewList(treeOak2List = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.02f, 0.75f, 0.0f, 0.836f, 0.0f, 0.164f, 0.164f, 8.0f);
			drawPineLevel(0.164f, 0.5f, 0.4f, 0.5f, 0.3f, 0.164f, 8.0f, false, 15.0f);
			drawPineLevel(0.836f, 0.836f, 0.3f, 0.6f, 0.5f, 0.164f, 8.0f, false);
			drawPineLevel(0.164f, 0.5f, 0.2f, 0.8f, 0.6f, 0.164f, 8.0f, true, 15.0f);
		glEndList();
		glNewList(treeOak2SmallList = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.02f, 0.75f, 0.0f, 0.836f, 0.0f, 0.164f, 0.164f, 8.0f);
			drawPineLevel(0.836f, 0.836f, 0.3f, 0.6f, 0.5f, 0.164f, 8.0f, true);
		glEndList();	

		glNewList(treeOak3List = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.02f, 0.75f, 0.0f, 0.836f, 0.0f, 0.164f, 0.164f, 8.0f);
			drawPineLevel(0.836f, 0.5f, 0.4f, 0.5f, 0.3f, 0.164f, 8.0f, false, 15.0f);
			drawPineLevel(0.5f, 0.5f, 0.3f, 0.6f, 0.5f, 0.164f, 8.0f, false);
			drawPineLevel(0.836f, 0.5f, 0.2f, 0.8f, 0.6f, 0.164f, 8.0f, true, 15.0f);
		glEndList();
		glNewList(treeOak3SmallList = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.02f, 0.75f, 0.0f, 0.836f, 0.0f, 0.164f, 0.164f, 8.0f);
			drawPineLevel(0.5f, 0.5f, 0.3f, 0.6f, 0.5f, 0.164f, 8.0f, true);
		glEndList();	

		glNewList(treeOak4List = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.02f, 0.75f, 0.0f, 0.836f, 0.0f, 0.164f, 0.164f, 8.0f);
			drawPineLevel(0.5f, 0.164f, 0.4f, 0.5f, 0.3f, 0.164f, 8.0f, false, 15.0f);
			drawPineLevel(0.164f, 0.164f, 0.3f, 0.6f, 0.5f, 0.164f, 8.0f, false);
			drawPineLevel(0.5f, 0.164f, 0.2f, 0.8f, 0.6f, 0.164f, 8.0f, true, 15.0f);
		glEndList();
		glNewList(treeOak4SmallList = glGenLists(1), GL_COMPILE);
			drawPineTrunc(0.02f, 0.75f, 0.0f, 0.836f, 0.0f, 0.164f, 0.164f, 8.0f);
			drawPineLevel(0.164f, 0.164f, 0.3f, 0.6f, 0.5f, 0.164f, 8.0f, true);
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
	case ePine2:
		pineTextureB_.draw();
		treeList = treePine2List;
		smallTreeList = treePine2SmallList;
		break;
	case ePine3:
		pineTextureB_.draw();
		treeList = treePine3List;
		smallTreeList = treePine3SmallList;
		break;
	case ePine4:
		pineTextureB_.draw();
		treeList = treePine4List;
		smallTreeList = treePine4SmallList;
		break;
	case ePine2Snow:
		pineTextureB_.draw();
		treeList = treePine2SnowList;
		smallTreeList = treePine2SnowSmallList;
		break;
	case ePine3Snow:
		pineTextureB_.draw();
		treeList = treePine3SnowList;
		smallTreeList = treePine3SnowSmallList;
		break;
	case ePine4Snow:
		pineTextureB_.draw();
		treeList = treePine4SnowList;
		smallTreeList = treePine4SnowSmallList;
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
	case ePalm2:
		palmTextureA_.draw();
		treeList = treePalm2List;
		smallTreeList = treePalm2SmallList;
		break;
	case ePalm3:
		palmTextureA_.draw();
		treeList = treePalm3List;
		smallTreeList = treePalm3SmallList;
		break;
	case ePalm4:
		palmTextureA_.draw();
		treeList = treePalm4List;
		smallTreeList = treePalm4SmallList;
		break;
	case ePalmB:
		palmTextureB_.draw();
		treeList = treePalmBList;
		smallTreeList = treePalmBList;
		break;
	case ePalmB2:
		palmTextureB_.draw();
		treeList = treePalmB2List;
		smallTreeList = treePalmB2List;
		break;
	case ePalmB3:
		palmTextureB_.draw();
		treeList = treePalmB3List;
		smallTreeList = treePalmB3List;
		break;
	case ePalmB4:
		palmTextureB_.draw();
		treeList = treePalmB4List;
		smallTreeList = treePalmB4List;
		break;
	case ePalmB5:
		palmTextureB_.draw();
		treeList = treePalmB5List;
		smallTreeList = treePalmB5List;
		break;
	case ePalmB6:
		palmTextureB_.draw();
		treeList = treePalmB6List;
		smallTreeList = treePalmB6List;
		break;
	case ePalmB7:
		palmTextureB_.draw();
		treeList = treePalmB7List;
		smallTreeList = treePalmB7List;
		break;
	case ePalmBurnt:
		palmTextureA_.draw();
		treeList = treePalmBurntList;
		smallTreeList = treePalmBurntSmallList;
		break;
	case eOak:
		oakTextureA_.draw();
		treeList = treeOakList;
		smallTreeList = treeOakSmallList;
		break;
	case eOak2:
		oakTextureA_.draw();
		treeList = treeOak2List;
		smallTreeList = treeOak2SmallList;
		break;
	case eOak3:
		oakTextureA_.draw();
		treeList = treeOak3List;
		smallTreeList = treeOak3SmallList;
		break;
	case eOak4:
		oakTextureA_.draw();
		treeList = treeOak4List;
		smallTreeList = treeOak4SmallList;
		break;
	};

	DIALOG_ASSERT(treeList && smallTreeList);

	bool vertexLighting = OptionsDisplay::instance()->getNoModelLighting();
	if (!vertexLighting)
	{
		Vector4 ambientColor(0.4f, 0.4f, 0.4f, 1.0f);
		Vector4 diffuseColor(1.2f, 1.2f, 1.2f, 1.2f);
		Vector4 specularColor(0.0f, 0.0f, 0.0f, 1.0f);
		Vector4 emissiveColor(0.0f, 0.0f, 0.0f, 1.0f);
		float shininess = 0.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissiveColor);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	}

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

void LandscapeObjectsEntryTree::simulate(float frameTime)
{
}

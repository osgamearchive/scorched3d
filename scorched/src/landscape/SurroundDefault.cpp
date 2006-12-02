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

#include <GLEXT/GLState.h>
#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLInfo.h>
#include <cgext/CGFog.h>
#include <client/ScorchedClient.h>
#include <graph/OptionsDisplay.h>
#include <landscape/Sky.h>
#include <landscape/Landscape.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapedef/LandscapeDefinition.h>
#include <landscapedef/LandscapeTex.h>
#include <landscape/SurroundDefault.h>

SurroundDefault::SurroundDefault(HeightMap &map) :
	listNo_(0), map_(map)
{

}

SurroundDefault::~SurroundDefault()
{

}

void SurroundDefault::draw()
{
	bool detail = (GLStateExtension::glActiveTextureARB() &&
		GLStateExtension::getTextureUnits() > 2 &&
		OptionsDisplay::instance()->getDetailTexture() &&
		GLStateExtension::hasEnvCombine());
	if (!listNo_)
	{
		glNewList(listNo_ = glGenLists(1), GL_COMPILE);
			generateList(detail);
		glEndList();
	}
	
	if (detail)
	{
		GLStateExtension::glActiveTextureARB()(GL_TEXTURE2_ARB);
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
		Landscape::instance()->getDetailTexture().draw(true);
		GLStateExtension::glActiveTextureARB()(GL_TEXTURE0_ARB);
	}
	Landscape::instance()->getGroundTexture().draw(true);

	//CGFog::instance()->bind(
	//	Landscape::instance()->getGroundTexture().getTexName());
	glCallList(listNo_);
	//CGFog::instance()->unBind();

	if (detail)
	{
		GLStateExtension::glActiveTextureARB()(GL_TEXTURE2_ARB);
		glDisable(GL_TEXTURE_2D);
		GLStateExtension::glActiveTextureARB()(GL_TEXTURE0_ARB);
	}

	GLInfo::addNoTriangles(8);
}

void SurroundDefault::generate()
{
	if (listNo_) glDeleteLists(listNo_, 1);
	listNo_ = 0;
}

void SurroundDefault::generateVerts()
{
	int width = 1536 + map_.getMapWidth();
	int height = 1536 + map_.getMapHeight();
	Vector centre(map_.getMapWidth() / 2, map_.getMapHeight() / 2, 0);
	Vector offset(map_.getMapWidth() + width, map_.getMapHeight() + height, 0);

	Vector offset2(map_.getMapWidth() / 2, map_.getMapHeight() / 2, 0);
	hMapBoxVerts_[0] = Vector(centre[0] - offset2[0], centre[1] - offset2[1], 0.0f);
	hMapBoxVerts_[1] = Vector(centre[0] - offset2[0], centre[1] + offset2[1], 0.0f);
	hMapBoxVerts_[2] = Vector(centre[0] + offset2[0], centre[1] + offset2[1], 0.0f);
	hMapBoxVerts_[3] = Vector(centre[0] + offset2[0], centre[1] - offset2[1], 0.0f);

	Vector offset3(width + map_.getMapWidth() * 2, height + map_.getMapWidth() * 2, 0);
	hMapBoxVerts_[4] = Vector(centre[0] - offset3[0], centre[1] - offset3[1], 0.0f);
	hMapBoxVerts_[5] = Vector(centre[0] - offset3[0], centre[1] + offset3[1], 0.0f);
	hMapBoxVerts_[6] = Vector(centre[0] + offset3[0], centre[1] + offset3[1], 0.0f);
	hMapBoxVerts_[7] = Vector(centre[0] + offset3[0], centre[1] - offset3[1], 0.0f);

	hMapBoxVerts_[8] = Vector(centre[0] - offset2[0], centre[1] - offset3[1], 0.0f);
	hMapBoxVerts_[9] = Vector(centre[0] - offset2[0], centre[1] + offset3[1], 0.0f);
	hMapBoxVerts_[10] = Vector(centre[0] + offset2[0], centre[1] + offset3[1], 0.0f);
	hMapBoxVerts_[11] = Vector(centre[0] + offset2[0], centre[1] - offset3[1], 0.0f);

	hMapBoxVerts_[12] = Vector(centre[0] - offset3[0], centre[1] - offset2[1], 0.0f);
	hMapBoxVerts_[13] = Vector(centre[0] - offset3[0], centre[1] + offset2[1], 0.0f);
	hMapBoxVerts_[14] = Vector(centre[0] + offset3[0], centre[1] + offset2[1], 0.0f);
	hMapBoxVerts_[15] = Vector(centre[0] + offset3[0], centre[1] - offset2[1], 0.0f);
}

void SurroundDefault::generateList(bool detail)
{
	generateVerts();

	const int dataOfs[8][4] = {
		{8,11,3,0},
		{1,2,10,9},
		{4,8,0,12},
		{11,7,15,3},
		{3,15,14,2},
		{2,14,6,10},
		{13,1,9,5},
		{12,0,1,13}
	};

	LandscapeTex &tex =
		*ScorchedClient::instance()->getLandscapeMaps().getDefinitions().getTex();
	Vector &ambient = tex.skyambience;
	Vector &diffuse = tex.skydiffuse;
	Vector &sunPos =
		Landscape::instance()->getSky().getSun().getPosition();
	Vector normal(0.0f, 0.0f, 1.0f);

	float texMultX = 
		64.0f *
        (float(Landscape::instance()->getGroundTexture().getWidth()) /
		256.0f);
	float texMultY = 
		64.0f *
        (float(Landscape::instance()->getGroundTexture().getHeight()) /
		256.0f);

	glBegin(GL_QUADS);
	for (int i=0; i<8; i++) 
	{
		for (int j=0; j<4; j++) 
		{
			float x = hMapBoxVerts_[dataOfs[i][j]][0] / texMultX;
			float y = hMapBoxVerts_[dataOfs[i][j]][1] / texMultY;

			Vector pos = hMapBoxVerts_[dataOfs[i][j]];
			Vector sunDirection = (sunPos - pos).Normalize();
			float diffuseLightMult = 
				(((normal.dotP(sunDirection)) / 2.0f) + 0.5f);			
			Vector light = diffuse * diffuseLightMult + ambient;
			light[0] = MIN(1.0f, light[0]);
			light[1] = MIN(1.0f, light[1]);
			light[2] = MIN(1.0f, light[2]);

			glTexCoord2f(x, y);
			if (detail) GLStateExtension::glMultiTextCoord2fARB()
				(GL_TEXTURE2_ARB, x, y);
			glColor3fv(light);
			glVertex3fv(pos);
		}
	}
	glEnd();
}

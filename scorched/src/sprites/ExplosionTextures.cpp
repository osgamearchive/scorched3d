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


// ExplosionTextures.cpp: implementation of the ExplosionTextures class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <GLEXT/GLBitmap.h>
#include <sprites/ExplosionTextures.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ExplosionTextures *ExplosionTextures::instance_ = 0;

ExplosionTextures *ExplosionTextures::instance()
{
	if (!instance_)
	{
		instance_ = new ExplosionTextures;
	}

	return instance_;
}

ExplosionTextures::ExplosionTextures()
{

}

ExplosionTextures::~ExplosionTextures()
{

}

void ExplosionTextures::addTextureToSet(GLTextureSet &set,
										const char *texPath)
{
	GLBitmap bitmap((char *) texPath, true);
	GLTexture *texture = new GLTexture;
	if (!texture->create(bitmap, GL_RGBA)) abort();
	set.addTexture(texture);
}

void ExplosionTextures::createTextures(ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Explosion Textures");

	GLBitmap bitmap( PKGDIR "data/textures/smoke01.bmp", true);
	smokeTexture.create(bitmap, GL_RGBA);

	addTextureToSet(exp00,  PKGDIR "data/textures/explode/exp00_1.bmp");
	addTextureToSet(exp00,  PKGDIR "data/textures/explode/exp00_2.bmp");
	addTextureToSet(exp00,  PKGDIR "data/textures/explode/exp00_3.bmp");
	addTextureToSet(exp00,  PKGDIR "data/textures/explode/exp00_4.bmp");
	addTextureToSet(exp00,  PKGDIR "data/textures/explode/exp00_5.bmp");
	addTextureToSet(exp00,  PKGDIR "data/textures/explode/exp00_6.bmp");
	addTextureToSet(exp00,  PKGDIR "data/textures/explode/exp00_7.bmp");
	addTextureToSet(exp00,  PKGDIR "data/textures/explode/exp00_8.bmp");
	addTextureToSet(exp00,  PKGDIR "data/textures/explode/exp00_9.bmp");
	addTextureToSet(exp00,  PKGDIR "data/textures/explode/exp00_10.bmp");
	textureSets["exp00"] = &exp00;

	addTextureToSet(exp01,  PKGDIR "data/textures/explode/exp01_1.bmp");
	addTextureToSet(exp01,  PKGDIR "data/textures/explode/exp01_2.bmp");
	addTextureToSet(exp01,  PKGDIR "data/textures/explode/exp01_3.bmp");
	textureSets["exp01"] = &exp01;

	addTextureToSet(exp02,  PKGDIR "data/textures/explode/exp02_1.bmp");
	addTextureToSet(exp02,  PKGDIR "data/textures/explode/exp02_2.bmp");
	addTextureToSet(exp02,  PKGDIR "data/textures/explode/exp02_3.bmp");
	textureSets["exp02"] = &exp02;

	addTextureToSet(exp03,  PKGDIR "data/textures/explode/exp03_1.bmp");
	addTextureToSet(exp03,  PKGDIR "data/textures/explode/exp03_2.bmp");
	addTextureToSet(exp03,  PKGDIR "data/textures/explode/exp03_3.bmp");
	textureSets["exp03"] = &exp03;

	addTextureToSet(exp04,  PKGDIR "data/textures/explode/exp04_1.bmp");
	addTextureToSet(exp04,  PKGDIR "data/textures/explode/exp04_2.bmp");
	addTextureToSet(exp04,  PKGDIR "data/textures/explode/exp04_3.bmp");
	textureSets["exp04"] = &exp04;

	addTextureToSet(exp05,  PKGDIR "data/textures/explode/exp05_1.bmp");
	addTextureToSet(exp05,  PKGDIR "data/textures/explode/exp05_2.bmp");
	addTextureToSet(exp05,  PKGDIR "data/textures/explode/exp05_3.bmp");
	addTextureToSet(exp05,  PKGDIR "data/textures/explode/exp05_4.bmp");
	addTextureToSet(exp05,  PKGDIR "data/textures/explode/exp05_5.bmp");
	addTextureToSet(exp05,  PKGDIR "data/textures/explode/exp05_6.bmp");
	addTextureToSet(exp05,  PKGDIR "data/textures/explode/exp05_7.bmp");
	textureSets["exp05"] = &exp05;
}

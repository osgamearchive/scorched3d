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

#include <stdio.h>
#include <GLEXT/GLBitmap.h>
#include <sprites/ExplosionTextures.h>

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
	GLBitmap bitmap((char *) texPath, (char *) texPath, false);
	GLTexture *texture = new GLTexture;

	if (!texture->create(bitmap, GL_RGBA))
	{
		dialogMessage("ExplosionTextures", "Failed to load texture \"%s\"",
			texPath);
		exit(1);
	}
	set.addTexture(texture);
}

void ExplosionTextures::createTextureSet(GLTextureSet &set,
										 int number, 
										 const char *baseName,
										 bool zeros)
{
	char name[256];
	for (int i=1; i<=number; i++)
	{
		if (!zeros) sprintf(name, "%s%i.bmp", baseName, i);
		else sprintf(name, "%s%02i.bmp", baseName, i);
		addTextureToSet(set, name);
	}
}

void ExplosionTextures::createTextures(ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Explosion Textures");

	std::string file1 = getDataFile("data/textures/smoke01.bmp");
	GLBitmap bitmap(file1.c_str(), file1.c_str(), false);
	smokeTexture.create(bitmap, GL_RGBA);

	std::string file2 = getDataFile("data/textures/smoke02.bmp");
	GLBitmap bitmap2(file2.c_str(), file2.c_str(), false);
	smokeTexture.create(bitmap2, GL_RGBA);

	createTextureSet(exp00, 10, getDataFile("data/textures/explode/exp00_"));
	textureSets["exp00"] = &exp00;

	createTextureSet(exp01, 3, getDataFile("data/textures/explode/exp01_"));
	textureSets["exp01"] = &exp01;

	createTextureSet(exp02, 3, getDataFile("data/textures/explode/exp02_"));
	textureSets["exp02"] = &exp02;

	createTextureSet(exp03, 3, getDataFile("data/textures/explode/exp03_"));
	textureSets["exp03"] = &exp03;

	createTextureSet(exp04, 3, getDataFile("data/textures/explode/exp04_"));
	textureSets["exp04"] = &exp04;

	createTextureSet(exp05, 7, getDataFile("data/textures/explode/exp05_"));
	textureSets["exp05"] = &exp05;

	createTextureSet(flames, 33, getDataFile("data/textures/flame/flame"), true);
}

GLTextureSet *ExplosionTextures::getTextureSetByName(const char *name)
{
	GLTextureSet * result = &exp00;
	std::map<std::string, GLTextureSet*>::iterator itor =
		textureSets.find(name);
	if (itor != textureSets.end())
	{
		result = (*itor).second;
	}
	return result;
}

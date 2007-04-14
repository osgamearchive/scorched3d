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

#include <GLEXT/GLImageFactory.h>
#include <XML/XMLFile.h>
#include <sprites/ExplosionTextures.h>
#include <landscape/Landscape.h>
#include <common/Defines.h>
#include <stdio.h>

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

bool ExplosionTextures::addTextureToSet(GLTextureSet &set,
										const char *texPath)
{
	GLImageHandle bitmap =
		GLImageFactory::loadImageHandle(
			(char *) texPath, (char *) texPath, false);
	GLTexture *texture = new GLTexture;

	if (!texture->create(bitmap, GL_RGBA)) return false;
	set.addTexture(texture);
	return true;
}

bool ExplosionTextures::createTextures(ProgressCounter *counter)
{
	if (counter) counter->setNewOp("Explosion Textures");

	std::string file1 = getDataFile("data/textures/smoke01.bmp");
	GLImageHandle bitmap = GLImageFactory::loadImageHandle(file1.c_str(), file1.c_str(), false);
	smokeTexture.create(bitmap, GL_RGBA);
	DIALOG_ASSERT(smokeTexture.textureValid());

	std::string file2 = getDataFile("data/textures/smoke02.bmp");
	GLImageHandle bitmap2 = GLImageFactory::loadImageHandle(file2.c_str(), file2.c_str(), false);
	smokeTexture2.create(bitmap2, GL_RGBA);
	DIALOG_ASSERT(smokeTexture2.textureValid());

	std::string file3 = getDataFile("data/textures/particle.bmp");
	GLImageHandle bitmap3 = GLImageFactory::loadImageHandle(file3.c_str(), file3.c_str(), false);
	particleTexture.create(bitmap3, GL_RGBA);
	DIALOG_ASSERT(particleTexture.textureValid());

	GLImageHandle talkBitmap = GLImageFactory::loadAlphaImageHandle(getDataFile("data/textures/talk.bmp"));
	talkTexture.create(talkBitmap, GL_RGBA);
	DIALOG_ASSERT(talkTexture.textureValid());

	std::string file4 = getDataFile("data/textures/lightning.bmp");
	GLImageHandle bitmap4 = GLImageFactory::loadImageHandle(file4.c_str(), file4.c_str(), false);
	lightningTexture.create(bitmap4, GL_RGBA);
	DIALOG_ASSERT(lightningTexture.textureValid());

	std::string file5 = getDataFile("data/textures/rain.bmp");
	std::string file5m = getDataFile("data/textures/rainm.bmp");
	GLImageHandle bitmap5 = GLImageFactory::loadImageHandle(file5m.c_str(), file5.c_str(), false);
	rainTexture.create(bitmap5, GL_RGBA);
	DIALOG_ASSERT(rainTexture.textureValid());

	std::string file6 = getDataFile("data/textures/snow.bmp");
	std::string file6m = getDataFile("data/textures/snowm.bmp");
	GLImageHandle bitmap6 = GLImageFactory::loadImageHandle(file6.c_str(), file6m.c_str(), false);
	snowTexture.create(bitmap6, GL_RGBA);
	DIALOG_ASSERT(snowTexture.textureValid());

	XMLFile file;
	if (!file.readFile(getDataFile("data/textureset.xml")))
	{
		dialogMessage("ExplosionTextures", formatString(
					  "Failed to parse \"%s\"\n%s", 
					  "data/textureset.xml",
					  file.getParserError()));
		return false;
	}
	if (!file.getRootNode())
	{
		dialogMessage("ExplosionTextures", formatString(
					  "Failed to find explosion textures definition file \"%s\"",
					  "data/textureset.xml"));
		return false;		
	}

    std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = file.getRootNode()->getChildren();
    for (childrenItor = children.begin();
		 childrenItor != children.end();
		 childrenItor++)
    {
		// Check if it is a texture set
        XMLNode *currentNode = (*childrenItor);
		if (strcmp(currentNode->getName(), "textureset"))
		{
			dialogMessage("ExplosionTextures",
						  "Failed to find textureset node");
			return false;
		}
		XMLNode *name = 0;
		if (!currentNode->getNamedParameter("name", name)) return false;
		const char *setName = name->getContent();

		// Create and store the new texture set
		GLTextureSet *set = new GLTextureSet();
		textureSets[setName] = set;

		// Load all the textures
		std::list<XMLNode *>::iterator textureItor;
		std::list<XMLNode *> &textures = currentNode->getChildren();
		for (textureItor = textures.begin();
			textureItor != textures.end();
			textureItor++)
		{
			// Check if it is a texture
			XMLNode *currentTexture = (*textureItor);
			if (strcmp(currentTexture->getName(), "texture"))
			{
				dialogMessage("ExplosionTextures",
							"Failed to find texture sub-node");
				return false;
			}

			// Load texture
			const char *texFile = 
				getDataFile(formatString("data/%s", currentTexture->getContent()));
			if (!addTextureToSet(*set, texFile))
			{
				dialogMessage("ExplosionTextures", formatString(
							"Failed to load texture %s",
							texFile));
				return false;
			}
		}
	}

	return true;
}

GLTextureSet *ExplosionTextures::getTextureSetByName(const char *name)
{
	DIALOG_ASSERT(!textureSets.empty());

	GLTextureSet *result = (*textureSets.begin()).second;
	std::map<std::string, GLTextureSet*>::iterator itor =
		textureSets.find(name);
	if (itor != textureSets.end())
	{
		result = (*itor).second;
	}
	return result;
}

GLImage &ExplosionTextures::getScorchBitmap(const char *name)
{
	if (name[0])
	{
		std::map<std::string, GLImage*>::iterator findItor =
			scorchedBitmaps.find(name);
		if (findItor != scorchedBitmaps.end())
		{
			return *(*findItor).second;
		}

		const char *fileName = getDataFile(name);
		if (s3d_fileExists(fileName))
		{
			GLImage *map = GLImageFactory::loadImage(fileName);
			scorchedBitmaps[name] = map;
			return *map;
		}
	}
	return Landscape::instance()->getScorchMap();
}

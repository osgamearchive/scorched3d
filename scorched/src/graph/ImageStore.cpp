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

#include <graph/ImageStore.h>
#include <GLEXT/GLIMageFactory.h>
#include <graph/OptionsDisplay.h>
#include <common/Defines.h>

ImageStore *ImageStore::instance_ = 0;

ImageStore *ImageStore::instance()
{
	if (!instance_)
	{
		instance_ = new ImageStore;
	}
	return instance_;
}

ImageStore::ImageStore()
{
}

ImageStore::~ImageStore()
{
}

GLImage *ImageStore::loadImage(ImageID &imageId)
{
	std::map<std::string, GLImage *>::iterator findItor =
		imageMap_.find(imageId.getStringHash());
	if (findItor == imageMap_.end())
	{
		GLImage *image = getImage(imageId);
		imageMap_[imageId.getStringHash()] = image;
		return image;
	}
	return (*findItor).second;
}

GLImage *ImageStore::getImage(ImageID &id)
{
	GLImage *image = 0;
	if (id.getAlphaName()[0])
	{
		image = GLImageFactory::loadImage(
			formatString(getDataFile(id.getImageName())),
			formatString(getDataFile(id.getAlphaName())),
			id.getInvert());
	}
	else
	{
		image = GLImageFactory::loadImage(
			getDataFile(id.getImageName()));
	}

	return image;
}

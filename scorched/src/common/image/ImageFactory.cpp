////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2013
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
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#include <image/ImageFactory.h>
#include <image/ImagePngFactory.h>
#include <string>

ImageFactory::ImageFactory()
{
}

Image ImageFactory::loadImageID(
	const ImageID &imageId)
{
	ImageID &otherId = (ImageID &) imageId;
	if (otherId.getImageName()[0] && !otherId.getAlphaName()[0])
	{
		return loadImageInternal(otherId.getImageLocation(), otherId.getImageName(), false);
	}
	else if (!otherId.getImageName()[0] && otherId.getAlphaName()[0])
	{
		return loadImageInternal(otherId.getImageLocation(), otherId.getAlphaName(), true);
	}
	else if (otherId.getImageName()[0] && otherId.getAlphaName()[0])
	{
		Image bitmap = loadImageInternal(otherId.getImageLocation(), otherId.getImageName(), false);
		Image alpha = loadImageInternal(otherId.getImageLocation(), otherId.getAlphaName(), false);
		return combineImage(bitmap, alpha, otherId.getInvert());
	}
	return Image();
}

Image ImageFactory::loadAlphaImage(
	S3D::FileLocation imageLocation,
	const std::string &filename)
{
	ImageID imageId(imageLocation, "", filename);
	return loadImageID(imageId);
}

Image ImageFactory::loadImage(
	S3D::FileLocation imageLocation,
	const std::string &filename, 
	const std::string &alphafilename, 
	bool invert)
{
	ImageID imageId(imageLocation, filename, alphafilename, invert);
	return loadImageID(imageId);
}

Image ImageFactory::loadImageInternal(S3D::FileLocation imageLocation, const std::string &filename, bool loadAlpha)
{
	std::string expandedFilename = S3D::getLocation(imageLocation, filename);
	if (strstr(filename.c_str(), ".png"))
	{
		return ImagePngFactory::loadFromFile(expandedFilename.c_str(), loadAlpha);
	}
	else 
	{
		S3D::dialogExit("ImageFactory", 
			S3D::formatStringBuffer("Only PNG image types are supported : %s", expandedFilename.c_str()));
	}
	return Image();
}

Image ImageFactory::combineImage(Image bitmap, Image alpha, bool invert)
{
	Image result;
	if (bitmap.getBits() && alpha.getBits() && 
		bitmap.getWidth() == alpha.getWidth() &&
		bitmap.getHeight() == alpha.getHeight())
	{
		DIALOG_ASSERT((bitmap.getComponents() == 3 && alpha.getComponents() == 1) ||
			(bitmap.getComponents() == 3 && alpha.getComponents() == 3) ||
			(bitmap.getComponents() == 1 && alpha.getComponents() == 3) ||
			(bitmap.getComponents() == 1 && alpha.getComponents() == 1));

		result = Image(bitmap.getWidth(), bitmap.getHeight(), true);

		unsigned char *bbits = bitmap.getBits();
		unsigned char *abits = alpha.getBits();
		unsigned char *bits = result.getBits();
		for (int y=0; y<bitmap.getHeight(); y++)
		{
			for (int x=0; x<bitmap.getWidth(); x++)
			{
				bits[0] = bbits[0];
				if (bitmap.getComponents() > 1)
				{
					bits[1] = bbits[1];
					bits[2] = bbits[2];
				}

				unsigned char avg = 0;
				if (alpha.getComponents() == 3) avg = (unsigned char)(int(abits[0] + abits[1] + abits[2]) / 3);
				else if (alpha.getComponents() == 1) avg = abits[0];
				if (invert)
				{
					bits[3] = (unsigned char)(255 - avg);
				}
				else
				{
					bits[3] = avg;
				}

				bbits += bitmap.getComponents();
				abits += alpha.getComponents();
				bits += result.getComponents();
			}
		}
	}

	return result;
}

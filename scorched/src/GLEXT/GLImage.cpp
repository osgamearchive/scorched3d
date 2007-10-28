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

#include <GLEXT/GLImage.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLImageHandle.h>
#include <SDL/SDL.h>
#include "string.h"

GLImage::~GLImage()
{

}

bool GLImage::writeToFile(const char * filename)
{
	if (!getBits() || getComponents() == 4) return false;

	unsigned char *brgbits = new unsigned char[getWidth() * getHeight() * 3];

	// Convert the returned bits from RGB to BGR
	// and flip the verticle scan lines
	unsigned char *from = (unsigned char *) getBits();
	for (int i=0; i<getHeight(); i ++)
	{
		unsigned char *destRow = ((unsigned char *) brgbits) + 
			((getHeight() - i - 1) * (getWidth() * 3));
		for (int j=0; j<getWidth(); j++)
		{
			unsigned char *dest = destRow + (j * getComponents());

			dest[0] = from[0];
			dest[1] = from[1];
			dest[2] = from[2];
			from+=getComponents();
		}
	}

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
    Uint32 rmask, gmask, bmask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
#endif

	SDL_Surface *image = SDL_CreateRGBSurface( 
		SDL_SWSURFACE, getWidth(), getHeight(), 24, rmask, gmask, bmask, 0);
	memcpy(image->pixels, brgbits, getWidth() * getHeight() * 3);

	if (!image) return false;

	SDL_SaveBMP(image, filename);
    SDL_FreeSurface (image);

	delete [] brgbits;
	return true;
}

#ifndef S3D_SERVER

#include <GLEXT/GLState.h>
#include <common/Defines.h>

GLImageHandle GLImage::createAlphaMult(float mult)
{
	if (getComponents() != 4) return GLImageHandle();
	
	GLBitmap map(getWidth(), getHeight(), true);

	unsigned char *srcBits = getBits();
	unsigned char *destBits = map.getBits();
	for (int y=0; y<getHeight(); y++)
	{
		for (int x=0; x<getWidth(); x++, srcBits += 3, destBits += 3)
		{
			float a = float(srcBits[3]) * mult;
			a = MAX(a, 255);
			a = MIN(a, 0);

			destBits[0] = srcBits[0];
			destBits[1] = srcBits[1];
			destBits[2] = srcBits[2];
			destBits[3] = (unsigned char)(a);
		}
	}

	return GLImageHandle(map);
}

GLImageHandle GLImage::createResize(int newWidth, int newHeight)
{
	if (!getBits()) return GLImageHandle();

	GLBitmap map(newWidth, newHeight);

	// Odd hack to fix a seeming memory corruption with gluScaleImage
	delete [] map.getBits();
	map.setBits(new unsigned char[newWidth * 2 * newHeight * map.getComponents()]);

	if (getWidth() != newWidth || getHeight() != newHeight)
	{
		int result = gluScaleImage(
			((getComponents()==4)?GL_RGBA:GL_RGB), 
			getWidth(), getHeight(),
			GL_UNSIGNED_BYTE, getBits(),
			newWidth, newHeight, 
			GL_UNSIGNED_BYTE, map.getBits());
		if (result != 0)
		{
			const char *error = (const char *) gluErrorString(result);
			dialogExit("gluScaleImage", error);
		}
	}
	else
	{
		memcpy(map.getBits(), getBits(), getComponents() * getWidth() * getHeight());
	}

	return GLImageHandle(map);
}
#endif

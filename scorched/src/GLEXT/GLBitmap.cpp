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


// GLBitmap.cpp: implementation of the GLBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLBitmap.h>
#include <SDL/SDL.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLBitmap::GLBitmap() :
	width_(0), height_(0), alpha_(false), newbits_(0)
{

}

GLBitmap::GLBitmap(int startWidth, int startHeight, bool alpha) : 
	width_(startWidth), height_(startHeight), alpha_(alpha), newbits_(0)
{
	createBlank(startWidth, startHeight, alpha);
}

GLBitmap::GLBitmap(char * filename, bool alpha) :
	newbits_(NULL), width_(0), height_(0), alpha_(alpha)
{
	if (filename)
	{
		loadFromFile(filename, alpha);
	}
}

GLBitmap::GLBitmap(char * filename, char *alphafilename, bool invert) : 
	newbits_(NULL), width_(0), height_(0), alpha_(false)
{
	GLBitmap bitmap(filename, false);
	GLBitmap alpha(alphafilename, false);

	if (bitmap.getBits() && alpha.getBits() && 
		bitmap.getWidth() == alpha.getWidth() &&
		bitmap.getHeight() == alpha.getHeight())
	{
		createBlank(bitmap.getWidth(), bitmap.getHeight(), true);
		GLubyte *bbits = bitmap.getBits();
		GLubyte *abits = alpha.getBits();
		GLubyte *bits = getBits();
		for (int y=0; y<bitmap.getHeight(); y++)
		{
			for (int x=0; x<bitmap.getWidth(); x++)
			{
				bits[0] = bbits[0];
				bits[1] = bbits[1];
				bits[2] = bbits[2];

				GLubyte avg = GLubyte(int(abits[0] + abits[1] + abits[2]) / 3);
				if (invert)
				{
					bits[3] = GLubyte(255 - avg);
				}
				else
				{
					bits[3] = avg;
				}

				bbits += 3;
				abits += 3;
				bits += 4;
			}
		}
	}
}

GLBitmap::~GLBitmap()
{
	clear();
}

void GLBitmap::resize(int newWidth, int newHeight)
{
	if (!newbits_) return;

	if (width_ == newWidth &&
		height_ == newHeight)
	{
		return;
	}

	GLubyte *oldbits = newbits_;
	int oldWidth = width_;
	int oldHeight = height_;
	newbits_ = 0;

	createBlank(newWidth, newHeight, alpha_);
	gluScaleImage(
		(alpha_?GL_RGBA:GL_RGB), 
		oldWidth, oldHeight,
		GL_UNSIGNED_BYTE, oldbits,
		newWidth, newHeight, 
		GL_UNSIGNED_BYTE, newbits_);

	delete [] oldbits;
}

void GLBitmap::createBlank(int width, int height, bool alpha)
{
	clear();
	width_ = width;
	height_ = height;
	alpha_ = alpha;
	int bitsize = getComponents() * width * height;

	newbits_ = new GLubyte[bitsize];
	memset(newbits_, 255, bitsize);
}

void GLBitmap::clear()
{
	delete [] newbits_;
	newbits_ = 0;
	width_ = 0;
	height_ = 0;
}

GLubyte *GLBitmap::getBits()
{
	return newbits_;
}

int GLBitmap::getWidth()
{
	return width_;
}

int GLBitmap::getHeight()
{
	return height_;
}

int GLBitmap::getAlignment()
{
	return 4;
}

int GLBitmap::getComponents()
{
	return alpha_?4:3;
}

bool GLBitmap::getAlpha()
{
	return alpha_;
}

bool GLBitmap::loadFromFile(char * filename, bool alpha)
{
	bool success = false;

	SDL_Surface *image = SDL_LoadBMP(filename);
	if (!image) return false;

	// Create the internal byte array
	createBlank(image->w, image->h, alpha);

	// Convert the returned bits from BGR to RGB
	// and flip the verticle scan lines
	GLubyte *from = (GLubyte *) image->pixels;
	for (int i=0; i<height_; i ++)
	{
		GLubyte *destRow = ((GLubyte *) newbits_) + ((height_ - i - 1) * (width_ * getComponents()));
		for (int j=0; j<width_; j++)
		{
			GLubyte *dest = destRow + (j * getComponents());

			dest[0] = from[2];
			dest[1] = from[1];
			dest[2] = from[0];
			if (alpha)
			{
				dest[3] = (GLubyte)(from[0]==0?0:255);
			}
			
			from+=3;
		}
	}
	
	SDL_FreeSurface(image);
	return success;
}

bool GLBitmap::writeToFile(char * filename)
{
	if (!newbits_) return false;

	GLubyte *brgbits = new GLubyte[width_ * height_ * 3];

	// Convert the returned bits from RGB to BGR
	// and flip the verticle scan lines
	GLubyte *from = (GLubyte *) newbits_;
	for (int i=0; i<height_; i ++)
	{
		GLubyte *destRow = ((GLubyte *) brgbits) + ((height_ - i - 1) * (width_ * 3));
		for (int j=0; j<width_; j++)
		{
			GLubyte *dest = destRow + (j * getComponents());

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
		SDL_SWSURFACE, width_, height_, 24, rmask, gmask, bmask, 0);
	memcpy(image->pixels, brgbits, width_ * height_ * 3);

	if (!image) return false;

	SDL_SaveBMP(image, filename);
    SDL_FreeSurface (image);

	delete [] brgbits;
	return true;
}

void GLBitmap::grabScreen()
{
	GLint		viewport[4];		/* Current viewport */
	glGetIntegerv(GL_VIEWPORT, viewport);
	createBlank(viewport[2], viewport[3], false);

	glFinish();				/* Finish all OpenGL commands */
	glPixelStorei(GL_PACK_ALIGNMENT, 4);	/* Force 4-byte alignment */
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

	glReadPixels(0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, newbits_);
}

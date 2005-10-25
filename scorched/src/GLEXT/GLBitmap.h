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

#if !defined(AFX_GLBITMAP_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_)
#define AFX_GLBITMAP_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_

#include <GLEXT/GLImage.h>

/**
A class that wraps the loading of windows .bmp image data into
a format that can be easily consumed by a GLTexture object.
The bitmap data is unpacked and is stored in either RGB or
RGBA format.  One ubyte per component.
No attempt is made to ensure the bitmap conforms to the
GLTexture specification (sizes == pow(2, x))
*/
class GLBitmap : public GLImage
{
public:
	/**
	Create an empty uninitialised bitmap.  The bitmap is created uninitialised
	so this call must be followed by a call to createBlank, loadFromFile
	or grabScreen before the bitmap contains any data.
	*/
	GLBitmap();
	/**
	Create an empty bitmap.  The bitmap is created with the given width
	and height.  The bitmap is created with all values nulled out (black).
	If alpha is true then the bitmap will have color 4 components (RGBA)
	otherwise 3 (RGB).
    */
	GLBitmap(int width, int height, bool alpha = false, unsigned char fill = 255);
	/**
	See: loadFromFile.
	*/
	GLBitmap(const char *filename, bool alpha = false);
	/**
	Create a bitmap from the contents of the given files.
	The first file specifies the color components of the bitmap.
	The second file specifies the alpha component of the bitmap 
	(its opacity).  With black being full opacity and white
	being min opacity.  The invert flag can be used to switch this
	to black having min opacity and white having max opacity.
	The bitmap is created in RGBA format.
	*/
	GLBitmap(const char *filename, const char *alphafilename, bool invert = true);
	virtual ~GLBitmap();

	/**
	Create an empty bitmap.  The bitmap is created with the given width
	and height.  The bitmap is created with all values nulled out (black).
	If alpha is true then the bitmap will have color 4 components (RGBA)
	otherwise 3 (RGB).
    */
	void createBlank(int width, int height, bool alpha = false, unsigned char fill = 255);
	/** 
	Resets any component values to zero in the current bitmap.
	Does not delete the internal storage.
	*/
	void clear();
	/**
	Scales the current bitmap to the new dimensions.
	*/
	void resize(int newWidth, int newHeight);

	/**
	Get access to the internal storage of the bitmap.
	The storage is an 1D array of unsigned bytes.
	For RGB mode bitmaps 3 bytes are used per pixel.
	For RGBA mode bitmaps 4 bytes are used per pixel.
	*/
	virtual GLubyte *getBits();
	/** 
	Get the width of this bitmap in pixels.
	*/
	virtual int getWidth();
	/**
	Get the height of this bitmap in pixels.
	*/
	virtual int getHeight();
	
	/**
	Returns the byte alignment for this bitmap (always 4).
	*/
	virtual int getAlignment();
	/**
	Returns the number of components being used to store
	the bitmap information.
	For RGB mode bitmaps 3 bytes/components are used per pixel.
	For RGBA mode bitmaps 4 bytes/components are used per pixel.	
	*/
	virtual int getComponents();
	/**
	Returns the alpha mode of the bitmap.
	True = RGBA, False = RGB.
	*/
	virtual bool getAlpha();

	/** 
	Multiplies the alpha channel my the given multiplier
	*/
	virtual void alphaMult(float mult);

	/**
	Overwrite the bitmap with the contents of the given file.
	If alpha is true then any black areas in the bitmap have full
	opacity.  When creating an alpha bitmap the bitmap is created
	in RGBA format, otherwise in RGB format.
	*/
	bool loadFromFile(const char *filename, bool alpha = false);
	/**
	Saves the current bitmap to a file.
	Files are saved in windows BMP format.
	Note: Only RGB format bitmaps can be saved.
	*/
	bool writeToFile(const char *filename);
	/**
	Create a bitmap using the current framebuffer as the contents.
	The width and height of the bitmap will be the screen width/height.
	*/
	void grabScreen();

protected:
	GLubyte *newbits_;
	int width_;
	int height_;
	bool alpha_;

private:
	GLBitmap(GLBitmap &other);
	GLBitmap &operator=(GLBitmap &other);
};

#endif // !defined(AFX_GLBITMAP_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_)

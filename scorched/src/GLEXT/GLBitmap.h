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


// GLBitmap.h: interface for the GLBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLBITMAP_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_)
#define AFX_GLBITMAP_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_

#include <GLEXT/GLImage.h>

class GLBitmap : public GLImage
{
public:
	GLBitmap();
	GLBitmap(int width, int height, bool alpha = false);
	GLBitmap(char * filename, bool alpha = false);
	GLBitmap(char * filename, char * alphafilename, bool invert = true);
	virtual ~GLBitmap();

	void createBlank(int width, int height, bool alpha = false);
	void clear();
	void resize(int newWidth, int newHeight);

	virtual GLubyte *getBits();
	virtual int getWidth();
	virtual int getHeight();
	
	virtual int getAlignment();
	virtual int getComponents();
	virtual bool getAlpha();

	bool loadFromFile(char * filename, bool alpha);
	bool writeToFile(char * filename);
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

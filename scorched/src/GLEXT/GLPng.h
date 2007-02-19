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

#if !defined(AFX_GLPng_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_)
#define AFX_GLPng_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_

#include <GLEXT/GLImage.h>
#include <net/NetBuffer.h>

class GLPng : public GLImage
{
public:
	GLPng();
	GLPng(const char *filename, bool readalpha = false);
	GLPng(const char *filename, const char *alphafilename, bool invert = true);
	virtual ~GLPng();

	virtual unsigned char *getBits() { return bits_; }
	virtual int getWidth() { return width_; }
	virtual int getHeight() { return height_; }
	virtual int getAlignment() { return 4; }
	virtual int getComponents() { return alpha_?4:3; }

	void clear();
	void createBlank(int width, int height, bool alpha = false, unsigned char fill = 255);
	bool loadFromFile(const char *filename, bool readalpha = false);
	bool loadFromBuffer(NetBuffer &buffer, bool readalpha = false);

protected:
	unsigned char *bits_;
	int width_;
	int height_;
	bool alpha_;

private:
	GLPng(GLPng &other);
	GLPng &operator=(GLPng &other);
};

#endif // !defined(AFX_GLPng_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_)

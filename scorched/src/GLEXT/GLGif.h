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

#if !defined(AFX_GLGIF_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_)
#define AFX_GLGIF_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_

#include <GLEXT/GLImage.h>

class GLGif : public GLImage
{
public:
	GLGif();
	GLGif(const char *filename);
	virtual ~GLGif();

	virtual GLubyte *getBits() { return bits_; }
	virtual int getWidth() { return width_; }
	virtual int getHeight() { return height_; }
	virtual int getAlignment() { return 4; }
	virtual int getComponents() { return 3; }

	bool loadFromFile(const char *filename);

protected:
	GLubyte *bits_;
	int width_;
	int height_;

private:
	GLGif(GLGif &other);
	GLGif &operator=(GLGif &other);
};

#endif // !defined(AFX_GLGif_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_)

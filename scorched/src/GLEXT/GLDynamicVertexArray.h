////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#if !defined(__INCLUDE_GLDynamicVertexArrayh_INCLUDE__)
#define __INCLUDE_GLDynamicVertexArrayh_INCLUDE__

#include <GLEXT/GLState.h>
#include <GLEXT/GLStateExtension.h>

class GLDynamicVertexArray
{
public:
	static GLDynamicVertexArray *instance();

	int getSpace() { return capacity_ - used_; }
	int getUsed() { return used_; }

	inline void addFloat(GLfloat floats)
	{
		DIALOG_ASSERT(used_ < capacity_);

		GLfloat *mem = array_;
		if (used_ == 0 && vbo_ != 0)
		{
			mem = (GLfloat *)
				GLStateExtension::glMapBufferARB()(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
		}

		mem[used_] = floats;
		used_++;
	}
	void drawROAM();
	void drawQuadStrip(bool useColor);

protected:
	static GLDynamicVertexArray *instance_;
	GLfloat *array_;
	unsigned int vbo_;
	int used_;
	int capacity_;

private:
	GLDynamicVertexArray();
	virtual ~GLDynamicVertexArray();
};

#endif // __INCLUDE_GLDynamicVertexArrayh_INCLUDE__

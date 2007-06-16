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

#include <common/Vector.h>
#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLInfo.h>
#include <landscape/LargeHemisphere.h>
#include <landscape/Hemisphere.h>

LargeHemisphere::LargeHemisphere()
{
}

LargeHemisphere::~LargeHemisphere()
{
}

void LargeHemisphere::clear()
{
	while (!entries_.empty())
	{
		Entry entry = entries_.back();
		entries_.pop_back();
		glDeleteLists(entry.listNo_, 1);
	}
}

void LargeHemisphere::draw(float radius, float radius2, 
	unsigned int flags)
{
	if (entries_.empty())
	{
		for (int i=0; i<10; i+=2)
		{
			for (int j=0; j<10; j+=2)
			{
				Entry entry;
				glNewList(entry.listNo_ = glGenLists(1), GL_COMPILE_AND_EXECUTE);
					Hemisphere::draw(radius, radius2, 10, 10, i, j, i+2, j+2, 
						false, flags);
				glEndList();
				entries_.push_back(entry);
			}
		}
	}
	else
	{
		std::list<Entry>::iterator itor;
		for (itor = entries_.begin();
			itor != entries_.end();
			itor++)
		{
			// Should check for visibility here, but it seems so quick anyway
			Entry &entry = *itor;
			glCallList(entry.listNo_);
			GLInfo::addNoTriangles(8);
		}
	}
}

void LargeHemisphere::drawColored(float radius, float radius2, 
	GLImage &colors, Vector &sunDir, int daytime)
{
	if (entries_.empty())
	{
		for (int i=0; i<10; i+=2)
		{
			for (int j=0; j<10; j+=2)
			{
				Entry entry;
				glNewList(entry.listNo_ = glGenLists(1), GL_COMPILE_AND_EXECUTE);
					Hemisphere::drawColored(radius, radius2, 10, 10, i, j, i+2, j+2, 
						false, colors, sunDir, daytime);
				glEndList();
				entries_.push_back(entry);
			}
		}
	}
	else
	{
		std::list<Entry>::iterator itor;
		for (itor = entries_.begin();
			itor != entries_.end();
			itor++)
		{
			Entry &entry = *itor;
			glCallList(entry.listNo_);
			GLInfo::addNoTriangles(8);
		}
	}
}

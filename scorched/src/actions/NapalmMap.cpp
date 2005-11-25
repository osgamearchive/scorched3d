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

#include <actions/NapalmMap.h>
#include <memory.h>

NapalmMap::NapalmMap() : entries_(0)
{
}

NapalmMap::~NapalmMap()
{
	delete [] entries_;
	entries_ = 0;
}

float &NapalmMap::getNapalmHeight(int w, int h) 
{
	if (w >= 0 && h >= 0 && w<=width_ && h<=height_)
	{
        return entries_[(width_+1) * h + w]; 
	}

	static float tmp;
	tmp = 1000.0f;
	return tmp;
}

void NapalmMap::create(int width, int height)
{
	width_ = width;
    height_ = height;
	delete [] entries_;

	entries_ = new float[(width_ + 1) * (height_ + 1)];
	clear();
}

void NapalmMap::clear()
{
	memset(entries_, 0, sizeof(float) * (width_ + 1) * (height_ + 1));
}
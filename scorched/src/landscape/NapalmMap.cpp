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

#include <landscape/NapalmMap.h>
#include <GLEXT/GLState.h>
#include <memory.h>

NapalmMap::NapalmMap(int width) : width_(width)
{
	entries_ = new float[(width_ + 1) * (width_ + 1)];
	clear();
}

NapalmMap::~NapalmMap()
{
	delete [] entries_;
	entries_ = 0;
}

void NapalmMap::clear()
{
	memset(entries_, 0, sizeof(float) * (width_ + 1) * (width_ + 1));
}

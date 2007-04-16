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

#include <GLEXT/GLImageHandle.h>

GLImageHandle::GLImageHandle() :
	owner_(true),
	width_(0), height_(0),
	alignment_(0), components_(0),
	bits_(0)
{
}

GLImageHandle::GLImageHandle(const GLImage &other) :
	owner_(true),
	width_(((GLImage &)other).getWidth()), 
	height_(((GLImage &)other).getHeight()),
	alignment_(((GLImage &)other).getAlignment()),
	components_(((GLImage &)other).getComponents()),
	bits_(((GLImage &)other).getBits())
{
	((GLImage &)other).removeOwnership();
}

GLImageHandle::GLImageHandle(const GLImageHandle &other) :
	owner_(true),
	width_(((GLImageHandle&)other).getWidth()), 
	height_(((GLImageHandle&)other).getHeight()),
	alignment_(((GLImageHandle&)other).getAlignment()),
	components_(((GLImageHandle&)other).getComponents()),
	bits_(((GLImageHandle&)other).getBits())
{
	((GLImageHandle&)other).removeOwnership();
}

GLImageHandle::~GLImageHandle()
{
	if (owner_) delete [] bits_;
}

GLImageHandle &GLImageHandle::operator=(GLImageHandle &other)
{
	if (owner_) delete [] bits_;

	owner_ = true;
	width_ = other.getWidth();
	height_ = other.getHeight();
	alignment_ = other.getAlignment();
	components_ = other.getComponents();
	bits_ = other.getBits();

	other.removeOwnership();

	return *this;
}

GLImageHandle &GLImageHandle::operator=(GLImage &other)
{
	if (owner_) delete [] bits_;

	owner_ = true;
	width_ = other.getWidth();
	height_ = other.getHeight();
	alignment_ = other.getAlignment();
	components_ = other.getComponents();
	bits_ = other.getBits();

	other.removeOwnership();

	return *this;
}

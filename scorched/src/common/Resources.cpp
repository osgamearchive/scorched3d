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


// Resources.cpp: implementation of the Timer class.
//
//////////////////////////////////////////////////////////////////////

#include <common/Resources.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Resources *Resources::instance_ = 0;

Resources *Resources::instance()
{
	if (!instance_)
	{
		instance_ = new Resources;
	}
	return instance_;
}

Resources::Resources()
{
}

Resources::~Resources()
{
}

Vector *Resources::vectorResource(const char *name)
{
	return (Vector *) instance_->main.getVectorResource(name);
}

char *Resources::stringResource(const char *name)
{
	return (char *) instance_->main.getStringResource(name);
}

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

#include <landscape/LandscapeObjectsSetEntry.h>
#include <common/Defines.h>

LandscapeObjectsSetEntry::LandscapeObjectsSetEntry()
{
}

LandscapeObjectsSetEntry::~LandscapeObjectsSetEntry()
{
}

void LandscapeObjectsSetEntry::addObject(LandscapeObjectEntryBase *object)
{
	objects_.insert(object);
}

bool LandscapeObjectsSetEntry::removeObject(LandscapeObjectEntryBase *object)
{
	return (objects_.erase(object) > 0);
}

LandscapeObjectEntryBase *LandscapeObjectsSetEntry::getRandomObject()
{
	int pos = rand() % objects_.size();

	std::set<LandscapeObjectEntryBase *>::iterator itor;
	for (itor = objects_.begin();
		itor != objects_.end();
		itor ++, pos--)
	{
		if (pos <=0) return (*itor);
	}
	return 0;
}

int LandscapeObjectsSetEntry::getObjectCount()
{ 
	return (int) objects_.size(); 
}

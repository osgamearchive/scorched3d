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

#include <target/TargetContainer.h>

TargetContainer::TargetContainer()
{
}

TargetContainer::~TargetContainer()
{
}

void TargetContainer::addTarget(Target *target)
{
	targets_[target->getPlayerId()] = target;
}

Target *TargetContainer::removeTarget(unsigned int playerId)
{
    std::map<unsigned int, Target *>::iterator itor =
		targets_.find(playerId);
	if (itor != targets_.end())
	{
		Target *current = (*itor).second;
		targets_.erase(itor);
		return current;		
	}
	return 0;
}

Target *TargetContainer::getTargetById(unsigned int id)
{
	std::map<unsigned int, Target *>::iterator mainitor =
		targets_.find(id);
	if (mainitor != targets_.end())
	{
		return (*mainitor).second;
	}
	return 0;
}

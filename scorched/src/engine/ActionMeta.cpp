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


#include <engine/ActionMeta.h>
#include <common/Defines.h>

ActionMeta::ActionMeta()
{
}

ActionMeta::~ActionMeta()
{
}

std::map<std::string, ActionMeta *> *ActionMetaRegistration::actionMap = 0;

void ActionMetaRegistration::addMap(const char *name, ActionMeta *action)
{
	if (!actionMap) actionMap = new std::map<std::string, ActionMeta *>;

	std::map<std::string, ActionMeta *>::iterator itor = 
		actionMap->find(name);
	DIALOG_ASSERT(itor == actionMap->end());

	(*actionMap)[name] = action;
}

ActionMeta *ActionMetaRegistration::getNewAction(const char *name)
{
	std::map<std::string, ActionMeta *>::iterator itor = 
		actionMap->find(name);
	if (itor == actionMap->end()) return 0;
	return (*itor).second->getActionCopy();
}


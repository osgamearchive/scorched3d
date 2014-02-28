////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2013
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
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#if !defined(__INCLUDE_TargetDefinitionh_INCLUDE__)
#define __INCLUDE_TargetDefinitionh_INCLUDE__

#include <XML/XMLEntryComplexTypes.h>
#include <common/RandomGenerator.h>
#include <engine/ScorchedContext.h>
#include <engine/ObjectGroupEntryDefinition.h>

class Target;
class TargetDefinition : public XMLEntryContainer
{
public:
	TargetDefinition();
	virtual ~TargetDefinition();

	Target *createTarget(unsigned int playerId,
		FixedVector &position,
		FixedVector &velocity,
		ScorchedContext &context,
		RandomGenerator &generator);

	ObjectGroupEntryDefinition &getGroups() { return groups_; }

protected:
	XMLEntryFixed life_;
	XMLEntryBool boundingsphere_;
	XMLEntryBool driveovertodestroy_, flattendestroy_;
	XMLEntryBool nocollision_, nodamageburn_, nofalling_;
	XMLEntryBool displaydamage_, displayshadow_;
	XMLEntryBool nofallingdamage_;
	XMLEntryBool useNormalMoves_;
	XMLEntryBool billboard_;
	XMLEntryString name_;
	XMLEntryString parachute_;
	XMLEntryString shield_;
	XMLEntryString ainame_;
	XMLEntryInt team_;
	XMLEntryFixed border_;
	XMLEntryString removeaction_;
	XMLEntryString burnaction_;
	XMLEntryString collisionaction_;
	XMLEntryModelID modelId_;
	ObjectGroupEntryDefinition groups_;
};

#endif // __INCLUDE_TargetDefinitionh_INCLUDE__

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

#if !defined(__INCLUDE_PlacementObjectTreeh_INCLUDE__)
#define __INCLUDE_PlacementObjectTreeh_INCLUDE__

#include <placement/PlacementObject.h>
#include <placement/PlacementShadowDefinition.h>
#include <placement/PlacementGroupDefinition.h>
#include <string>

class PlacementObjectTree : public PlacementObject
{
public:
	PlacementObjectTree();
	virtual ~PlacementObjectTree();

	enum TreeType
	{
		eNone,
		ePineNormal,
		ePineBurnt,
		ePineYellow,
		ePineLight,
		ePineSnow,
		ePine2,
		ePine3,
		ePine4,
		ePine2Snow,
		ePine3Snow,
		ePine4Snow,
		ePalmNormal,
		ePalmBurnt,
		ePalm2,
		ePalm3,
		ePalm4,
		ePalmB,
		ePalmB2,
		ePalmB3,
		ePalmB4,
		ePalmB5,
		ePalmB6,
		ePalmB7,
		eOak,
		eOak2,
		eOak3,
		eOak4
	};

	virtual bool readXML(XMLNode *node);
	virtual PlacementObject::Type getType() { return PlacementObject::eTree; }
	virtual void createObject(ScorchedContext &context,
		RandomGenerator &generator,
		unsigned int &playerId,
		PlacementType::Position &position);

protected:
	PlacementShadowDefinition shadow_;
	PlacementGroupDefinition group_;
	std::string tree_;
	std::string burnaction_;
	std::string removeaction_;
	float snow_;
	float border_;

	bool getTypes(const char *type, bool snow, 
		TreeType &normalType, TreeType &burntType);
};

#endif // __INCLUDE_PlacementObjectTreeh_INCLUDE__

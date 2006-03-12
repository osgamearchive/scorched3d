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

#if !defined(__INCLUDE_PlacementObjectModelh_INCLUDE__)
#define __INCLUDE_PlacementObjectModelh_INCLUDE__

#include <placement/PlacementObject.h>
#include <3dsparse/Model.h>
#include <3dsparse/ModelID.h>

class PlacementObjectModel : public PlacementObject
{
public:
	PlacementObjectModel();
	virtual ~PlacementObjectModel();

	virtual bool readXML(XMLNode *node);
	virtual PlacementObject::Type getType() { return PlacementObject::eModel; }
	virtual void createObject(ScorchedContext &context,
		RandomGenerator &generator,
		unsigned int &playerId,
		PlacementType::Information &information,
		PlacementType::Position &position);

protected:
	std::string burnaction;
	std::string removeaction;
	ModelID modelId;
	ModelID modelburntId;
	Model *model;
	Model *modelburnt;
	float modelsize;
};

#endif // __INCLUDE_PlacementObjectModelh_INCLUDE__

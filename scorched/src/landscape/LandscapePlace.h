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

#if !defined(__INCLUDE_LandscapePlaceh_INCLUDE__)
#define __INCLUDE_LandscapePlaceh_INCLUDE__

#include <3dsparse/ModelID.h>
#include <XML/XMLFile.h>
#include <string>
#include <vector>

class LandscapePlaceType
{
public:
	virtual bool readXML(XMLNode *node) = 0;
};

class LandscapePlaceObjectsModel : public LandscapePlaceType
{
public:
	ModelID model;
	ModelID modelburnt;

	virtual bool readXML(XMLNode *node);
};

class LandscapePlaceObjectsTree : public LandscapePlaceType
{
public:
	std::string tree;
	float snow;

	virtual bool readXML(XMLNode *node);
};

class LandscapePlaceObjectsPlacement : public LandscapePlaceType
{
public:
	virtual ~LandscapePlaceObjectsPlacement();

	std::string removeaction;
	std::string burnaction;
	std::string objecttype;
	std::string groupname;
	LandscapePlaceType *object;

	virtual bool readXML(XMLNode *node);
};

class LandscapePlaceObjectsPlacementDirect : public LandscapePlaceObjectsPlacement
{
public:
	struct Position
	{
		Vector position;
		float rotation;
		float size;
	};

	std::list<Position> positions;

	virtual bool readXML(XMLNode *node);
};

class LandscapePlaceObjectsPlacementTree : public LandscapePlaceObjectsPlacement
{
public:
	int numobjects;
	int numclusters;
	float minheight, maxheight;

	virtual bool readXML(XMLNode *node);
};

class LandscapePlaceObjectsPlacementMask : public LandscapePlaceObjectsPlacement
{
public:
	int numobjects;
	std::string mask;
	float minheight, maxheight;
	float mincloseness, minslope;
	float xsnap, ysnap, angsnap;

	virtual bool readXML(XMLNode *node);
};

class LandscapePlace
{
public:
	LandscapePlace();
	virtual ~LandscapePlace();

	std::string name;

	std::vector<std::string> objectstype;
	std::vector<LandscapePlaceType *> objects;

	bool readXML(XMLNode *node);

private:
	LandscapePlace(const LandscapePlace &other);
	LandscapePlace &operator=(LandscapePlace &other);
};

#endif // __INCLUDE_LandscapePlaceh_INCLUDE__

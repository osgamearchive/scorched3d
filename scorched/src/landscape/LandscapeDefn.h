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

#if !defined(__INCLUDE_LandscapeDefnh_INCLUDE__)
#define __INCLUDE_LandscapeDefnh_INCLUDE__

#include <coms/ComsMessage.h>
#include <XML/XMLFile.h>
#include <string>
#include <vector>

class LandscapeDefinitions;
class LandscapePlace;
class LandscapeSound;
class LandscapeDefnType
{
public:
	enum DefnType
	{
		eNone,
		eStartHeight,
		eRoofCavern,
		eHeightMapFile,
		eHeightMapGenerate
	};

	virtual bool readXML(XMLNode *node) = 0;
	virtual DefnType getType() = 0;
};

class LandscapeDefnTypeNone : public LandscapeDefnType
{
public:
	virtual bool readXML(XMLNode *node);
	virtual DefnType getType() { return eNone; }
};

class LandscapeDefnStartHeight : public LandscapeDefnType
{
public:
	float startcloseness;
	float heightmin, heightmax;
	std::string startmask;

	virtual bool readXML(XMLNode *node);
	virtual DefnType getType() { return eStartHeight; }
};

class LandscapeDefnRoofCavern : public LandscapeDefnType
{
public:
	LandscapeDefnRoofCavern();
	virtual ~LandscapeDefnRoofCavern();

	float width;
	float height;
	LandscapeDefnType *heightmap;

	virtual bool readXML(XMLNode *node);
	virtual DefnType getType() { return eRoofCavern; }
};

class LandscapeDefnHeightMapFile : public LandscapeDefnType
{
public:
	std::string file;
	bool levelsurround;

	virtual bool readXML(XMLNode *node);
	virtual DefnType getType() { return eHeightMapFile; }
};

class LandscapeDefnHeightMapGenerate : public LandscapeDefnType
{
public:
	std::string mask;
	float landhillsmax, landhillsmin;
	float landheightmax, landheightmin;
	float landpeakwidthxmax, landpeakwidthxmin;
	float landpeakwidthymax, landpeakwidthymin;
	float landpeakheightmax, landpeakheightmin;
	float landsmoothing;
	bool levelsurround;

	virtual bool readXML(XMLNode *node);
	virtual DefnType getType() { return eHeightMapGenerate; }
};

class LandscapeDefn
{
public:
	LandscapeDefn();
	virtual ~LandscapeDefn();

	int minplayers;
	int maxplayers;
	int landscapewidth;
	int landscapeheight;

	LandscapeDefnType *roof;
	LandscapeDefnType *surround;
	LandscapeDefnType *tankstart;
	LandscapeDefnType *heightmap;

	std::vector<LandscapeSound *> sounds;
	std::vector<LandscapePlace *> placements;

	bool readXML(LandscapeDefinitions *definitions, XMLNode *node);

private:
	LandscapeDefn(const LandscapeDefn &other);
	LandscapeDefn &operator=(LandscapeDefn &other);

};

#endif

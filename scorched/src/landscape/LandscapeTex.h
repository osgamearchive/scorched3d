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

#if !defined(__INCLUDE_LandscapeTexh_INCLUDE__)
#define __INCLUDE_LandscapeTexh_INCLUDE__

#include <3dsparse/ModelID.h>
#include <XML/XMLFile.h>
#include <string>
#include <vector>

class ScorchedContext;
class LandscapeTexType
{
public:
	virtual bool readXML(XMLNode *node) = 0;
};

class LandscapeTexTypeNone : public LandscapeTexType
{
public:
	virtual bool readXML(XMLNode *node);
};

class LandscapeTexCondition : public LandscapeTexType
{
public:
	virtual float getNextEventTime() = 0;
};

class LandscapeTexConditionTime : public LandscapeTexCondition
{
public:
	float mintime;
	float maxtime;

	virtual float getNextEventTime();
	virtual bool readXML(XMLNode *node);
};

class LandscapeTexAction : public LandscapeTexType
{
public:
	virtual void fireAction(ScorchedContext &context) = 0;
};

class LandscapeTexActionFireWeapon : public LandscapeTexAction
{
public:
	std::string weapon;

	virtual void fireAction(ScorchedContext &context);
	virtual bool readXML(XMLNode *node);
};

class LandscapeTexEvent : public LandscapeTexType
{
public:
	virtual ~LandscapeTexEvent();

	std::string conditiontype;
	std::string actiontype;
	LandscapeTexCondition *condition;
	LandscapeTexAction *action;

	virtual bool readXML(XMLNode *node);
};

class LandscapeTexPrecipitation : public LandscapeTexType
{
public:
	int particles;

	virtual bool readXML(XMLNode *node);
};

class LandscapeTexBoids : public LandscapeTexType
{
public:
	ModelID model;
	int count;
	int minz, maxz;

	virtual bool readXML(XMLNode *node);
};

class LandscapeTexBorderWater : public LandscapeTexType
{
public:
	std::string reflection;
	std::string texture;
	std::string wavetexture1;
	std::string wavetexture2;
	float height;
	Vector wavecolor;

	virtual bool readXML(XMLNode *node);
};

class LandscapeTexTextureGenerate : public LandscapeTexType
{
public:
	std::string surround;
	std::string roof;
	std::string rockside;
	std::string shore;
	std::string texture0;
	std::string texture1;
	std::string texture2;
	std::string texture3;
	std::string texture4;

	virtual bool readXML(XMLNode *node);
};

class LandscapeDefinitions;
class LandscapeTex
{
public:
	LandscapeTex();
	virtual ~LandscapeTex();

	std::string name;
	std::string detail;
	std::string magmasmall;
	std::string scorch;
	std::vector<std::string> placements;
	Vector fog;
	Vector suncolor;
	float fogdensity;
	float lowestlandheight;
	std::string skytexture;
	std::string skytexturemask;
	std::string skycolormap;
	int skytimeofday;
	float skysunxy;
	float skysunyz;
	Vector skydiffuse;
	Vector skyambience;

	std::string bordertype;
	LandscapeTexType *border;
	std::string texturetype;
	LandscapeTexType *texture;
	std::string precipitationtype;
	LandscapeTexType *precipitation;

	std::vector<LandscapeTexEvent *> events;
	std::vector<LandscapeTexBoids *> boids;

	bool readXML(LandscapeDefinitions *definitions, XMLNode *node);

private:
	LandscapeTex(const LandscapeTex &other);
	LandscapeTex &operator=(LandscapeTex &other);
};

#endif

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

#include <coms/ComsMessage.h>
#include <3dsparse/ModelID.h>
#include <XML/XMLFile.h>
#include <string>
#include <vector>

class LandscapeTexType
{
public:
	virtual bool writeMessage(NetBuffer &buffer) = 0;
	virtual bool readMessage(NetBufferReader &reader) = 0;
	virtual bool readXML(XMLNode *node) = 0;
};

class LandscapeTexTypeNone : public LandscapeTexType
{
public:
	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);
	virtual bool readXML(XMLNode *node);
};

class LandscapeTexObjectsModel : public LandscapeTexType
{
public:
	ModelID model;
	ModelID modelburnt;

	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);
	virtual bool readXML(XMLNode *node);
};

class LandscapeTexObjectsTree : public LandscapeTexType
{
public:
	std::string tree;
	float snow;

	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);
	virtual bool readXML(XMLNode *node);
};

class LandscapeTexObjectsPlacement : public LandscapeTexType
{
public:
	virtual ~LandscapeTexObjectsPlacement();

	std::string objecttype;
	LandscapeTexType *object;

	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);
	virtual bool readXML(XMLNode *node);
};

class LandscapeTexObjectsPlacementTree : public LandscapeTexObjectsPlacement
{
public:
	int numobjects;
	int numclusters;
	float minheight, maxheight;

	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);
	virtual bool readXML(XMLNode *node);
};

class LandscapeTexObjectsPlacementMask : public LandscapeTexObjectsPlacement
{
public:
	int numobjects;
	std::string mask;
	float minheight, maxheight;
	float mincloseness, minslope;
	float xsnap, ysnap, angsnap;

	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);
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

	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);
	virtual bool readXML(XMLNode *node);
};

class LandscapeTexTextureGenerate : public LandscapeTexType
{
public:
	std::string roof;
	std::string rockside;
	std::string shore;
	std::string texture0;
	std::string texture1;
	std::string texture2;
	std::string texture3;
	std::string texture4;

	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);
	virtual bool readXML(XMLNode *node);
};

class LandscapeTex
{
public:
	LandscapeTex();
	virtual ~LandscapeTex();

	std::string name;
	std::string detail;
	std::string magmasmall;
	std::string scorch;
	Vector fog;
	float lowestlandheight;
	std::string skytexture;
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

	std::vector<std::string> objectstype;
	std::vector<LandscapeTexType *> objects;

	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);
	bool readXML(XMLNode *node);

private:
	LandscapeTex(const LandscapeTex &other);
	LandscapeTex &operator=(LandscapeTex &other);
};

#endif

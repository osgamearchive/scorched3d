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

class LandscapeDefnType
{
public:
	virtual bool readXML(XMLNode *node) = 0;
	virtual bool writeMessage(NetBuffer &buffer) = 0;
	virtual bool readMessage(NetBufferReader &reader) = 0;
};

class LandscapeDefnStartHeight: public LandscapeDefnType
{
public:
	float startcloseness;
	float heightmin, heightmax;

	virtual bool readXML(XMLNode *node);
	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);
};

class LandscapeDefnHeightMapFile : public LandscapeDefnType
{
public:
	std::string file;

	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);
	virtual bool readXML(XMLNode *node);
};

class LandscapeDefnHeightMapGenerate : public LandscapeDefnType
{
public:
	std::string mask;
	float landhillsmax, landhillsmin;
	float landheightmax, landheightmin;
	float landwidthx, landwidthy;
	float landpeakwidthxmax, landpeakwidthxmin;
	float landpeakwidthymax, landpeakwidthymin;
	float landpeakheightmax, landpeakheightmin;
	float landsmoothing;

	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);
	virtual bool readXML(XMLNode *node);
};

class LandscapeDefn
{
public:
	LandscapeDefn();
	virtual ~LandscapeDefn();

	std::string name;
	int minplayers;
	int maxplayers;

	std::string tankstarttype;
	LandscapeDefnType *tankstart;
	std::string heightmaptype;
	LandscapeDefnType *heightmap;

	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);
	bool readXML(XMLNode *node);

private:
	LandscapeDefn(const LandscapeDefn &other);
	LandscapeDefn &operator=(LandscapeDefn &other);

};

#endif

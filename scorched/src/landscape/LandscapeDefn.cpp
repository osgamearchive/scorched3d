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

#include <landscape/LandscapeDefn.h>
#include <common/Defines.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static LandscapeDefnType *fetchTankStartDefnType(const char *type)
{
	if (0 == strcmp(type, "height")) return new LandscapeDefnStartHeight;
	dialogMessage("LandscapeDefnType", "Unknown tankstart type %s", type);
	return 0;
}

static LandscapeDefnType *fetchRoofHeightMapDefnType(const char *type)
{
	if (0 == strcmp(type, "none")) return new LandscapeDefnTypeNone;
	if (0 == strcmp(type, "generate")) return new LandscapeDefnHeightMapGenerate;
	if (0 == strcmp(type, "file")) return new LandscapeDefnHeightMapFile;
	dialogMessage("LandscapeDefnType", "Unknown heightmap type %s", type);
	return 0;
}

static LandscapeDefnType *fetchSurroundDefnType(const char *type)
{
	if (0 == strcmp(type, "none")) return new LandscapeDefnTypeNone;
	dialogMessage("LandscapeDefnType", "Unknown surround type %s", type);
	return 0;
}

static bool parseMinMax(XMLNode *parent, const char *name, 
	float &min, float &max)
{
	XMLNode *node = 0;
	if (!parent->getNamedChild(name, node)) return false;
	if (!node->getNamedChild("max", max)) return false;
	if (!node->getNamedChild("min", min)) return false;
	return node->failChildren();
}

bool LandscapeDefnTypeNone::writeMessage(NetBuffer &buffer)
{
	return true;
}

bool LandscapeDefnTypeNone::readMessage(NetBufferReader &reader)
{
	return true;
}

bool LandscapeDefnTypeNone::readXML(XMLNode *node)
{
	return node->failChildren();
}

bool LandscapeDefnStartHeight::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(startcloseness);
	buffer.addToBuffer(heightmin);
	buffer.addToBuffer(heightmax);
	buffer.addToBuffer(startmask);
	return true;
}

bool LandscapeDefnStartHeight::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(startcloseness)) return false;
	if (!reader.getFromBuffer(heightmin)) return false;
	if (!reader.getFromBuffer(heightmax)) return false;
	if (!reader.getFromBuffer(startmask)) return false;
	return true;
}

bool LandscapeDefnStartHeight::readXML(XMLNode *node)
{
	if (!node->getNamedChild("startcloseness", 
		startcloseness)) return false;
	if (!parseMinMax(node, "height", 
		heightmin, heightmax)) return false;
	if (!node->getNamedChild("startmask", 
		startmask)) return false;
	if (!startmask.empty())
	{
		if (!checkDataFile(startmask.c_str())) return false;
	}
	return node->failChildren();
}

bool LandscapeDefnHeightMapFile::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(file);
	buffer.addToBuffer(levelsurround);
	return true;
}

bool LandscapeDefnHeightMapFile::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(file)) return false;
	if (!reader.getFromBuffer(levelsurround)) return false;
	return true;
}

bool LandscapeDefnHeightMapFile::readXML(XMLNode *node)
{
	if (!node->getNamedChild("file", file)) return false;
	if (!node->getNamedChild("levelsurround", levelsurround)) return false;
	if (!checkDataFile(file.c_str())) return false;
	return node->failChildren();
}

bool LandscapeDefnHeightMapGenerate::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(mask);
	buffer.addToBuffer(landhillsmax);
	buffer.addToBuffer(landhillsmin);
	buffer.addToBuffer(landheightmax);
	buffer.addToBuffer(landheightmin);
	buffer.addToBuffer(landwidthx);
	buffer.addToBuffer(landwidthy);
	buffer.addToBuffer(landpeakwidthxmax);
	buffer.addToBuffer(landpeakwidthxmin);
	buffer.addToBuffer(landpeakwidthymax);
	buffer.addToBuffer(landpeakwidthymin);
	buffer.addToBuffer(landpeakheightmax);
	buffer.addToBuffer(landpeakheightmin);
	buffer.addToBuffer(landsmoothing);
	buffer.addToBuffer(levelsurround);
	return true;
}

bool LandscapeDefnHeightMapGenerate::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(mask)) return false;
	if (!reader.getFromBuffer(landhillsmax)) return false;
	if (!reader.getFromBuffer(landhillsmin)) return false;
	if (!reader.getFromBuffer(landheightmax)) return false;
	if (!reader.getFromBuffer(landheightmin)) return false;
	if (!reader.getFromBuffer(landwidthx)) return false;
	if (!reader.getFromBuffer(landwidthy)) return false;
	if (!reader.getFromBuffer(landpeakwidthxmax)) return false;
	if (!reader.getFromBuffer(landpeakwidthxmin)) return false;
	if (!reader.getFromBuffer(landpeakwidthymax)) return false;
	if (!reader.getFromBuffer(landpeakwidthymin)) return false;
	if (!reader.getFromBuffer(landpeakheightmax)) return false;
	if (!reader.getFromBuffer(landpeakheightmin)) return false;
	if (!reader.getFromBuffer(landsmoothing)) return false;
	if (!reader.getFromBuffer(levelsurround)) return false;
	return true;
}

bool LandscapeDefnHeightMapGenerate::readXML(XMLNode *node)
{
	if (!node->getNamedChild("mask", mask)) return false;
	if (!parseMinMax(node, "landhills", 
		landhillsmin, landhillsmax)) return false;
	if (!parseMinMax(node, "landheight", 
		landheightmin, landheightmax)) return false;
	if (!node->getNamedChild("landwidthx", landwidthx)) return false;
	if (!node->getNamedChild("landwidthy", landwidthy)) return false;
	if (!parseMinMax(node, "landpeakwidthx", 
		landpeakwidthxmin, landpeakwidthxmax)) return false;
	if (!parseMinMax(node, "landpeakwidthy", 
		landpeakwidthymin, landpeakwidthymax)) return false;
	if (!parseMinMax(node, "landpeakheight", 
		landpeakheightmin, landpeakheightmax)) return false;
	if (!node->getNamedChild("landsmoothing", landsmoothing)) return false;
	if (!node->getNamedChild("levelsurround", levelsurround)) return false;

	if (!mask.empty())
	{
		if (!checkDataFile(mask.c_str())) return false;
	}
	return node->failChildren();
}

LandscapeDefn::LandscapeDefn() :
	heightmap(0), tankstart(0), surround(0), roof(0)
{
}

LandscapeDefn::~LandscapeDefn()
{
}

bool LandscapeDefn::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(name);
	buffer.addToBuffer(minplayers);
	buffer.addToBuffer(maxplayers);
	buffer.addToBuffer(tankstarttype);
	if (!tankstart->writeMessage(buffer)) return false;
	buffer.addToBuffer(heightmaptype);
	if (!heightmap->writeMessage(buffer)) return false;
	buffer.addToBuffer(rooftype);
	if (!roof->writeMessage(buffer)) return false;
	buffer.addToBuffer(surroundtype);
	if (!surround->writeMessage(buffer)) return false;
	return true;
}

bool LandscapeDefn::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(name)) return false;
	if (!reader.getFromBuffer(minplayers)) return false;
	if (!reader.getFromBuffer(maxplayers)) return false;
	
	if (!reader.getFromBuffer(tankstarttype)) return false;
	if (!(tankstart = fetchTankStartDefnType(tankstarttype.c_str()))) return false;
	if (!tankstart->readMessage(reader)) return false;
	
	if (!reader.getFromBuffer(heightmaptype)) return false;
	if (!(heightmap = fetchRoofHeightMapDefnType(heightmaptype.c_str()))) return false;
	if (!heightmap->readMessage(reader)) return false;
	
	if (!reader.getFromBuffer(rooftype)) return false;
	if (!(roof = fetchRoofHeightMapDefnType(rooftype.c_str()))) return false;
	if (!roof->readMessage(reader)) return false;
	
	if (!reader.getFromBuffer(surroundtype)) return false;
	if (!(surround = fetchSurroundDefnType(surroundtype.c_str()))) return false;
	if (!surround->readMessage(reader)) return false;
	
	return true;
}

bool LandscapeDefn::readXML(XMLNode *node)
{
	if (!node->getNamedChild("name", name)) return false;
	if (!node->getNamedChild("minplayers", minplayers)) return false;
	if (!node->getNamedChild("maxplayers", maxplayers)) return false;

	{
		XMLNode *startNode;
		if (!node->getNamedChild("tankstart", startNode)) return false;
		if (!startNode->getNamedParameter("type", tankstarttype)) return false;
		if (!(tankstart = fetchTankStartDefnType(tankstarttype.c_str()))) return false;
		if (!tankstart->readXML(startNode)) return false;
	}
	{
		XMLNode *heightNode;
		if (!node->getNamedChild("heightmap", heightNode)) return false;
		if (!heightNode->getNamedParameter("type", heightmaptype)) return false;
		if (!(heightmap = fetchRoofHeightMapDefnType(heightmaptype.c_str()))) return false;
		if (!heightmap->readXML(heightNode)) return false;
	}
	{
		XMLNode *surroundNode;
		if (!node->getNamedChild("surround", surroundNode)) return false;
		if (!surroundNode->getNamedParameter("type", surroundtype)) return false;
		if (!(surround = fetchSurroundDefnType(surroundtype.c_str()))) return false;
		if (!surround->readXML(surroundNode)) return false;
	}
	{
		XMLNode *roofNode;
		if (!node->getNamedChild("roof", roofNode)) return false;
		if (!roofNode->getNamedParameter("type", rooftype)) return false;
		if (!(roof = fetchRoofHeightMapDefnType(rooftype.c_str()))) return false;
		if (!roof->readXML(roofNode)) return false;
	}
	return node->failChildren();
}


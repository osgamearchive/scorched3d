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
#include <XML/XMLFile.h>
#include <string>
#include <vector>

class LandscapeTexType
{
public:
	std::string type;

	virtual bool readXML(XMLNode *node) = 0;
};

class LandscapeTexBorderWater : public LandscapeTexType
{
public:
	std::string reflection;
	std::string texture;
	std::string wavetexture1;
	std::string wavetexture2;

	virtual bool readXML(XMLNode *node);
};

class LandscapeTexTextureGenerate : public LandscapeTexType
{
public:
	std::string rockside;
	std::string shore;
	std::string texture0;
	std::string texture1;
	std::string texture2;
	std::string texture3;
	std::string texture4;

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
	Vector skyfog;
	std::string skytexture;
	std::string skycolormap;
	int skytimeofday;
	int skysunxy;
	int skysunyz;

	LandscapeTexType *border;
	LandscapeTexType *texture;

	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);
	bool readXML(XMLNode *node);

private:
	LandscapeTex(const LandscapeTex &other);
	LandscapeTex &operator=(LandscapeTex &other);
};

#endif

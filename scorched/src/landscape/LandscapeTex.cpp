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

#include <landscape/LandscapeTex.h>
#include <common/Defines.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static LandscapeTexType *fetchBorderTexType(const char *type)
{
	if (0 == strcmp(type, "water")) return new LandscapeTexBorderWater;
	dialogMessage("LandscapeTexType", "Unknown border type %s", type);
	return 0;
}

static LandscapeTexType *fetchTextureTexType(const char *type)
{
	if (0 == strcmp(type, "generate")) return new LandscapeTexTextureGenerate;
	dialogMessage("LandscapeTexType", "Unknown texture type %s", type);
	return 0;
}

bool LandscapeTexBorderWater::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(reflection);
	buffer.addToBuffer(texture);
	buffer.addToBuffer(wavetexture1);
	buffer.addToBuffer(wavetexture2);
	return true;
}

bool LandscapeTexBorderWater::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(reflection)) return false;
	if (!reader.getFromBuffer(texture)) return false;
	if (!reader.getFromBuffer(wavetexture1)) return false;
	if (!reader.getFromBuffer(wavetexture2)) return false;
	return true;
}

bool LandscapeTexBorderWater::readXML(XMLNode *node)
{
	if (!node->getNamedChild("reflection", reflection)) return false;
	if (!node->getNamedChild("texture", texture)) return false;
	if (!node->getNamedChild("wavetexture1", wavetexture1)) return false;
	if (!node->getNamedChild("wavetexture2", wavetexture2)) return false;
	if (!checkDataFile(reflection.c_str())) return false;
	if (!checkDataFile(texture.c_str())) return false;
	if (!checkDataFile(wavetexture1.c_str())) return false;
	if (!checkDataFile(wavetexture2.c_str())) return false;
	return node->failChildren();
}

bool LandscapeTexTextureGenerate::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(rockside);
	buffer.addToBuffer(shore);
	buffer.addToBuffer(texture0);
	buffer.addToBuffer(texture1);
	buffer.addToBuffer(texture2);
	buffer.addToBuffer(texture3);
	buffer.addToBuffer(texture4);
	return true;
}

bool LandscapeTexTextureGenerate::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(rockside)) return false;
	if (!reader.getFromBuffer(shore)) return false;
	if (!reader.getFromBuffer(texture0)) return false;
	if (!reader.getFromBuffer(texture1)) return false;
	if (!reader.getFromBuffer(texture2)) return false;
	if (!reader.getFromBuffer(texture3)) return false;
	if (!reader.getFromBuffer(texture4)) return false;
	return true;
}

bool LandscapeTexTextureGenerate::readXML(XMLNode *node)
{
	if (!node->getNamedChild("rockside", rockside)) return false;
	if (!node->getNamedChild("shore", shore)) return false;
	if (!node->getNamedChild("texture0", texture0)) return false;
	if (!node->getNamedChild("texture1", texture1)) return false;
	if (!node->getNamedChild("texture2", texture2)) return false;
	if (!node->getNamedChild("texture3", texture3)) return false;
	if (!node->getNamedChild("texture4", texture4)) return false;
	if (!checkDataFile(rockside.c_str())) return false;
	if (!checkDataFile(shore.c_str())) return false;
	if (!checkDataFile(texture0.c_str())) return false;
	if (!checkDataFile(texture1.c_str())) return false;
	if (!checkDataFile(texture2.c_str())) return false;
	if (!checkDataFile(texture3.c_str())) return false;
	if (!checkDataFile(texture4.c_str())) return false;
	return node->failChildren();
}

LandscapeTex::LandscapeTex() :
	border(0), texture(0)
{
}

LandscapeTex::~LandscapeTex()
{
	delete border;
	delete texture;
}

bool LandscapeTex::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(name);
	buffer.addToBuffer(detail);
	buffer.addToBuffer(magmasmall);
	buffer.addToBuffer(scorch);
	buffer.addToBuffer(fog);
	buffer.addToBuffer(skytexture);
	buffer.addToBuffer(skycolormap);
	buffer.addToBuffer(skytimeofday);
	buffer.addToBuffer(skysunxy);
	buffer.addToBuffer(skysunxy);
	buffer.addToBuffer(bordertype);
	if (!border->writeMessage(buffer)) return false;
	buffer.addToBuffer(texturetype);
	if (!texture->writeMessage(buffer)) return false;
	return true;
}

bool LandscapeTex::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(name)) return false;
	if (!reader.getFromBuffer(detail)) return false;
	if (!reader.getFromBuffer(magmasmall)) return false;
	if (!reader.getFromBuffer(scorch)) return false;
	if (!reader.getFromBuffer(fog)) return false;
	if (!reader.getFromBuffer(skytexture)) return false;
	if (!reader.getFromBuffer(skycolormap)) return false;
	if (!reader.getFromBuffer(skytimeofday)) return false;
	if (!reader.getFromBuffer(skysunxy)) return false;
	if (!reader.getFromBuffer(skysunyz)) return false;

	if (!reader.getFromBuffer(bordertype)) return false;
	if (!(border = fetchBorderTexType(bordertype.c_str()))) return false;
	if (!border->readMessage(reader)) return false;

	if (!reader.getFromBuffer(texturetype)) return false;
	if (!(texture = fetchTextureTexType(texturetype.c_str()))) return false;
	if (!texture->readMessage(reader)) return false;
	return true;
}

bool LandscapeTex::readXML(XMLNode *node)
{
	if (!node->getNamedChild("name", name)) return false;
	if (!node->getNamedChild("detail", detail)) return false;
	if (!node->getNamedChild("magmasmall", magmasmall)) return false;
	if (!node->getNamedChild("scorch", scorch)) return false;
	if (!node->getNamedChild("fog", fog)) return false;
	if (!node->getNamedChild("skytexture", skytexture)) return false;
	if (!node->getNamedChild("skycolormap", skycolormap)) return false;
	if (!node->getNamedChild("skytimeofday", skytimeofday)) return false;
	if (!node->getNamedChild("skysunxy", skysunxy)) return false;
	if (!node->getNamedChild("skysunyz", skysunyz)) return false;

	if (!checkDataFile(detail.c_str())) return false;
	if (!checkDataFile(magmasmall.c_str())) return false;
	if (!checkDataFile(scorch.c_str())) return false;
	if (!checkDataFile(skytexture.c_str())) return false;
	if (!checkDataFile(skycolormap.c_str())) return false;

	{
		XMLNode *borderNode;
		if (!node->getNamedChild("border", borderNode)) return false;
		if (!borderNode->getNamedParameter("type", bordertype)) return false;
		if (!(border = fetchBorderTexType(bordertype.c_str()))) return false;
		if (!border->readXML(borderNode)) return false;
	}
	{
		XMLNode *textureNode;
		if (!node->getNamedChild("texture", textureNode)) return false;
		if (!textureNode->getNamedParameter("type", texturetype)) return false;
		if (!(texture = fetchTextureTexType(texturetype.c_str()))) return false;
		if (!texture->readXML(textureNode)) return false;
	}
	return node->failChildren();
}


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

bool LandscapeTexBorderWater::readXML(XMLNode *node)
{
	if (!node->getNamedString("reflection", reflection)) return false;
	if (!node->getNamedString("texture", texture)) return false;
	if (!node->getNamedString("wavetexture1", wavetexture1)) return false;
	if (!node->getNamedString("wavetexture2", wavetexture2)) return false;

	return node->failChildren();
}

bool LandscapeTexTextureGenerate::readXML(XMLNode *node)
{
	if (!node->getNamedString("rockside", rockside)) return false;
	if (!node->getNamedString("shore", shore)) return false;
	if (!node->getNamedString("texture0", texture0)) return false;
	if (!node->getNamedString("texture1", texture1)) return false;
	if (!node->getNamedString("texture2", texture2)) return false;
	if (!node->getNamedString("texture3", texture3)) return false;
	if (!node->getNamedString("texture4", texture4)) return false;

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
	buffer.addToBuffer(skyfog);
	buffer.addToBuffer(skytexture);
	buffer.addToBuffer(skycolormap);
	buffer.addToBuffer(skytimeofday);
	buffer.addToBuffer(skysunxy);
	buffer.addToBuffer(skysunxy);
	return true;
}

bool LandscapeTex::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(name)) return false;
	if (!reader.getFromBuffer(detail)) return false;
	if (!reader.getFromBuffer(magmasmall)) return false;
	if (!reader.getFromBuffer(scorch)) return false;
	if (!reader.getFromBuffer(skyfog)) return false;
	if (!reader.getFromBuffer(skytexture)) return false;
	if (!reader.getFromBuffer(skycolormap)) return false;
	if (!reader.getFromBuffer(skytimeofday)) return false;
	if (!reader.getFromBuffer(skysunxy)) return false;
	if (!reader.getFromBuffer(skysunyz)) return false;
	return true;
}

bool LandscapeTex::readXML(XMLNode *node)
{
	if (!node->getNamedString("name", name)) return false;
	if (!node->getNamedString("detail", detail)) return false;
	if (!node->getNamedString("magmasmall", magmasmall)) return false;
	if (!node->getNamedString("scorch", scorch)) return false;
	if (!node->getNamedVector("skyfog", skyfog)) return false;
	if (!node->getNamedString("skytexture", skytexture)) return false;
	if (!node->getNamedString("skycolormap", skycolormap)) return false;
	if (!node->getNamedInt("skytimeofday", skytimeofday)) return false;
	if (!node->getNamedInt("skysunxy", skysunxy)) return false;
	if (!node->getNamedInt("skysunyz", skysunyz)) return false;

	XMLNode *borderNode;
	if (!node->getNamedChild("border", borderNode)) return false;
	border = new LandscapeTexBorderWater;
	if (!border->readXML(borderNode)) return false;

	XMLNode *textureNode;
	if (!node->getNamedChild("texture", textureNode)) return false;
	texture = new LandscapeTexTextureGenerate;
	if (!texture->readXML(textureNode)) return false;

	return node->failChildren();
}


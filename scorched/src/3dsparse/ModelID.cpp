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

#include <3dsparse/ModelID.h>
#include <common/Defines.h>
#include <coms/NetBuffer.h>
#include <XML/XMLParser.h>
#include <wx/utils.h>

ModelID::ModelID()
{
}

ModelID::ModelID(const ModelID &other) : 
	type_(other.type_),
	meshName_(other.meshName_),
	skinName_(other.skinName_)
{
}

ModelID::~ModelID()
{
}

ModelID &ModelID::operator=(const ModelID &other)
{
	type_ = other.type_;
	meshName_ = other.meshName_;
	skinName_ = other.skinName_;
	return *this;
}

bool ModelID::initFromString(
		const char *type,
		const char *meshName,
		const char *skinName)
{
	type_ = type;
	meshName_ = meshName;
	skinName_ = skinName;
	return true;
}


bool ModelID::initFromNode(const char *directory, XMLNode *modelNode)
{
	XMLNode *typeNode = 0;
	if (!modelNode->getNamedParameter("type", typeNode)) return false;

	// Store the model type
	type_ = typeNode->getContent();

	// Create the correct model depending on the model type
	if (strcmp(typeNode->getContent(), "ase") == 0)
	{
		// 3DS Studio ASCII Files
		XMLNode *meshNode, *skinNode;
		if (!modelNode->getNamedChild("mesh", meshNode)) return false;
		const char *meshNameContent = meshNode->getContent();
		static char meshName[1024];
		snprintf(meshName, 1024, "%s/%s", directory, meshNameContent);
		if (!s3d_fileExists(getDataFile(meshName)))
		{
			dialogMessage("Scorched Models", formatString(
						"Mesh file \"%s\"does not exist",
						meshName));
			return false;
		}

		if (!modelNode->getNamedChild("skin", skinNode)) return false;
		const char *skinNameContent = skinNode->getContent();
		static char skinName[1024];
		if (strcmp(skinNameContent, "none") != 0)
		{
			snprintf(skinName, 1024, "%s/%s", directory, skinNameContent);
			if (!s3d_fileExists(getDataFile(skinName)))
			{
				dialogMessage("Scorched Models", formatString(
							"Skin file \"%s\" does not exist",
							skinName));
				return false;
			}
		}
		else
		{
			snprintf(skinName, 1024, "%s", skinNameContent);
		}

		meshName_ = meshName;
		skinName_ = skinName;
	}
	else if (strcmp(typeNode->getContent(), "MilkShape") == 0)
	{
		const char *meshNameContent = modelNode->getContent();
		static char meshName[1024];
		snprintf(meshName, 1024, "%s/%s", directory, meshNameContent);

		if (!s3d_fileExists(getDataFile(meshName)))
		{
			dialogMessage("Scorched Models", formatString(
						"Mesh file \"%s\"does not exist",
						meshName));
			return false;
		}

		meshName_ = meshName;
	}
	else
	{
		dialogMessage("Scorched Models", formatString(
					"Unknown mesh type \"%s\"",
					typeNode->getContent()));
		return false;
	}

	return true;
}

bool ModelID::writeModelID(NetBuffer &buffer)
{
	buffer.addToBuffer(type_);
	buffer.addToBuffer(meshName_);
	buffer.addToBuffer(skinName_);
	return true;
}

bool ModelID::readModelID(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(type_)) return false;
	if (!reader.getFromBuffer(meshName_)) return false;
	if (!reader.getFromBuffer(skinName_)) return false;
	return true;	
}

const char *ModelID::getStringHash()
{ 
	hash_ = meshName_ + "-" + skinName_;
	return hash_.c_str(); 
}

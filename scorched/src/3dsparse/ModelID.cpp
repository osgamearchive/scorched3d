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
#include <3dsparse/MSFile.h>
#include <3dsparse/ASEFile.h>
#include <common/Defines.h>
#include <wx/utils.h>

ModelID::ModelID() : cachedFile_(0)
{
}

ModelID::ModelID(const ModelID &other) : 
	cachedFile_(0),
	type_(other.type_),
	meshName_(other.meshName_),
	skinName_(other.skinName_)
{

}

ModelID::~ModelID()
{
	delete cachedFile_;
	cachedFile_ = 0;
}

ModelID &ModelID::operator=(const ModelID &other)
{
	delete cachedFile_;
	cachedFile_ = 0;
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
	XMLNode *typeNode = modelNode->getNamedParameter("type");
	if (!typeNode)
	{
		dialogMessage("Scorched Models",
						"Failed to find type node");
		return false;
	}

	// Store the model type
	type_ = typeNode->getContent();

	// Create the correct model depending on the model type
	if (strcmp(typeNode->getContent(), "ase") == 0)
	{
		// 3DS Studio ASCII Files
		XMLNode *meshNode = modelNode->getNamedChild("mesh");
		XMLNode *skinNode = modelNode->getNamedChild("skin");
		if (!meshNode || !skinNode)
		{
			dialogMessage("Scorched Models",
						"Failed to find mesh or skin node");
			return false;
		}

		const char *skinNameContent = skinNode->getContent();
		static char skinName[1024];
		sprintf(skinName, "%s/%s", directory, skinNameContent);

		const char *meshNameContent = meshNode->getContent();
		static char meshName[1024];
		sprintf(meshName, "%s/%s", directory, meshNameContent);

		if (!::wxFileExists(getDataFile(skinName)))
		{
			dialogMessage("Scorched Models",
						"Skin file \"%s\" does not exist",
						skinName);
			return false;
		}
		if (!::wxFileExists(getDataFile(meshName)))
		{
			dialogMessage("Scorched Models",
						"Mesh file \"%s\"does not exist",
						meshName);
			return false;
		}

		meshName_ = meshName;
		skinName_ = skinName;
	}
	else if (strcmp(typeNode->getContent(), "MilkShape") == 0)
	{
		const char *meshNameContent = modelNode->getContent();
		static char meshName[1024];
		sprintf(meshName, "%s/%s", directory, meshNameContent);

		if (!::wxFileExists(getDataFile(meshName)))
		{
			dialogMessage("Scorched Models",
						"Mesh file \"%s\"does not exist",
						meshName);
			return false;
		}

		meshName_ = meshName;
	}
	else
	{
		dialogMessage("Scorched Models",
					"Unknown mesh type \"%s\"",
					typeNode->getContent());
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

ModelsFile *ModelID::getModelsFile()
{
	ModelsFile *newFile = cachedFile_;
	if (newFile == 0)
	{
		if (0 == strcmp(getType(), "ase"))
		{
			// Load the ASEFile containing the tank definitions
			std::string meshName(getDataFile(getMeshName()));
			newFile = new ASEFile(meshName.c_str(), getDataFile(getSkinName()));
			if (!newFile->getSuccess())
			{
				dialogMessage("ASE File", "Failed to load ASE file \"%s\"", getMeshName());
				return 0;
			}
		}
		else
		{
			// Load the Milkshape containing the tank definitions
			newFile = new MSFile(getDataFile(getMeshName()));
			if (!newFile->getSuccess())
			{
				dialogMessage("MS File", "Failed to load MS file \"%s\"", getMeshName());
				return 0;
			}			
		}
	}

	cachedFile_ = newFile;
	return newFile;
}

void ModelID::clearCachedFile()
{
	delete cachedFile_;
	cachedFile_ = 0;
}

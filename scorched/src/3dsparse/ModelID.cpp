#include <3dsparse/ModelID.h>
#include <3dsparse/MSFile.h>
#include <3dsparse/ASEFile.h>
#include <common/Defines.h>
#include <wx/utils.h>

ModelID::ModelID()
{
}

ModelID::~ModelID()
{
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

bool ModelID::initFromNode(XMLNode *modelNode)
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
		sprintf(skinName, PKGDIR "data/tanks/%s", skinNameContent);

		const char *meshNameContent = meshNode->getContent();
		static char meshName[1024];
		sprintf(meshName, PKGDIR "data/tanks/%s", meshNameContent);

		if (!::wxFileExists(skinName))
		{
			dialogMessage("Scorched Models",
						"Skin file \"%s\" does not exist",
						skinName);
			return false;
		}
		if (!::wxFileExists(meshName))
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
		sprintf(meshName, PKGDIR "data/tanks/%s", meshNameContent);

		if (!::wxFileExists(meshName))
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

ModelsFile *ModelID::getNewFile()
{
	ModelsFile *newFile = 0;
	if (0 == strcmp(getType(), "ase"))
	{
		// Load the ASEFile containing the tank definitions
		newFile = new ASEFile(getMeshName(), getSkinName());
		if (!newFile->getSuccess())
		{
			dialogMessage("ASE File", "Failed to load ASE file \"%s\"", getMeshName());
			return 0;
		}
	}
	else
	{
		// Load the Milkshape containing the tank definitions
		newFile = new MSFile(getMeshName());
		if (!newFile->getSuccess())
		{
			dialogMessage("MS File", "Failed to load MS file \"%s\"", getMeshName());
			return 0;
		}			
	}
	return newFile;
}

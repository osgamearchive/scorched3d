#pragma once

#include <XML/XMLFile.h>

class ModelsFile;
class ModelID
{
public:
	ModelID();
	virtual ~ModelID();

	bool initFromNode(XMLNode *modelNode);
	bool initFromString(
		const char *type,
		const char *meshName,
		const char *skinName);

	// Not very generic but it will do for now!!
	const char *getStringHash() { return meshName_.c_str(); }
	const char *getSkinName() { return skinName_.c_str(); }
	const char *getMeshName() { return meshName_.c_str(); }
	const char *getType() { return type_.c_str(); }

	ModelsFile *getNewFile();

protected:
	std::string type_;
	std::string meshName_;
	std::string skinName_;

};

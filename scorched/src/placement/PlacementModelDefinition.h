#pragma once

#include <3dsparse/ModelID.h>
#include <engine/ScorchedContext.h>

class LandscapeObjectsEntryModel;
class PlacementModelDefinition
{
public:
	PlacementModelDefinition();
	virtual ~PlacementModelDefinition();

	virtual bool readXML(XMLNode *node, const char *base);
	LandscapeObjectsEntryModel *createModel(ScorchedContext &context);

protected:
	float size_;
	float modelscale_;
	float modelrotation_;
	std::string removeaction_;
	std::string burnaction_;
	ModelID modelId_;
	ModelID modelburntId_;
};


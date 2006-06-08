#pragma once

#include <landscape/LandscapeObjectsEntry.h>
#include <XML/XMLNode.h>
#include <vector>
#include <string>

class ScorchedContext;
class PlacementGroupDefinition
{
public:
	PlacementGroupDefinition();
	virtual ~PlacementGroupDefinition();

	bool readXML(XMLNode *node);

	void addToGroups(ScorchedContext &context,
		LandscapeObjectEntryBase *objectEntry);

protected:
	std::vector<std::string> groupnames_;

	void addToGroup(const char *groupName,
		ScorchedContext &context,
		LandscapeObjectEntryBase *objectEntry);
};

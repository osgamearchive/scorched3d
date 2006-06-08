#include <placement/PlacementGroupDefinition.h>
#include <engine/ScorchedContext.h>
#include <landscape/LandscapeMaps.h>

PlacementGroupDefinition::PlacementGroupDefinition()
{
}

PlacementGroupDefinition::~PlacementGroupDefinition()
{
}

bool PlacementGroupDefinition::readXML(XMLNode *node)
{
	std::string groupname;
	while (node->getNamedChild("groupname", groupname, false))
	{
		groupnames_.push_back(groupname);
	}
	return true;
}

void PlacementGroupDefinition::addToGroups(
	ScorchedContext &context,
	LandscapeObjectEntryBase *objectEntry)
{
	for (unsigned int i=0; i<groupnames_.size(); i++)
	{
		std::string groupname = groupnames_[i];
		addToGroup(groupname.c_str(), context, objectEntry);
	}
}

void PlacementGroupDefinition::addToGroup(
	const char *groupName,
	ScorchedContext &context,
	LandscapeObjectEntryBase *objectEntry)
{
	LandscapeObjectsGroupEntry *group =
		context.landscapeMaps->getGroundMaps().getObjects().getGroup(
			groupName, 
			&context.landscapeMaps->getGroundMaps().getHeightMap());
	if (group)
	{
		group->addObject(objectEntry);
		objectEntry->groups.push_back(group);
	}
}

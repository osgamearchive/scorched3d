#include <placement/PlacementTypeTankStart.h>
#include <server/ServerNewGameState.h>
#include <engine/ScorchedContext.h>
#include <common/ProgressCounter.h>
#include <common/RandomGenerator.h>
#include <common/Defines.h>
#include <XML/XMLParser.h>

PlacementTypeTankStart::PlacementTypeTankStart()
{
}

PlacementTypeTankStart::~PlacementTypeTankStart()
{
}

bool PlacementTypeTankStart::readXML(XMLNode *node)
{
	if (!node->getNamedChild("numobjects", numobjects)) return false;
	if (!node->getNamedChild("team", team)) return false;
	return PlacementType::readXML(node);
}

void PlacementTypeTankStart::getPositions(ScorchedContext &context,
	RandomGenerator &generator,
	std::list<Position> &returnPositions,
	ProgressCounter *counter)
{
	for (int i=0; i<numobjects; i++)
	{
		Position position;
		position.position = 
			ServerNewGameState::placeTank(0, team, context, generator);
		returnPositions.push_back(position);
	}
}

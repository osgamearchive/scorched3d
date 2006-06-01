#pragma once

#include <placement/PlacementType.h>

class PlacementTypeTankStart : public PlacementType
{
public:
	PlacementTypeTankStart();
	virtual ~PlacementTypeTankStart();

	virtual PlacementType::Type getType() { return PlacementType::eTankStart; }
	virtual bool readXML(XMLNode *node);
	virtual void getPositions(ScorchedContext &context,
		RandomGenerator &generator,
		std::list<Position> &returnPositions,
		ProgressCounter *counter = 0);

protected:
	int numobjects;
	int team;
};


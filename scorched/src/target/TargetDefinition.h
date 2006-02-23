#pragma once

#include <3dsparse/ModelID.h>
#include <engine/ScorchedContext.h>

class Target;
class TargetDefinition
{
public:
	TargetDefinition();
	virtual ~TargetDefinition();

	virtual bool readXML(XMLNode *node);
	Target *createTarget(unsigned int playerId,
		ScorchedContext &context);

protected:
	std::string name_;
	std::string parachute_;
	std::string shield_;
	std::string removeaction_;
	std::string burnaction_;
	ModelID modelId_;
	ModelID modelburntId_;
};

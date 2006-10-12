#pragma once

#include <landscape/LandscapeObjectsEntry.h>

class Target;
class TargetGroupEntry : public LandscapeObjectEntryBase
{
public:
	TargetGroupEntry();
	virtual ~TargetGroupEntry();

	void setTarget(Target *target);

	virtual Vector &getPosition();

protected:
	Target *target_;
};

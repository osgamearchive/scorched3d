#include <target/TargetGroupEntry.h>
#include <target/Target.h>

TargetGroupEntry::TargetGroupEntry()
{
}

TargetGroupEntry::~TargetGroupEntry()
{
}

void TargetGroupEntry::setTarget(Target *target)
{
	target_ = target;
}

Vector &TargetGroupEntry::getPosition()
{
	return target_->getTargetPosition();
}

////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Scorched3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////

#include <target/TargetLife.h>
#include <target/Target.h>
#include <tank/TankType.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <common/Defines.h>

TargetLife::TargetLife(ScorchedContext &context, unsigned int playerId) :
	context_(context),
	targetInfo_(CollisionIdTarget),
	life_(100.0f), maxLife_(100.0f)
{
	// The tank collision object
	targetGeom_ = 
		dCreateSphere(context.actionController->getPhysics().getSpace(), 2.0f);
	targetInfo_.data = (void *) playerId;
	dGeomSetData(targetGeom_, &targetInfo_);
	dGeomDisable(targetGeom_);
}

TargetLife::~TargetLife()
{
	dGeomDestroy(targetGeom_);
}

void TargetLife::newGame()
{
	maxLife_ = 10.0f;
	if (target_->getTargetType() == Target::eTank)
	{
		TankType *tankType = 
			target_->getModel().getTankType(context_);
		maxLife_ = tankType->getLife();
	}

	setLife(maxLife_);
}

void TargetLife::setLife(float life)
{
	life_ = life;

	if (life_ >= maxLife_) life_ = maxLife_;
	if (life_ <= 0)
	{
		life_ = 0;
		dGeomDisable(targetGeom_);
	}
	else
	{
		dGeomEnable(targetGeom_);
	}
}

void TargetLife::setPosition(Vector &pos)
{
	dGeomSetPosition(targetGeom_, pos[0], pos[1], pos[2]);
}

bool TargetLife::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(maxLife_);
	buffer.addToBuffer(life_);
	return true;
}

bool TargetLife::readMessage(NetBufferReader &reader)
{
	float l;
	if (!reader.getFromBuffer(maxLife_)) return false;
	if (!reader.getFromBuffer(l)) return false;
	setLife(l);
	return true;
}

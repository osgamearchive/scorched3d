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
	context_(context), sphereGeom_(true),
	targetInfo_(CollisionIdTarget),
	life_(100.0f), maxLife_(1.0f), size_(2.0f)
{
	// The tank collision object
	if (sphereGeom_)
	{
		targetGeom_ = 
			dCreateSphere(
				context.actionController->getPhysics().getSpace(), 
				size_);
	}
	else
	{
		targetGeom_ =
			dCreateBox(
				context.actionController->getPhysics().getSpace(), 
				size_, size_, size_);
	}

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
		setSize(size_);
		dGeomEnable(targetGeom_);
	}
}

void TargetLife::setSize(float size)
{
	size_ = size;
	if (sphereGeom_)
	{
		dGeomSphereSetRadius(targetGeom_, size_);
	}
	else
	{
		dGeomBoxSetLengths(targetGeom_, size_, size_, size_);
	}
	setPosition(target_->getTargetPosition());
}

void TargetLife::setPosition(Vector &pos)
{
	// Set the position so the geom sits on the top of the ground
	dGeomSetPosition(targetGeom_, pos[0], pos[1], pos[2] + size_ / 2.0f);
}

bool TargetLife::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(maxLife_);
	buffer.addToBuffer(life_);
	buffer.addToBuffer(size_);
	return true;
}

bool TargetLife::readMessage(NetBufferReader &reader)
{
	float l;
	if (!reader.getFromBuffer(maxLife_)) return false;
	if (!reader.getFromBuffer(l)) return false;
	setLife(l);
	if (!reader.getFromBuffer(size_)) return false;
	return true;
}

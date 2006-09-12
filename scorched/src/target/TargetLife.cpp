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
	life_(100.0f), maxLife_(1.0f), 
	size_(2.0f, 2.0f, 2.0f), rotation_(0.0f),
	driveOverToDestroy_(false),
	targetGeom_(0)
{
	targetInfo_.data = (void *) playerId;
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
		rotation_ = 0.0f;
		dGeomDisable(targetGeom_);
	}
	else
	{
		setSize(size_);
		dGeomEnable(targetGeom_);
	}
}

void TargetLife::setSize(Vector &size)
{
	size_ = size;
	if (sphereGeom_)
	{
		double radius = MAX(size_[0], size_[1]) / 2.0f;
		dGeomSphereSetRadius(targetGeom_, radius);
	}
	else
	{
		dGeomBoxSetLengths(targetGeom_, 
			size_[0], size_[1], size_[2]);
	}

	setPosition(target_->getTargetPosition());
}

Vector TargetLife::getGeomRelativePosition(Vector &position)
{
  dVector3 p;
  p[0] = position[0];
  p[1] = position[1];
  p[2] = position[2];
  p[3] = 0;
  dVector3 result;
  const dReal *R = dGeomGetRotation(targetGeom_);

  dMULTIPLY1_331(result,R, p);

  Vector vResult((float)result[0], (float)result[1], (float)result[2]);
  return vResult;
}

void TargetLife::setRotation(float rotation)
{
	rotation_ = rotation;

	dQuaternion q;
	dQFromAxisAndAngle(q, 0.0, 0.0, 1.0, double(rotation_) / 180.0 * 3.14);
	dGeomSetQuaternion(targetGeom_, q);
}

void TargetLife::setPosition(Vector &pos)
{
	// Set the position so the geom sits on the top of the ground
	dGeomSetPosition(targetGeom_, pos[0], pos[1], pos[2] + size_[2] / 2.0f);
}

void TargetLife::setBoundingSphere(bool sphereGeom)
{ 
	if (targetGeom_) dGeomDestroy(targetGeom_);
	sphereGeom_ = sphereGeom; 

	dSpaceID spaceId = context_.actionController->getPhysics().getTargetSpace();
	if (!target_->isTarget())
	{
		spaceId = context_.actionController->getPhysics().getTankSpace();
	}

	// The tank collision object
	if (sphereGeom_)
	{
		double radius = MAX(size_[0], size_[1]) / 2.0f;
		targetGeom_ = dCreateSphere(spaceId, radius);
	}
	else
	{
		targetGeom_ = dCreateBox(spaceId, size_[0], size_[1], size_[2]);
	}
	dGeomSetData(targetGeom_, &targetInfo_);
	dGeomDisable(targetGeom_);
}

bool TargetLife::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(maxLife_);
	buffer.addToBuffer(life_);
	buffer.addToBuffer(size_);
	buffer.addToBuffer(rotation_);
	return true;
}

bool TargetLife::readMessage(NetBufferReader &reader)
{
	float l;
	if (!reader.getFromBuffer(maxLife_)) return false;
	if (!reader.getFromBuffer(l)) return false;
	setLife(l);
	if (!reader.getFromBuffer(size_)) return false;
	if (!reader.getFromBuffer(rotation_)) return false;
	return true;
}

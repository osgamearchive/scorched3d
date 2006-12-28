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
#include <target/TargetSpace.h>
#include <target/Target.h>
#include <tank/TankType.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <tankai/TankAIAdder.h>
#include <common/Defines.h>

TargetLife::TargetLife(ScorchedContext &context, unsigned int playerId) :
	context_(context), sphereGeom_(true),
	life_(100.0f), maxLife_(1.0f), target_(0),
	size_(2.0f, 2.0f, 2.0f), rotation_(0.0f),
	driveOverToDestroy_(false)
{
}

TargetLife::~TargetLife()
{

	removeFromSpace();
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

		removeFromSpace();
	}
	else
	{
		updateAABB();
		addToSpace();
	}
}

void TargetLife::setSize(Vector &size)
{
	size_ = size;

	updateAABB();
}

void TargetLife::setRotation(float rotation)
{
	rotation_ = rotation;
	Vector zaxis(0.0f, 0.0f, 1.0f);
	quaternion_.setQuatFromAxisAndAngle(zaxis, rotation_ / 180.0f * PI);

	updateAABB();
}

void TargetLife::setPosition(Vector &pos)
{
	if (life_ > 0)
	{
		addToSpace();
	}
}

float TargetLife::collisionDistance(Vector &position)
{
	Vector &currentPosition = target_->getCenterPosition();
	Vector direction = position - currentPosition;
	float dist = 0.0f;

	// Get how close the explosion was
	if (getBoundingSphere())
	{
		// Find how close the explosion was to the 
		// outside of the sphere
		float sphereRadius = MAX(MAX(size_[0], size_[1]), size_[2]) / 2.0f;
		dist = direction.Magnitude() - sphereRadius;
		if (dist < 0.0f) dist = 0.0f;
	}
	else
	{
		// Make the direction relative to the geom
		// incase the geom has been rotated
		Vector relativeDirection;
		quaternion_.getRelativeVector(relativeDirection, direction);

		// Find how close the explosion was to the 
		// outside edge of the cube
		Vector touchPosition = relativeDirection;

		// Check each size of the cube to see if the point is outside.
		// If it is, then scale it back until the point sits on the
		// outside edge of the cube.
		int inner = 0;
		for (int i=0; i<3; i++)
		{
			float halfSize = size_[i] / 2.0f;
			if (touchPosition[i] < -halfSize)
			{
				// Scale the point so it sits on this edge
				float diff = -halfSize / touchPosition[i];
				touchPosition *= diff;
			}
			else if (touchPosition[i] > halfSize)
			{
				// Scale the point so it sits on this edge
				float diff = halfSize / touchPosition[i];
				touchPosition *= diff;
			}
			else inner++; // The point is inside this edge
		}

		if (inner == 3)
		{
			// We are inside the cube
			dist = 0.0f;
		}
		else
		{
			// We are outside the cube
			relativeDirection -= touchPosition;
			dist = relativeDirection.Magnitude();
		}
	}

	return dist;
}

bool TargetLife::collision(Vector &position)
{
	Vector &currentPosition = target_->getCenterPosition();
	Vector direction = position - currentPosition;

	// Check against bounding box
	if (direction[0] < -aabbSize_[0] ||
		direction[0] > aabbSize_[0] ||
		direction[1] < -aabbSize_[1] ||
		direction[1] > aabbSize_[1] ||
		direction[2] < -aabbSize_[2] ||
		direction[2] > aabbSize_[2])
	{
		return false;
	}

	// Check against actual bounds
	if (sphereGeom_)
	{
		float radius = MAX(MAX(size_[0], size_[1]), size_[2]) / 2.0f;
		if (direction.Magnitude() > radius) return false;
	}
	else
	{
		// Make the direction relative to the geom
		// incase the geom has been rotated
		Vector relativeDirection;
		quaternion_.getRelativeVector(relativeDirection, direction);

		// Check each side of the cube to see if the point is inside it
		int inner = 0;
		for (int i=0; i<3; i++)
		{
			float halfSize = size_[i] / 2.0f;
			if (-halfSize < relativeDirection[i] && 
				relativeDirection[i] < halfSize)
			{
				inner++; // The point is inside this edge
			}
		}

		// We need the point to be inside each of the 6 edges
		if (inner < 3) return false;
	}

	return true;
}

void TargetLife::setBoundingSphere(bool sphereGeom)
{ 
	sphereGeom_ = sphereGeom; 

	updateAABB();
	removeFromSpace();
}

void TargetLife::addToSpace()
{
	removeFromSpace();
	context_.targetSpace->addTarget(target_);
}

void TargetLife::removeFromSpace()
{
	context_.targetSpace->removeTarget(target_);
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

void TargetLife::updateAABB()
{
	if (sphereGeom_)
	{
		float radius = MAX(MAX(size_[0], size_[1]), size_[2]) / 2.0f;
		aabbSize_ = Vector(radius * 2.0f, radius * 2.0f, radius * 2.0f);
	}
	else
	{
		for (int i=0; i<8; i++)
		{
			Vector position;
			switch (i)
			{
			case 0:
				position = Vector(size_[0], size_[1], size_[2]);
				break;
			case 1:
				position = Vector(-size_[0], size_[1], size_[2]);
				break;
			case 2:
				position = Vector(size_[0], -size_[1], size_[2]);
				break;
			case 3:
				position = Vector(-size_[0], -size_[1], size_[2]);
				break;
			case 4:
				position = Vector(size_[0], size_[1], -size_[2]);
				break;
			case 5:
				position = Vector(-size_[0], size_[1], -size_[2]);
				break;
			case 6:
				position = Vector(size_[0], -size_[1], -size_[2]);
				break;
			case 7:
				position = Vector(-size_[0], -size_[1], -size_[2]);
				break;
			}

			Vector result;
			quaternion_.getRelativeVector(result, position);

			if (i == 0) aabbSize_ = result;
			else
			{
				aabbSize_[0] = MAX(aabbSize_[0], result[0]);
				aabbSize_[1] = MAX(aabbSize_[1], result[1]);
				aabbSize_[2] = MAX(aabbSize_[2], result[2]);
			}
		}
	}
}
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

#include <tank/TankPhysics.h>
#include <tank/TankLib.h>
#include <engine/ScorchedContext.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>

TankPhysics::TankPhysics(ScorchedContext &context, unsigned int playerId) :
	tankInfo_(CollisionIdTank),
	shieldSmallInfo_(CollisionIdShieldSmall),
	shieldLargeInfo_(CollisionIdShieldLarge),
	turretRotXY_(0.0f), turretRotYZ_(0.0f),
	oldTurretRotXY_(0.0f), oldTurretRotYZ_(0.0f),
	angle_(0.0f)
{
	// The tank collision object
	tankGeom_ = 
		dCreateSphere(context.actionController.getPhysics().getSpace(), 1.0f);
	tankInfo_.data = (void *) playerId;
	dGeomSetData(tankGeom_, &tankInfo_);

	// The tank shield collision object
	shieldSmallGeom_ =
		dCreateSphere(context.actionController.getPhysics().getSpace(), 3.0f);
	shieldLargeGeom_ =
		dCreateSphere(context.actionController.getPhysics().getSpace(), 6.0f);
	shieldSmallInfo_.data = (void *) playerId;
	shieldLargeInfo_.data = (void *) playerId;
	dGeomSetData(shieldSmallGeom_, &shieldSmallInfo_);
	dGeomSetData(shieldLargeGeom_, &shieldLargeInfo_);
}

TankPhysics::~TankPhysics()
{
	dGeomDestroy(tankGeom_);
	dGeomDestroy(shieldSmallGeom_);
	dGeomDestroy(shieldLargeGeom_);
}

void TankPhysics::newGame()
{
	oldTurretRotXY_ = turretRotXY_ = RAND * 360;
	oldTurretRotYZ_ = turretRotYZ_ = RAND * 90;
}

void TankPhysics::nextRound()
{
	angle_ = 0.0f;
	oldTurretRotXY_ = turretRotXY_;
	oldTurretRotYZ_ = turretRotYZ_;
}

void TankPhysics::setTankPosition(Vector &pos)
{
	position_ = pos;
	dGeomSetPosition(tankGeom_, pos[0], pos[1], pos[2]);
	dGeomSetPosition(shieldSmallGeom_, pos[0], pos[1], pos[2]);
	dGeomSetPosition(shieldLargeGeom_, pos[0], pos[1], pos[2]);
}

Vector &TankPhysics::getTankGunPosition()
{
	const float gunLength = 1.0f;
	const float degToRad = 180.0f * 3.14f;
	static Vector tankGunPosition;
	tankGunPosition[0] = gunLength * sinf(-turretRotXY_ / degToRad) * 
		sinf((90.0f -turretRotYZ_) / degToRad);
	tankGunPosition[1] = gunLength * cosf(-turretRotXY_ / degToRad) * 
		sinf((90.0f -turretRotYZ_) / degToRad);
	tankGunPosition[2] = gunLength * cosf((90.0f -turretRotYZ_) / degToRad);
	tankGunPosition += getTankTurretPosition();

	return tankGunPosition;
}

Vector &TankPhysics::getTankTurretPosition()
{
	static Vector tankTurretPosition;
	tankTurretPosition = position_;
	tankTurretPosition[2] += 1.0f;//model_->getTurretHeight();
	return tankTurretPosition;
}

Vector &TankPhysics::getTankPosition()
{ 
	return position_; 
}

Vector &TankPhysics::getVelocityVector()
{
	return TankLib::getVelocityVector(
		getRotationGunXY(), getRotationGunYZ());
}

float TankPhysics::rotateGunXY(float angle, bool diff)
{
	if (diff) turretRotXY_ += angle;
	else turretRotXY_ = angle;

	if (turretRotXY_ <= 0.0f) turretRotXY_ = 360.0f + turretRotXY_;
	else if (turretRotXY_ > 360.0f) turretRotXY_ = turretRotXY_ - 360.0f;

	return turretRotXY_;
}

float TankPhysics::rotateGunYZ(float angle, bool diff)
{
	if (diff) turretRotYZ_ += angle;
	else turretRotYZ_ = angle;

	if (turretRotYZ_ < 0.0f) turretRotYZ_ = 0.0f;
	else if (turretRotYZ_ > 90.0f) turretRotYZ_ = 90.0f;

	return turretRotYZ_;
}

const char *TankPhysics::getRotationString()
{
	static char messageBuffer[255];
	float rotDiff = (360.0f - getRotationGunXY()) - (360.0f - getOldRotationGunXY());
	if (rotDiff > 180.0f) rotDiff -= 360.0f;
	else if (rotDiff < -180.0f) rotDiff += 360.0f;

	if (OptionsDisplay::instance()->getUseHexidecimal())
	{
		sprintf(messageBuffer, "0x%x (0x%x)", 
				int(360.0f - getRotationGunXY()),
				int(rotDiff));
	}
	else
	{
		sprintf(messageBuffer, "%.1f (%+.1f)", 
				360.0f - getRotationGunXY(),
				rotDiff);
	}

	return messageBuffer;
}

const char *TankPhysics::getElevationString()
{
	static char messageBuffer[255];
	if (OptionsDisplay::instance()->getUseHexidecimal())
	{
		sprintf(messageBuffer, "0x%x (0X%x)", 
				int(getRotationGunYZ()),
				int(getRotationGunYZ() - 
				getOldRotationGunYZ()));
	}
	else
	{
		sprintf(messageBuffer, "%.1f (%+.1f)", 
				getRotationGunYZ(),
				getRotationGunYZ() - 
				getOldRotationGunYZ());
	}
	return messageBuffer;
}

bool TankPhysics::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(position_[0]);
	buffer.addToBuffer(position_[1]);
	buffer.addToBuffer(position_[2]);
	buffer.addToBuffer(turretRotXY_);
	buffer.addToBuffer(turretRotYZ_);
	buffer.addToBuffer(oldTurretRotXY_);
	buffer.addToBuffer(oldTurretRotYZ_);
	return true;
}

bool TankPhysics::readMessage(NetBufferReader &reader)
{
	Vector pos;
	if (!reader.getFromBuffer(pos[0])) return false;
	if (!reader.getFromBuffer(pos[1])) return false;
	if (!reader.getFromBuffer(pos[2])) return false;
	setTankPosition(pos);
	if (!reader.getFromBuffer(turretRotXY_)) return false;
	if (!reader.getFromBuffer(turretRotYZ_)) return false;
	if (!reader.getFromBuffer(oldTurretRotXY_)) return false;
	if (!reader.getFromBuffer(oldTurretRotYZ_)) return false;
	return true;
}

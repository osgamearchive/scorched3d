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

#include <tank/Tank.h>
#include <tank/TankLib.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <common/OptionsDisplay.h>

TankPhysics::TankPhysics(ScorchedContext &context, unsigned int playerId) :
	tankInfo_(CollisionIdTank),
	shieldSmallInfo_(CollisionIdShieldSmall),
	shieldLargeInfo_(CollisionIdShieldLarge),
	turretRotXY_(0.0f), turretRotYZ_(0.0f),
	angle_(0.0f), power_(1000.0f), tank_(0), context_(context)
{
	// Only make the very first shot random angle
	turretRotXY_ = RAND * 360;
	turretRotYZ_ = RAND * 90;

	// The tank collision object
	tankGeom_ = 
		dCreateSphere(context.actionController->getPhysics().getSpace(), 2.0f);
	tankInfo_.data = (void *) playerId;
	dGeomSetData(tankGeom_, &tankInfo_);

	// The tank shield collision object
	shieldSmallGeom_ =
		dCreateSphere(context.actionController->getPhysics().getSpace(), 3.0f);
	shieldLargeGeom_ =
		dCreateSphere(context.actionController->getPhysics().getSpace(), 6.0f);
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

void TankPhysics::enablePhysics()
{
	dGeomEnable(tankGeom_);
	dGeomEnable(shieldSmallGeom_);
	dGeomEnable(shieldLargeGeom_);
}

void TankPhysics::disablePhysics()
{
	dGeomDisable(tankGeom_);
	dGeomDisable(shieldSmallGeom_);
	dGeomDisable(shieldLargeGeom_);
}

void TankPhysics::setTankPosition(Vector &pos)
{
	position_ = pos;
	dGeomSetPosition(tankGeom_, pos[0], pos[1], pos[2]);
	dGeomSetPosition(shieldSmallGeom_, pos[0], pos[1], pos[2]);
	dGeomSetPosition(shieldLargeGeom_, pos[0], pos[1], pos[2]);
}

void TankPhysics::clientNewGame()
{
	power_ = 1000.0f;
	angle_ = 0.0f;
	oldShots_.clear();
	madeShot();
}

void TankPhysics::madeShot()
{
	oldShots_.push_back(ShotEntry(power_, turretRotXY_, turretRotYZ_));
	if (oldShots_.size() > 15) 
	{
		oldShots_.erase(oldShots_.begin());
	}
}

float TankPhysics::getOldPower()
{
	if (oldShots_.empty()) return power_;
	return oldShots_.back().power;
}

float TankPhysics::getOldRotationGunXY()
{
	if (oldShots_.empty()) return turretRotXY_;
	return oldShots_.back().rot;
}

float TankPhysics::getOldRotationGunYZ()
{
	if (oldShots_.empty()) return turretRotYZ_;
	return oldShots_.back().ele;
}

void TankPhysics::revertSettings(unsigned int index)
{
	if (index < oldShots_.size())
	{
		int newIndex = (oldShots_.size() - 1) - index;
		rotateGunXY(oldShots_[newIndex].rot, false);
		rotateGunYZ(oldShots_[newIndex].ele, false);
		changePower(oldShots_[newIndex].power, false);

		madeShot(); // Store this as the newest chosen settings
	}
}

Vector &TankPhysics::getTankGunPosition()
{
	static Vector tankGunPosition;
	tankGunPosition = TankLib::getGunPosition(
			getRotationGunXY(), getRotationGunYZ());
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

float TankPhysics::changePower(float power, bool diff)
{
	if (diff) power_ += power;
	else power_ = power;

	if (power_ < 0.0f) power_ = 0.0f;
	if (context_.optionsGame->getLimitPowerByHealth())
	{
		if (power_ > tank_->getState().getLife() * 10.0f) 
			power_ = tank_->getState().getLife() * 10.0f;
	}
	else
	{
		if (power_ > 1000.0f) power_ = 1000.0f;
	}

	return power_;
}

float TankPhysics::getRotationXYDiff()
{
	float rotDiff = (360.0f - getRotationGunXY()) - (360.0f - getOldRotationGunXY());
	if (rotDiff > 180.0f) rotDiff -= 360.0f;
	else if (rotDiff < -180.0f) rotDiff += 360.0f;
	return rotDiff;
}

float TankPhysics::getRotationYZDiff()
{
	return getRotationGunYZ() - getOldRotationGunYZ();
}

float TankPhysics::getPowerDiff()
{
	return getPower() - getOldPower();
}

const char *TankPhysics::getRotationString()
{
	static char messageBuffer[255];
	float rotDiff = getRotationXYDiff();

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
	float rotDiff = getRotationYZDiff();

	if (OptionsDisplay::instance()->getUseHexidecimal())
	{
		sprintf(messageBuffer, "0x%x (0X%x)", 
				int(getRotationGunYZ()),
				int(rotDiff));
	}
	else
	{
		sprintf(messageBuffer, "%.1f (%+.1f)", 
				getRotationGunYZ(),
				rotDiff);
	}
	return messageBuffer;
}

const char *TankPhysics::getPowerString()
{
	static char messageBuffer[255];
	float powDiff = getPowerDiff();

	if (OptionsDisplay::instance()->getUseHexidecimal())
	{
		sprintf(messageBuffer, "0X%x (0X%x)", 		
				int(getPower()),
				int(powDiff));
	}
	else
	{
		sprintf(messageBuffer, "%.1f (%+.1f)", 		
				getPower(),
				powDiff);
	}

	return messageBuffer;
}

bool TankPhysics::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(position_[0]);
	buffer.addToBuffer(position_[1]);
	buffer.addToBuffer(position_[2]);
	return true;
}

bool TankPhysics::readMessage(NetBufferReader &reader)
{
	Vector pos;
	if (!reader.getFromBuffer(pos[0])) return false;
	if (!reader.getFromBuffer(pos[1])) return false;
	if (!reader.getFromBuffer(pos[2])) return false;
	setTankPosition(pos);
	angle_ = 0.0f;
	return true;
}


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

TankPosition::TankPosition(ScorchedContext &context, unsigned int playerId) :
	turretRotXY_(0.0f), turretRotYZ_(0.0f),
	oldTurretRotXY_(0.0f), oldTurretRotYZ_(0.0f),
	power_(1000.0f), oldPower_(1000.0f),
	angle_(0.0f), tank_(0), context_(context)
{
	// Only make the very first shot random angle
	oldTurretRotXY_ = turretRotXY_ = RAND * 360;
	oldTurretRotYZ_ = turretRotYZ_ = RAND * 90;
}

TankPosition::~TankPosition()
{
}

std::vector<TankPosition::ShotEntry> &TankPosition::getOldShots()
{
	std::vector<ShotEntry>::iterator itor;
	for (itor = oldShots_.begin();
		itor != oldShots_.end();
		itor++)
	{
		ShotEntry &entry = *itor;
		entry.current = 
			(entry.ele == oldTurretRotYZ_ &&
			entry.rot == oldTurretRotXY_ &&
			entry.power == oldPower_);
	}

	return oldShots_; 
}

void TankPosition::clientNewGame()
{
	oldPower_ = power_ = 1000.0f;
	angle_ = 0.0f;
	oldShots_.clear();
	madeShot();
}

void TankPosition::madeShot()
{
	oldPower_ = power_;
	oldTurretRotXY_ = turretRotXY_;
	oldTurretRotYZ_ = turretRotYZ_;

	if (oldShots_.empty() ||
		oldShots_.back().power != power_ ||
		oldShots_.back().rot != turretRotXY_ ||
		oldShots_.back().ele != turretRotYZ_)
	{
		oldShots_.push_back(ShotEntry(power_, turretRotXY_, turretRotYZ_));
	}

	if (oldShots_.size() > 15) 
	{
		oldShots_.erase(oldShots_.begin());
	}
}

void TankPosition::revertSettings(unsigned int index)
{
	if (index < oldShots_.size())
	{
		int newIndex = (oldShots_.size() - 1) - index;
		rotateGunXY(oldShots_[newIndex].rot, false);
		rotateGunYZ(oldShots_[newIndex].ele, false);
		changePower(oldShots_[newIndex].power, false);

		oldPower_ = power_;
		oldTurretRotXY_ = turretRotXY_;
		oldTurretRotYZ_ = turretRotYZ_;
	}
}

void TankPosition::undo()
{
	rotateGunXY(oldTurretRotXY_, false);
	rotateGunYZ(oldTurretRotYZ_, false);
	changePower(oldPower_, false);
}

Vector &TankPosition::getTankGunPosition()
{
	static Vector tankGunPosition;
	tankGunPosition = TankLib::getGunPosition(
			getRotationGunXY(), getRotationGunYZ());
	tankGunPosition += getTankTurretPosition();

	return tankGunPosition;
}

Vector &TankPosition::getTankTurretPosition()
{
	static Vector tankTurretPosition;
	tankTurretPosition = getTankPosition();
	tankTurretPosition[2] += 1.0f;//model_->getTurretHeight();

	return tankTurretPosition;
}

Vector &TankPosition::getTankPosition()
{ 
	return tank_->getTargetPosition();
}

Vector &TankPosition::getVelocityVector()
{
	return TankLib::getVelocityVector(
		getRotationGunXY(), getRotationGunYZ());
}

float TankPosition::rotateGunXY(float angle, bool diff)
{
	if (diff) turretRotXY_ += angle;
	else turretRotXY_ = angle;

	if (turretRotXY_ <= 0.0f) turretRotXY_ = 360.0f + turretRotXY_;
	else if (turretRotXY_ > 360.0f) turretRotXY_ = turretRotXY_ - 360.0f;

	return turretRotXY_;
}

float TankPosition::rotateGunYZ(float angle, bool diff)
{
	if (diff) turretRotYZ_ += angle;
	else turretRotYZ_ = angle;

	if (turretRotYZ_ < 0.0f) turretRotYZ_ = 0.0f;
	else if (turretRotYZ_ > 90.0f) turretRotYZ_ = 90.0f;

	return turretRotYZ_;
}

float TankPosition::changePower(float power, bool diff)
{
	if (diff) power_ += power;
	else power_ = power;

	if (power_ < 0.0f) power_ = 0.0f;
	if (context_.optionsGame->getLimitPowerByHealth())
	{
		if (power_ > tank_->getLife().getLife() * 10.0f) 
			power_ = tank_->getLife().getLife() * 10.0f;
	}
	else
	{
		if (power_ > 1000.0f) power_ = 1000.0f;
	}

	return power_;
}

float TankPosition::getRotationXYDiff()
{
	float rotDiff = (360.0f - turretRotXY_) - (360.0f - oldTurretRotXY_);
	if (rotDiff > 180.0f) rotDiff -= 360.0f;
	else if (rotDiff < -180.0f) rotDiff += 360.0f;
	return rotDiff;
}

float TankPosition::getRotationYZDiff()
{
	return turretRotYZ_ - oldTurretRotYZ_;
}

float TankPosition::getPowerDiff()
{
	return power_ - oldPower_;
}

const char *TankPosition::getRotationString()
{
	static char messageBuffer[255];
	float rotDiff = getRotationXYDiff();

	if (OptionsDisplay::instance()->getUseHexidecimal())
	{
		snprintf(messageBuffer, 255, "0x%x (0x%x)", 
				int(360.0f - getRotationGunXY()),
				int(rotDiff));
	}
	else
	{
		snprintf(messageBuffer, 255, "%.1f (%+.1f)", 
				360.0f - getRotationGunXY(),
				rotDiff);
	}

	return messageBuffer;
}

const char *TankPosition::getElevationString()
{
	static char messageBuffer[255];
	float rotDiff = getRotationYZDiff();

	if (OptionsDisplay::instance()->getUseHexidecimal())
	{
		snprintf(messageBuffer, 255, "0x%x (0X%x)", 
				int(getRotationGunYZ()),
				int(rotDiff));
	}
	else
	{
		snprintf(messageBuffer, 255, "%.1f (%+.1f)", 
				getRotationGunYZ(),
				rotDiff);
	}
	return messageBuffer;
}

const char *TankPosition::getPowerString()
{
	static char messageBuffer[255];
	float powDiff = getPowerDiff();

	if (OptionsDisplay::instance()->getUseHexidecimal())
	{
		snprintf(messageBuffer, 255, "0X%x (0X%x)", 		
				int(getPower()),
				int(powDiff));
	}
	else
	{
		snprintf(messageBuffer, 255, "%.1f (%+.1f)", 		
				getPower(),
				powDiff);
	}

	return messageBuffer;
}
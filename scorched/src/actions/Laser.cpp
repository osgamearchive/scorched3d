////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <actions/Laser.h>
#include <actions/TankDamage.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <weapons/AccessoryStore.h>
#include <weapons/Shield.h>
#include <actions/ShieldHit.h>
#include <target/TargetContainer.h>
#include <target/TargetShield.h>
#include <target/TargetLife.h>
#include <target/TargetSpace.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <GLEXT/GLState.h>
#include <math.h>
#include <set>

Laser::Laser(WeaponLaser *weapon,
		Vector &position, Vector &direction,
		WeaponFireContext &weaponContext) :
	totalTime_(0.0f),
	drawLength_(0.0f),
	firstTime_(true),
	weaponContext_(weaponContext), 
	weapon_(weapon),
	position_(position), 
	direction_(direction)
{
}

Laser::~Laser()
{
}

void Laser::init()
{
	float per = direction_.Magnitude() / 50.0f;
	length_ = weapon_->getMinimumDistance() + 
		(weapon_->getMaximumDistance() - weapon_->getMinimumDistance()) * per;
	damage_ = weapon_->getMinimumHurt() + 
		(weapon_->getMaximumHurt() - weapon_->getMinimumHurt()) * (1.0f - per);

	Vector dir = direction_.Normalize();
	angXY_ = 180.0f - atan2f(dir[0], dir[1]) / 3.14f * 180.0f;
	angYZ_ = acosf(dir[2]) / 3.14f * 180.0f;
}

void Laser::simulate(float frameTime, bool &remove)
{
	if (firstTime_)
	{
		firstTime_ = false;
		if (damage_ > 0.0f && direction_.Magnitude() > 0.0f)
		{
			std::set<unsigned int> damagedTargets_;

			// Build a set of all tanks in the path of the laser
			Vector pos = position_;
			Vector dir = direction_.Normalize() / 10.0f;
			bool end = false;
			while (!end)
			{
				std::map<unsigned int, Target *> collisionTargets;
				context_->targetSpace->getCollisionSet(pos, 
					float(weapon_->getHurtRadius()), collisionTargets);
				std::map<unsigned int, Target *>::iterator itor;
				for (itor = collisionTargets.begin();
					itor != collisionTargets.end();
					itor++)
				{
					Target *current = (*itor).second;
					if (current->getAlive() &&
						((current->getPlayerId() != weaponContext_.getPlayerId()) ||
						weapon_->getHurtFirer()))
					{
						Vector offset = current->getLife().getTargetPosition() -	pos;
						float targetDistance = offset.Magnitude();
						Shield::ShieldLaserProofType laserProof = Shield::ShieldLaserProofNone;

						if (current->getShield().getCurrentShield())
						{
							Shield *shield = (Shield *)
								current->getShield().getCurrentShield()->getAction();
							if (shield->getLaserProof() != Shield::ShieldLaserProofNone)
							{
								laserProof = shield->getLaserProof();
								if (shield->inShield(offset))
								{
									context_->actionController->addAction(
										new ShieldHit(current->getPlayerId(), pos, 0.0f));

									end = true;
									break;
								}
							}
						}

						if (laserProof != Shield::ShieldLaserProofTotal)
						{
							if (targetDistance < weapon_->getHurtRadius() + 
								MAX(current->getLife().getSize()[0], current->getLife().getSize()[1]))
							{
								damagedTargets_.insert(current->getPlayerId());
							}
						}
					}
				}

				if (!end)
				{
					pos += dir;
					drawLength_ = (pos - position_).Magnitude();
					if (drawLength_ > length_) end = true;
				}
			}

			// Subtract set amount from all tanks
			std::set<unsigned int>::iterator itor;
			for (itor = damagedTargets_.begin();
				itor != damagedTargets_.end();
				itor++)
			{
				unsigned int damagedTarget = (*itor);
				context_->actionController->addAction(
					new TankDamage(
						weapon_, damagedTarget, weaponContext_,
						damage_, false, false, false));
			}
		}
	}

	totalTime_ += frameTime;

	remove = (totalTime_ > weapon_->getTotalTime());
	Action::simulate(frameTime, remove);
}

void Laser::draw()
{
#ifndef S3D_SERVER
	if (!context_->serverMode && (drawLength_ > 0.0f))
	{
		static GLUquadric *obj = 0;
		if (!obj)
		{
			obj = gluNewQuadric();
		}
		float timePer = (1.0f - totalTime_ / weapon_->getTotalTime()) * 0.5f;
		float radius1 = 0.05f / 2.0f * weapon_->getHurtRadius();
		float radius2 = 0.2f / 2.0f * weapon_->getHurtRadius();

		GLState glState(GLState::TEXTURE_OFF | GLState::BLEND_ON);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glColor4f(1.0f, 1.0f, 1.0f,	timePer);
		glPushMatrix();
			glTranslatef(position_[0], position_[1], position_[2]);
			glRotatef(angXY_, 0.0f, 0.0f, 1.0f);
			glRotatef(angYZ_, 1.0f, 0.0f, 0.0f);

			glColor4f(1.0f, 1.0f, 1.0f,	timePer);
			gluCylinder(obj, radius1, radius1, drawLength_, 3, 1);

			glColor4f(
				weapon_->getColor()[0],
				weapon_->getColor()[1],
				weapon_->getColor()[2],
				timePer);
			gluCylinder(obj, radius2, radius2, drawLength_, 5, 1);
		glPopMatrix();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
#endif // #ifndef S3D_SERVER
}

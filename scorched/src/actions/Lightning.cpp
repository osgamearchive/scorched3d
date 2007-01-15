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

#include <actions/Lightning.h>
#include <common/Defines.h>
#include <sound/SoundUtils.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLCamera.h>
#include <sprites/ExplosionTextures.h>
#include <target/TargetContainer.h>
#include <target/TargetDamageCalc.h>
#include <target/TargetLife.h>
#include <target/TargetSpace.h>
#include <weapons/AccessoryStore.h>
#include <math.h>

Lightning::Lightning(WeaponLightning *weapon,
		WeaponFireContext &weaponContext,
		Vector &position, Vector &velocity) :
	totalTime_(0.0f),
	weapon_(weapon),
	weaponContext_(weaponContext),
	position_(position), velocity_(velocity),
	generator_(0)
{
}

Lightning::~Lightning()
{
	delete generator_;
}

void Lightning::init()
{
	generator_ = new RandomGenerator();
	generator_->seed(weaponContext_.getPlayerId());
	Vector direction = velocity_.Normalize();
	std::map<unsigned int, float> hurtMap;

	generateLightning(0, 1, weapon_->getSize(), 
		position_, direction, position_, direction,
		hurtMap);

	std::map<unsigned int, float>::iterator hurtItor;
	for (hurtItor = hurtMap.begin();
		hurtItor != hurtMap.end();
		hurtItor++)
	{
		unsigned int playerId = (*hurtItor).first;
		float damage = (*hurtItor).second;

		Target *target = context_->targetContainer->getTargetById(playerId);
		if (target)
		{
			TargetDamageCalc::damageTarget(
				*context_, target, weapon_, weaponContext_, 
				damage, true, false, false);
		}
	}
}

void Lightning::simulate(float frameTime, bool &remove)
{
#ifndef S3D_SERVER
	if (!context_->serverMode)
	{   
		if (firstTime_)
		{ 
			firstTime_ = false;
			if (weapon_->getSound() &&
				0 != strcmp("none", weapon_->getSound()))
			{
				SoundBuffer *expSound =
					Sound::instance()->fetchOrCreateBuffer(
					(char *) getDataFile(weapon_->getSound()));
				SoundUtils::playAbsoluteSound(VirtualSoundPriority::eAction,
					expSound, position_);
			}
		} 
	}
#endif // #ifndef S3D_SERVER

	totalTime_ += frameTime;
	remove = (totalTime_ > weapon_->getTotalTime());
	Action::simulate(frameTime, remove);
}

void Lightning::draw()
{
#ifndef S3D_SERVER
	if (!context_->serverMode)
	{
		Vector &cameraPos = 
			GLCamera::getCurrentCamera()->getCurrentPos();

		GLState state(GLState::TEXTURE_ON | GLState::BLEND_ON);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDepthMask(GL_FALSE);
		glColor4f(1.0f, 1.0f, 1.0f, 
			1.0f - totalTime_ / weapon_->getTotalTime());

		ExplosionTextures::instance()->lightningTexture.draw();
		
		Vector offset(0.0f, 0.0f, 0.5f);
		bool began = false; 
		float texCoord = 0.0f;
		std::list<Segment>::iterator itor;
		for (itor = segments_.begin();
			itor != segments_.end();
			itor++)
		{
			Segment &segment = (*itor);
			Vector cameraDir = (segment.start - cameraPos).Normalize();
			Vector offset = (segment.direction * cameraDir).Normalize();
			offset *= 0.4f;

			if (!began)
			{
				began = true;
				glBegin(GL_QUAD_STRIP);
			}

			glTexCoord2f(1.0f, texCoord);
			glVertex3fv(segment.start + offset * segment.size);
			glTexCoord2f(0.0f, texCoord);
			glVertex3fv(segment.start - offset * segment.size);
			texCoord += 1.0f;

			if (segment.endsegment)
			{
				glTexCoord2f(0.0f, texCoord);
				glVertex3fv(segment.end - offset * segment.size);
				glTexCoord2f(1.0f, texCoord);
				glVertex3fv(segment.end + offset * segment.size);
				texCoord += 1.0f;

				began = false;
				glEnd();
			}
		}

		glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
#endif // #ifndef S3D_SERVER
}

void Lightning::dispaceDirection(Vector &direction, 
	Vector &originalDirection, float angle)
{
	int breakCount = 0;

	Vector newdir;
	while (breakCount++ < 1000)
	{
		newdir[0] = (generator_->getRandFloat() - 0.5f) * 2.0f;
		newdir[1] = (generator_->getRandFloat() - 0.5f) * 2.0f;
		newdir[2] = (generator_->getRandFloat() - 0.5f) * 2.0f;
		newdir.StoreNormalize();

		float a = newdir[0] * direction[0] + 
			newdir[1] * direction[1] + 
			newdir[2] * direction[2];
		if (a < angle) continue;

		float b = newdir[0] * originalDirection[0] + 
			newdir[1] * originalDirection[1] + 
			newdir[2] * originalDirection[2];
		if (b >= weapon_->getDerivAngle())
		{
			direction = newdir;
			return;
		}
	}	
}

void Lightning::generateLightning(int id, int depth, float size, 
	Vector &originalPosition, Vector &originalDirection,
	Vector &start, Vector &direction,
	std::map<unsigned int, float> &hurtMap)
{
	if (id > 100) return;

	float length = weapon_->getSegLength() + 
		weapon_->getSegVar() * generator_->getRandFloat();
	Vector end = start + direction * length;

	// Add the new lightning segment
	segments_.push_back(Segment());
	Segment &segment = segments_.back();

	// Set the segment
	segment.start = start;
	segment.end = end;
	segment.size = size;
	segment.direction = direction;
	segment.endsegment = false;

	// Damage any tanks
	damageTargets(segment.end, hurtMap);

	// Rand posibility that we stop
	if (depth > 1 && generator_->getRandFloat() < 
		weapon_->getDeathProb())
	{
		segment.endsegment = true;
		return;
	}

	// Check if we have gone too far
	if ((originalPosition - end).Magnitude() > 
		weapon_->getConeLength()) 
	{
		segment.endsegment = true;
		return;
	}

	// Continue this lightning strand
	{
		Vector newdirection = end - start;
		dispaceDirection(newdirection, originalDirection, 
			weapon_->getAngleVar() * 0.25f);
		generateLightning(id + 1, depth, size, 
			originalPosition, originalDirection, 
			end, newdirection,
			hurtMap);	
	}

	// Make a new strand
	if (generator_->getRandFloat() <= 
		weapon_->getSplitProb() - (depth - 1) * weapon_->getSplitVar())
    {
		float newsize = size + weapon_->getSizeVar();
		if (newsize < weapon_->getMinSize()) 
			newsize = weapon_->getMinSize();

		Vector newdirection = end - start;
		dispaceDirection(newdirection, originalDirection, 
			weapon_->getAngleVar());
		generateLightning(id + 1, depth + 1, newsize, 
			originalPosition, originalDirection, 
			end, newdirection,
			hurtMap);	
	}
}

void Lightning::damageTargets(Vector &position, 
		std::map<unsigned int, float> &hurtMap)
{
	if (weapon_->getSegHurt() <= 0.0f) return;

	std::map<unsigned int, Target *> collisionTargets;
	context_->targetSpace->getCollisionSet(position, 
		weapon_->getSegHurtRadius() * 1.5f, collisionTargets);
	std::map<unsigned int, Target *>::iterator itor;
	for (itor = collisionTargets.begin();
		itor != collisionTargets.end();
		itor++)
	{
		Target *target = (*itor).second;
		if (target->getAlive() &&
			target->getPlayerId() != weaponContext_.getPlayerId())
		{
			float distance = (target->getLife().getTargetPosition() -
				position).Magnitude();
			if (distance < weapon_->getSegHurtRadius() + 
				MAX(target->getLife().getSize()[0], target->getLife().getSize()[1]))
			{
				std::map<unsigned int, float>::iterator findItor = 
					hurtMap.find(target->getPlayerId());
				if (findItor == hurtMap.end())
				{
					hurtMap[target->getPlayerId()] = weapon_->getSegHurt();
				}
				else
				{
					hurtMap[target->getPlayerId()] += weapon_->getSegHurt();
				}
			}
		}
	}
}

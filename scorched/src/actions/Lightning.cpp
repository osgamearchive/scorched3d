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
#include <common/SoundStore.h>
#include <GLEXT/GLState.h>
#include <sprites/ExplosionTextures.h>
#include <client/MainCamera.h>
#include <tank/TankContainer.h>
#include <tank/TankController.h>
#include <weapons/AccessoryStore.h>
#include <math.h>

REGISTER_ACTION_SOURCE(Lightning);

Lightning::Lightning() :
	totalTime_(0.0f), firstTime_(true),
	weapon_(0), playerId_(0), data_(0)
{
}

Lightning::Lightning(WeaponLightning *weapon,
		unsigned int playerId, 
		Vector &position, Vector &velocity,
		unsigned int data) :
	totalTime_(0.0f),
	weapon_(weapon),
	playerId_(playerId), data_(data),
	position_(position), velocity_(velocity)
{
	seed_ = (unsigned int) rand();
}

Lightning::~Lightning()
{
}

void Lightning::init()
{
	generator_.seed(seed_);
	Vector direction = velocity_.Normalize();
	std::map<Tank *, float> hurtMap;

	generateLightning(0, 1, weapon_->getSize(), 
		position_, direction, position_, direction,
		hurtMap);

	std::map<Tank *, float>::iterator hurtItor;
	for (hurtItor = hurtMap.begin();
		hurtItor != hurtMap.end();
		hurtItor++)
	{
		Tank *tank = (*hurtItor).first;
		float damage = (*hurtItor).second;
		TankController::damageTank(
			*context_, tank, weapon_, playerId_, 
			damage, false, data_);
	}
}

void Lightning::simulate(float frameTime, bool &remove)
{
	if (!context_->serverMode)
	{   
		if (firstTime_)
		{ 
			firstTime_ = false;
			if (weapon_->getSound() &&
				0 != strcmp("none", weapon_->getSound()))
			{
				SoundBuffer *expSound =
					SoundStore::instance()->fetchOrCreateBuffer(
					(char *) getDataFile(weapon_->getSound()));
				expSound->play();
			}
		} 
	}

	totalTime_ += frameTime;
	remove = (totalTime_ > weapon_->getTotalTime());
	Action::simulate(frameTime, remove);
}

void Lightning::draw()
{
	if (!context_->serverMode)
	{
		Vector &cameraPos =
			MainCamera::instance()->getCamera().getCurrentPos();

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
}

bool Lightning::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(seed_);
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(data_);
	buffer.addToBuffer(position_);
	buffer.addToBuffer(velocity_);
	context_->accessoryStore->writeWeapon(buffer, weapon_);
	return true;
}

bool Lightning::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(seed_)) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(data_)) return false;
	if (!reader.getFromBuffer(position_)) return false;
	if (!reader.getFromBuffer(velocity_)) return false;
	weapon_ = (WeaponLightning*) context_->accessoryStore->readWeapon(reader); if (!weapon_) return false;
	return true;
}

void Lightning::dispaceDirection(Vector &direction, 
	Vector &originalDirection, float angle)
{
	Vector newdir;
	while (true)
	{
		newdir[0] = (generator_.getRandFloat() - 0.5f) * 2.0f;
		newdir[1] = (generator_.getRandFloat() - 0.5f) * 2.0f;
		newdir[2] = (generator_.getRandFloat() - 0.5f) * 2.0f;
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
	std::map<Tank *, float> &hurtMap)
{
	float length = weapon_->getSegLength() + 
		weapon_->getSegVar() * generator_.getRandFloat();
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
	damageTanks(segment.end, hurtMap);

	// Rand posibility that we stop
	if (depth > 1 && generator_.getRandFloat() < 
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
	if (generator_.getRandFloat() <= 
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

void Lightning::damageTanks(Vector &position, 
		std::map<Tank *, float> &hurtMap)
{
	if (weapon_->getSegHurt() <= 0.0f) return;

	std::map<unsigned int, Tank *> &tanks = 
		context_->tankContainer->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getState().getState() == TankState::sNormal &&
			!tank->getState().getSpectator() &&
			tank->getPlayerId() != playerId_)
		{
			float distance = (tank->getPhysics().getTankPosition() -
				position).Magnitude();
			if (distance < weapon_->getSegHurtRadius())
			{
				std::map<Tank *, float>::iterator findItor = 
					hurtMap.find(tank);
				if (findItor == hurtMap.end())
				{
					hurtMap[tank] = weapon_->getSegHurt();
				}
				else
				{
					hurtMap[tank] += weapon_->getSegHurt();
				}
			}
		}
	}
}

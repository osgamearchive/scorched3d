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

#include <GLEXT/GLState.h>
#include <GLEXT/GLCameraFrustum.h>
#include <3dsparse/ModelStore.h>
#include <3dsparse/ModelRenderer.h>
#include <client/ScorchedClient.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/LandscapeTex.h>
#include <landscape/HeightMap.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>
#include <sound/Sound.h>
#include <boids/BoidWorld.h>
#include <boids/Boid.h>
#include <boids/Obstacle.h>
#include <boids/ScorchedBoidsObstacle.h>
#include <stdlib.h>

BoidWorld::BoidWorld(LandscapeTexBoids *boids) : 
	visibilityMatrix_(0), boidSoundIndex_(0),
	elapsedTime_(0.0f), stepTime_(0.0f), stepTime2_(0.0f),
	halfTime_(false), modelSize_(boids->modelsize * 0.005f),
	soundCurrentTime_(0.0f), soundNextTime_(0.0f),
	soundMinTime_(boids->soundmintime), soundMaxTime_(boids->soundmaxtime),
	cruiseDistance_(0.75f * boids->cruisedistance), 
	maxVelocity_(15.0f * boids->maxvelocity), 
	maxAcceleration_(0.65f * boids->maxacceleration)
{
	// Create boids
	makeBoids(boids->count, boids->maxbounds, boids->minbounds);
	makeObstacles(boids->maxbounds, boids->minbounds);

	if (!OptionsDisplay::instance()->getNoBoidSound())
	{
		makeSounds(boids->sounds, boids->soundmaxsimul, boids->soundvolume);
	}

	// Allocate a new visibility matrix
	visibilityMatrix_ = new int*[boids_.size()];
	for (unsigned int i=0; i<boids_.size(); i++)
	{
		visibilityMatrix_[i] = new int[boids_.size()];
		for (unsigned int j=0; j<boids_.size(); j++)
		{
			visibilityMatrix_[i][j] = -1;
		}
	}

	// Create bird model
	bird_ = new ModelRenderer(
			ModelStore::instance()->loadModel(boids->model));
}

BoidWorld::~BoidWorld()
{
	delete bird_;
	for (unsigned int i=0; i<boids_.size(); i++) 
	{
		delete []visibilityMatrix_[i];
	}
	delete []visibilityMatrix_;
	visibilityMatrix_ = 0;

	while (!boids_.empty())
	{
		Boid *boid = boids_.back();
		boids_.pop_back();
		delete boid;
	}
	while (!obstacles_.empty())
	{
		Obstacle *obstacle = obstacles_.back();
		obstacles_.pop_back();
		delete obstacle;
	}
	while (!currentSounds_.empty())
	{
		SoundEntry entry = currentSounds_.back();
		currentSounds_.pop_back();
		delete entry.source;
	}
}

void BoidWorld::makeSounds(std::list<std::string> &sounds, 
	int soundmaxsimul, float soundvolume)
{
	// Create the list of sound sources that will be used
	// to play sounds at the same time
	for (int i=0; i<soundmaxsimul; i++)
	{
		SoundEntry entry;
		entry.source = new VirtualSoundSource(VirtualSoundPriority::eBoids, false, false);
		entry.source->setGain(soundvolume / 100.0f);
		currentSounds_.push_back(entry);
	}

	// Create the list of sound buffers that contains
	// the list of sounds that can be played
	std::list<std::string>::iterator itor;
	for (itor = sounds.begin();
		itor != sounds.end();
		itor++)
	{
		const char *sound = (*itor).c_str();
		SoundBuffer *buffer =
			Sound::instance()->fetchOrCreateBuffer(
				(char *) getDataFile(sound));
		sounds_.push_back(buffer);
	}
}

void BoidWorld::makeBoids(int boidCount, Vector &maxBounds, Vector &minBounds)
{
	BoidVector p;
	BoidVector attitude;      // roll, pitch, yaw
	BoidVector v;             // velocity vector

	for (int i=0; i<boidCount; i++)
	{
		// Set up attitude, position and velocity.
		do
		{
			p = BoidVector(
			rand() % 200 + 25, 
			rand() % ((int)maxBounds[2] - (int)minBounds[2] - 2) + (int) minBounds[2] + 1,
			rand() % 200 + 25);
		} while (ScorchedClient::instance()->getLandscapeMaps().
			getGroundMaps().getInterpHeight((float) p.x, (float) p.z) > p.y);

		attitude = BoidVector(
			rand() % 200 + 25, 
			(maxBounds[2] + minBounds[2]) / 2,
			rand() % 200 + 25);
		v  = Direction(attitude) * (maxVelocity_) / 4.0;

		// Add to world
		Boid *boid = new Boid(this, i + 1, p, v);
		getBoids().push_back(boid);
	}
}

void BoidWorld::makeObstacles(Vector &maxBounds, Vector &minBounds) 
{
	ScorchedBoidsObstacle *o = 
		new ScorchedBoidsObstacle(maxBounds, minBounds);
	getObstacles().push_back(o);
}

void BoidWorld::simulate(float frameTime)
{
	const float StepTime = 0.04f;
	elapsedTime_ += frameTime;

	stepTime_ += frameTime;
	if (stepTime_ > StepTime)
	{
		halfTime_ = !halfTime_;
		for (int i=0; i<getBoidCount(); i++) 
		{
			getBoids()[i]->update(elapsedTime_);
			if (halfTime_) getBoids()[i]->updateslow();
		}

		stepTime_ = 0.0f;
	}

	const float StepTime2 = 0.5f;
	stepTime2_ += frameTime * 20.0f;
	while (stepTime2_ > StepTime2)
	{
		bird_->simulate(StepTime2);
		stepTime2_ -= StepTime2;

		std::vector<SoundEntry>::iterator itor;
		for (itor = currentSounds_.begin();
			itor != currentSounds_.end();
			itor++)
		{
			SoundEntry &entry = (*itor);
			if (entry.boid)
			{
				if (entry.source->getPlaying())
				{
					Vector position(
						(float) entry.boid->getPosition().x,
						(float) entry.boid->getPosition().y,
						(float) entry.boid->getPosition().z);
					Vector velocity(
						(float) entry.boid->getVelocity().x,
						(float) entry.boid->getVelocity().y,
						(float) entry.boid->getVelocity().z);
					entry.source->setPosition(position);
					entry.source->setVelocity(velocity);
				}
				else
				{
					// Finished
					entry.boid = 0;
				}
			}
		}
	}

	soundCurrentTime_ += frameTime;
	if (!sounds_.empty() && soundCurrentTime_ > soundNextTime_)
	{
		soundCurrentTime_ = 0.0f;
		soundNextTime_ = soundMinTime_ + soundMaxTime_ * RAND;

		std::vector<SoundEntry>::iterator itor;
		for (itor = currentSounds_.begin();
			itor != currentSounds_.end();
			itor++)
		{
			SoundEntry &entry = (*itor);
			if (!entry.boid)
			{
				entry.boid = boids_[boidSoundIndex_++ % boids_.size()];

				Vector position(
					(float) entry.boid->getPosition().x,
					(float) entry.boid->getPosition().y,
					(float) entry.boid->getPosition().z);
				Vector velocity(
					(float) entry.boid->getVelocity().x,
					(float) entry.boid->getVelocity().y,
					(float) entry.boid->getVelocity().z);
				entry.source->setPosition(position);
				entry.source->setVelocity(velocity);
				entry.source->play(sounds_[rand() % sounds_.size()]);
				break;
			}
		}
	}
}

void BoidWorld::draw()
{
	GLState state(GLState::TEXTURE_OFF | GLState::BLEND_OFF);

	glColor3f(1.0f, 0.0f, 0.0f);
	for (int i=0; i<getBoidCount(); i++) 
	{
		Boid *boid = getBoids()[i];
		const BoidVector &position = boid->getPosition();
		Vector pos((float) position.x, (float) position.z, (float) position.y);
		if (!GLCameraFrustum::instance()->sphereInFrustum(pos))
		{
			continue;
		}

		glPushMatrix();
			glTranslated(position.x, position.z, position.y);
			glRotated(-boid->yaw / 3.14f * 180.0f, 0.0f, 0.0f, 1.0f);
			glRotated(boid->pitch / 3.14f * 180.0f, 1.0f, 0.0f, 0.0f);
			glRotated(boid->dampedroll / 3.14f * 180.0f, 0.0f, 1.0f, 0.0f);
			glScalef(modelSize_, modelSize_, modelSize_);
		
			bird_->draw();
		glPopMatrix();
	}
}

#include <GLEXT/GLState.h>
#include <GLEXT/GLCameraFrustum.h>
#include <3dsparse/ModelStore.h>
#include <client/ScorchedClient.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/HeightMap.h>
#include <boids/BoidWorld.h>
#include <boids/Boid.h>
#include <boids/Obstacle.h>
#include <boids/ScorchedBoidsObstacle.h>
#include <stdlib.h>

BoidWorld::BoidWorld(
	ModelID &birdModel, 
	int boidCount, 
	int maxZ, int minZ) : 
	visibilityMatrix_(0), 
	elapsedTime_(0.0f), stepTime_(0.0f)
{
	// Create boids
	makeBoids(boidCount, maxZ, minZ);
	makeObstacles(maxZ, minZ);

	// Create bird model
	bird_ = ModelStore::instance()->loadOrGetArray(birdModel);
}

BoidWorld::~BoidWorld()
{
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
}


void BoidWorld::makeBoids(int boidCount, int maxZ, int minZ)
{
	BoidVector p;
	BoidVector attitude;      // roll, pitch, yaw
	BoidVector v;             // velocity vector
	BoidVector d(1, .2, .75); // dimensions of boid (RAD, height, length)
	double mv = 15;

	for (int i=0; i<boidCount; i++)
	{
		// Set up attitude, position and velocity.
		do
		{
			p = BoidVector(
			rand() % 200 + 25, 
			rand() % (maxZ - minZ - 2) + minZ + 1,
			rand() % 200 + 25);
		} while (ScorchedClient::instance()->getLandscapeMaps().
			getHMap().getInterpHeight((float) p.x, (float) p.z) > p.y);

		attitude = BoidVector(
			rand() % 200 + 25, 
			(maxZ + minZ) / 2,
			rand() % 200 + 25);
		v  = Direction(attitude) * (mv) / 4.0;

		Boid *boid = new Boid(this, i + 1, p, v, d);

		// Set mac acceleration, cruising distance, and max velocity.
		boid->maxAcceleration = 0.65;
		boid->cruiseDistance = 0.75;
		boid->maxVelocity = mv;

		// Add to world
		getBoids().push_back(boid);
	}
}

void BoidWorld::makeObstacles(int maxZ, int minZ) 
{
	Box *b = 
		new Box(
			BoidVector(256, maxZ, 256), 
			BoidVector(0, minZ, 0));
	getObstacles().push_back(b);
	ScorchedBoidsObstacle *o = 
		new ScorchedBoidsObstacle();
	getObstacles().push_back(o);
}

void BoidWorld::simulate(float frameTime)
{
	const float StepTime = 0.04f;
	elapsedTime_ += frameTime;
	stepTime_ += frameTime;

	if (stepTime_ > StepTime)
	{
		for (int i=0; i<getBoidCount(); i++) 
		{
			getBoids()[i]->update(elapsedTime_);
		}

		stepTime_ = 0.0f;
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
		if (!GLCameraFrustum::instance()->pointInFrustum(pos))
		{
			continue;
		}

		glPushMatrix();
			glTranslated(position.x, position.z, position.y);
			glRotated(-boid->yaw / 3.14 * 180.0, 0.0f, 0.0f, 1.0f);
			glRotated(boid->pitch / 3.14 * 180.0, 1.0f, 0.0f, 0.0f);
			glRotated(boid->roll / 3.14 * 180.0, 0.0f, 1.0f, 0.0f);
			glScalef(0.03f, 0.03f, 0.03f);
		
			bird_->draw();
		glPopMatrix();
	}
}

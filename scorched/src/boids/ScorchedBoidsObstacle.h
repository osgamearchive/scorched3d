#pragma once

#include <boids/Obstacle.h>

class ScorchedBoidsObstacle : public Obstacle
{
public:
	ScorchedBoidsObstacle();
	virtual ~ScorchedBoidsObstacle();

	virtual Obstacle *Clone(void) const 
		{ return new ScorchedBoidsObstacle; }

protected:
	virtual ISectData IntersectionWithRay(const BoidVector & raydirection,
		const BoidVector &rayorigin) const;
};

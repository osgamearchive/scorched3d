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

#if !defined(__INCLUDE_BoidWorldh_INCLUDE__)
#define __INCLUDE_BoidWorldh_INCLUDE__

#include <vector>
#include <list>
#include <common/Vector.h>

class SoundBuffer;
class VirtualSoundSource;
class ModelID;
class ModelRenderer;
class Boid;
class Obstacle;
class LandscapeTexBoids;
class BoidWorld
{
public:
	BoidWorld(LandscapeTexBoids *boids);
	virtual ~BoidWorld();

	void simulate(float frameTime);
	void draw();

	float getCruiseDistance() { return cruiseDistance_; }
	float getMaxVelocity() { return maxVelocity_; }
	float getMaxAcceleration() { return maxAcceleration_; }

	std::vector<Boid *> &getBoids() { return boids_; }
	std::vector<Obstacle *> &getObstacles() { return obstacles_; }
	int getBoidCount() { return (int) boids_.size(); } 
	int **getVisibilityMatrix() { return visibilityMatrix_; }

protected:
	// The currently playing sounds
	float soundMinTime_, soundMaxTime_;
	float soundNextTime_, soundCurrentTime_;
	int boidSoundIndex_;
	struct SoundEntry 
	{
		SoundEntry() : boid(0), source(0) {}
		VirtualSoundSource *source;
		Boid *boid;
	};
	std::vector<SoundEntry> currentSounds_;

	// All the possible sounds that may be played for these boids
	std::vector<SoundBuffer *> sounds_;

	std::vector<Boid *> boids_;
	// All boids
	// have access to this list, and use it to determine where all the other
	// boids are. 

	std::vector<Obstacle *> obstacles_;
	// Linked list of obstacles that every boid will try to avoid. By default
	// there are no obstacles.

	int **visibilityMatrix_;
	// See description in CalculateVisibilityMatrix() and the comments for the
	// visMatrix macro for more info.

	ModelRenderer *bird_;
	float modelSize_;
	float elapsedTime_;
	float stepTime_;
	float stepTime2_;
	bool halfTime_;

	float cruiseDistance_;
	float maxVelocity_;
	float maxAcceleration_;

	void makeBoids(int boidCount, Vector &maxBounds, Vector &minBounds);
	void makeObstacles(Vector &maxBounds, Vector &minBounds);
	void makeSounds(std::list<std::string> &sounds,
		int soundmaxsimul, float soundvolume);

};

#endif // __INCLUDE_BoidWorldh_INCLUDE__

#pragma once

#include <vector>

class ModelID;
class GLVertexSet;
class Boid;
class Obstacle;
class BoidWorld
{
public:
	BoidWorld(ModelID &birdModel, 
		int boidCount, int maxZ, int minZ);
	virtual ~BoidWorld();

	void simulate(float frameTime);
	void draw();

	std::vector<Boid *> &getBoids() { return boids_; }
	std::vector<Obstacle *> &getObstacles() { return obstacles_; }
	int getBoidCount() { return (int) boids_.size(); } 
	int **getVisibilityMatrix()
	{
		if (!visibilityMatrix_)
		{
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
		}

		return visibilityMatrix_; 
	}

protected:
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

	GLVertexSet *bird_;
	float elapsedTime_;
	float stepTime_;

	void makeBoids(int boidCount, int maxZ, int minZ);
	void makeObstacles(int maxZ, int minZ);

};

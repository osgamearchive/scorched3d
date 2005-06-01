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

class ModelID;
class ModelRenderer;
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

	ModelRenderer *bird_;
	float elapsedTime_;
	float stepTime_;
	float stepTime2_;
	bool halfTime_;

	void makeBoids(int boidCount, int maxZ, int minZ);
	void makeObstacles(int maxZ, int minZ);

};

#endif // __INCLUDE_BoidWorldh_INCLUDE__
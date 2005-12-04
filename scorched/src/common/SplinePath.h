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

#if !defined(__INCLUDE_SplinePathh_INCLUDE__)
#define __INCLUDE_SplinePathh_INCLUDE__

#include <common/Vector.h>
#include <vector>

class SplinePath
{
public:
	SplinePath();
	virtual ~SplinePath();

	void generate(
		std::vector<Vector> &inPoints, 
		int resolution = 200,
		int polynomials = 3,
		float pointsPerSecond = 5.0f);

	void draw();
	void simulate(float frameTime);
	void getPathAttrs(Vector &position, Vector &direction);

	std::vector<Vector> &getControlPoints() { return controlPoints_; }
	std::vector<Vector> &getPathPoints() { return pathPoints_; }

protected:
	std::vector<Vector> controlPoints_;
	std::vector<Vector> pathPoints_;
	float pathTime_;
	float pointsPerSecond_;
};

#endif // __INCLUDE_SplinePathh_INCLUDE__

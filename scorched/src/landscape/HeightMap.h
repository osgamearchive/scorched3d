////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
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

#if !defined(AFX_HEIGHTMAP_H__F4CB4CAD_C592_4183_AFB2_016FC66C144A__INCLUDED_)
#define AFX_HEIGHTMAP_H__F4CB4CAD_C592_4183_AFB2_016FC66C144A__INCLUDED_

#include <stdlib.h>
#include <common/ProgressCounter.h>
#include <common/Vector.h>
#include <common/Defines.h>

class Line;

class HeightMap  
{
public:
	HeightMap(int width);
	virtual ~HeightMap();

	void reset();
	void resetMinHeight();

	void generateNormals(int minX, int maxX, int minY, 
		int maxY, ProgressCounter *counter = 0);
	int getWidth() { return width_; }

	inline float getHeight(int w, int h) { 
		if (w >= 0 && h >= 0 && w<=width_ && h<=width_) 
			return hMap_[(width_+1) * h + w]; 
		return 0.0f; }
	float getInterpHeight(float w, float h);

	inline Vector &getNormal(int w, int h) {
		if (w >= 0 && h >= 0 && w<=width_ && h<=width_) 
			return normals_[(width_+1) * h + w]; 
		return nvec; }
	void getInterpNormal(float w, float h, Vector &normal);

	bool getIntersect(Line &direction, Vector &intersect);

	float getMinHeight(int w, int h);
	int getMinWidth() { return minWidth_; }

	// Returns the actual internal HeightMap points
	// Should not be used
	float &getHeightRef(int w, int h); // Do not use to alter height
	void setHeight(int w, int h, float height);
	float *getData() { return hMap_; }

protected:
	static Vector nvec;
	int width_, minWidth_;
	float *hMap_;
	float *minMap_;
	Vector *normals_;

	void performItteration();
	float getDist(Vector &start, Vector &dir, Vector &pos);
	bool getVector(Vector &vec, int x, int y);
	void getVectorPos(int pos, int &x, int &y, int dist=1);
};

#endif // !defined(AFX_HEIGHTMAP_H__F4CB4CAD_C592_4183_AFB2_016FC66C144A__INCLUDED_)


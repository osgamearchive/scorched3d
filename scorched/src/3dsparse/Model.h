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

#if !defined(__INCLUDE_Modelh_INCLUDE__)
#define __INCLUDE_Modelh_INCLUDE__

#pragma warning(disable: 4786)

#include <common/Vector.h>
#include <common/Face.h>
#include <string>
#include <vector>
#include <list>

// The base class container for, err, models (3d models)
class Model
{
public:
	Model(char *name);
	virtual ~Model();

	// Returns a list of triangle triplets that can be sent
	// to glBegin(GL_TRIANGLES);
	// and a list of normals for each triangle point.
	// Detail is the LOD for the returned mesh (1.0 = full, 0.0 = none)
	virtual void getArray(std::list<Vector> &triList, 
		std::list<Vector> &normalList,
		float detail = 1.0f);

	Vector &getMax() { return max_; }
	Vector &getMin() { return min_; }
	Vector &getColor() { return color_; }
	const char *getName() { return name_.c_str(); }

	virtual void centre(Vector &centre);
	virtual void scale(float scalef);

	// Used by parser
	virtual void setColor(Vector &color);
	virtual void setFaceNormal(Vector &normal, int face, int index);
	virtual void insertVertex(Vector &newVertex);
	virtual void insertFace(Face &newFace);
	
protected:
	std::string name_;
	std::vector<Vector> vertexes_;
	std::vector<Face> faces_;
	Vector max_, min_;
	Vector color_; // Color for meshes with no texture (material)

	// All used only for LOD computations
	bool computedCollapseCosts_; // Has the collapse cost been calculated
	std::vector<int> map_; // Cached collapsed costs
	int mapIndex(int pos, float currentReduction); 
	Vector *getVertex(int pos);
	void computeCollapseCosts();
};


#endif

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

#include <GLEXT/GLVertexArray.h>
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

	// Detail is the LOD for the returned mesh (1.0 = full, 0.0 = none)
	virtual GLVertexArray *getArray(bool useTextures, float detail = 1.0f);

	Vector &getVertex(int pos);

	Vector &getMax() { return max_; }
	Vector &getMin() { return min_; }
	Vector &getColor() { return color_; }
	const char *getName() { return name_.c_str(); }
	const char *getTextureName() { return texture_.c_str(); }
	const char *getATextureName() { return atexture_.c_str(); }

	std::vector<Vector> &getVertices() { return vertexes_; }
	std::vector<Face> &getFaces() { return faces_; }

	virtual void centre(Vector &centre);
	virtual void scale(float scalef);

	// Used by parser
	virtual void setTextureName(const char *name) { texture_ = name; }
	virtual void setATextureName(const char *name) { atexture_ = name; }
	virtual void setColor(Vector &color);
	virtual void setFaceNormal(Vector &normal, int face, int index);
	virtual void setFaceTCoord(Vector &tcoord, int face, int index);
	virtual void insertVertex(Vector &newVertex);
	virtual void insertFace(Face &newFace);
	
protected:
	std::string name_;
	std::vector<Vector> vertexes_;
	std::vector<Face> faces_;
	Vector max_, min_;
	Vector color_; // Color for meshes with no texture (material)
	std::string texture_;
	std::string atexture_; // Alpha texture

	GLVertexArray *getNoTexArray(float detail);
	GLVertexArray *getTexArray(float detail);

	// All used only for LOD computations
	bool computedCollapseCosts_; // Has the collapse cost been calculated
	std::vector<int> map_; // Cached collapsed costs
	int mapIndex(int pos, float currentReduction); 
	void computeCollapseCosts();
	void formArray(
		std::list<Vector> &triList, 
		std::list<Vector> &normalList,
		std::list<Vector> &texCoordList,
		float detail);

};


#endif

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


// ASEModel.h: interface for the ASEModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASEMODEL_H__5164D8CF_5506_4E58_BB87_A89EDC315B0B__INCLUDED_)
#define AFX_ASEMODEL_H__5164D8CF_5506_4E58_BB87_A89EDC315B0B__INCLUDED_

#pragma warning(disable: 4786)


#include <common/Vector.h>
#include <common/Face.h>
#include <string>
#include <vector>
#include <list>

class ASEModel  
{
public:
	ASEModel(char *name);
	virtual ~ASEModel();

	// Returns a list of triangle triplets that can be sent
	// to glBegin(GL_TRIANGLES);
	// and a list of normals for each triangle point.
	// Detail is the LOD for the returned mesh (1.0 = full, 0.0 = none)
	void getArray(std::list<Vector> &triList, 
		std::list<Vector> &normalList,
		float detail = 1.0f);

	Vector &getMax() { return max_; }
	Vector &getMin() { return min_; }
	Vector &getColor() { return color_; }
	const char *getName() { return name_.c_str(); }

	// Used by parser
	void insertVertex(Vector &newVertex);
	void insertFace(Face &newFace);
	void setColor(Vector &color);
	void setTmRow(Vector &row, int index);
	void setFaceNormal(Vector &normal, int face, int index);

	void centre(Vector &centre);
	void scale(float scalef);
	
protected:
	std::string name_;
	std::vector<Vector> vertexes_;
	std::vector<Face> faces_;
	std::vector<int> map_;
	Vector color_;
	Vector tmRow_[3];
	Vector max_, min_;
	bool computedCollapseCosts_;

	int mapIndex(int pos, float currentReduction);
	void computeCollapseCosts();
	Vector *getVertex(int pos);
};

#endif // !defined(AFX_ASEMODEL_H__5164D8CF_5506_4E58_BB87_A89EDC315B0B__INCLUDED_)

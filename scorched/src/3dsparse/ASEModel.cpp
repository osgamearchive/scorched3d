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


// ASEModel.cpp: implementation of the ASEModel class.
//
//////////////////////////////////////////////////////////////////////

#include <common/Defines.h>
#include <GLEXT/GLState.h>
#include <3dsparse/ASEModel.h>
#include <3dsparse/MeshLOD.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ASEModel::ASEModel(char *name) : name_(name), computedCollapseCosts_(false)
{

}

ASEModel::~ASEModel()
{

}

int ASEModel::mapIndex(int i, float currentReduction)
{
	if (currentReduction == 1.0f) return i;

	int newVertexVal = (int (currentReduction * (float) vertexes_.size()));

	int returnval = i;
	while (returnval > newVertexVal)
	{
		returnval=map_[returnval];
	}

	return returnval;
}

void ASEModel::insertVertex(Vector &newVertex)
{
	if (vertexes_.empty())
	{
		max_ = newVertex;
		min_ = newVertex;
	}
	else
	{
		max_[0] = MAX(max_[0], newVertex[0]);
		max_[1] = MAX(max_[1], newVertex[1]);
		max_[2] = MAX(max_[2], newVertex[2]);

		min_[0] = MIN(min_[0], newVertex[0]);
		min_[1] = MIN(min_[1], newVertex[1]);
		min_[2] = MIN(min_[2], newVertex[2]);
	}

	vertexes_.push_back(newVertex);
}

void ASEModel::insertFace(Face &newFace)
{
	faces_.push_back(newFace);
}

void ASEModel::setColor(Vector &color)
{
	color_ = color;
}

void ASEModel::setTmRow(Vector &row, int index)
{
	DIALOG_ASSERT(index < 3);
	tmRow_[index] = row.Normalize();
}

void ASEModel::setFaceNormal(Vector &normal, int face, int index)
{
	DIALOG_ASSERT(index < 3);
	DIALOG_ASSERT(face < (int) faces_.size());

	Vector n(normal[0] * tmRow_[0][0] + normal[1] * tmRow_[1][0] + normal[2] * tmRow_[2][0],
		normal[0] * tmRow_[0][1] + normal[1] * tmRow_[1][1] + normal[2] * tmRow_[2][1],
		normal[0] * tmRow_[0][2] + normal[1] * tmRow_[1][2] + normal[2] * tmRow_[2][2]);

	faces_[face].normal[index] = n;
}

Vector *ASEModel::getVertex(int pos)
{
	DIALOG_ASSERT(pos < (int) vertexes_.size());
	return &vertexes_[pos];
}

void ASEModel::computeCollapseCosts()
{
	if (!computedCollapseCosts_)
	{
		MeshLOD::progressiveMesh(vertexes_, faces_, map_);
		computedCollapseCosts_ = true;
	}
}

void ASEModel::centre(Vector &centre)
{
	std::vector<Vector>::iterator itor;
	for (itor = vertexes_.begin();
		itor != vertexes_.end();
		itor++)
	{
		(*itor) -= centre;
	}
	max_ -= centre;
	min_ -= centre;
}

void ASEModel::scale(float scalef)
{
	std::vector<Vector>::iterator itor;
	for (itor = vertexes_.begin();
		itor != vertexes_.end();
		itor++)
	{
		(*itor) *= scalef;
	}
	max_ *= scalef;
	min_ *= scalef;
}

void ASEModel::getArray(std::list<Vector> &triList, 
						std::list<Vector> &normalList,
						float detail)
{
	if (detail != 1.0f) computeCollapseCosts();

	std::vector<Face>::iterator itor;
	for (itor = faces_.begin();
		itor != faces_.end();
		itor++)
	{
		int posA = mapIndex(itor->v[0], detail);
		int posB = mapIndex(itor->v[1], detail);
		int posC = mapIndex(itor->v[2], detail);
		if ( posA == posB || posA == posC || posB == posC )
		{
			// Do not draw			
		}
		else
		{
			Vector *a = getVertex(posA);
			Vector *b = getVertex(posB);
			Vector *c = getVertex(posC);

			triList.push_back(*a);
			triList.push_back(*b);
			triList.push_back(*c);

			normalList.push_back(itor->normal[0]);
			normalList.push_back(itor->normal[1]);
			normalList.push_back(itor->normal[2]);
		}
	}
}

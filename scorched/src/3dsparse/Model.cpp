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

#include <common/Defines.h>
#include <3dsparse/Model.h>
#include <3dsparse/MeshLOD.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Model::Model(char *name) : 
	name_(name), computedCollapseCosts_(false), 
	color_(0.5f, 0.5f, 0.5f)
{

}

Model::~Model()
{

}

void Model::setColor(Vector &color)
{
	color_ = color;
}

void Model::insertVertex(Vector &newVertex)
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

void Model::insertFace(Face &newFace)
{
	faces_.push_back(newFace);
}

void Model::setFaceNormal(Vector &normal, int face, int index)
{
	DIALOG_ASSERT(index < 3);
	DIALOG_ASSERT(face < (int) faces_.size());

	faces_[face].normal[index] = normal;
}

Vector *Model::getVertex(int pos)
{
	DIALOG_ASSERT(pos < (int) vertexes_.size());
	return &vertexes_[pos];
}

void Model::centre(Vector &centre)
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

void Model::scale(float scalef)
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

void Model::computeCollapseCosts()
{
	if (!computedCollapseCosts_)
	{
		MeshLOD::progressiveMesh(vertexes_, faces_, map_);
		computedCollapseCosts_ = true;
	}
}

int Model::mapIndex(int i, float currentReduction)
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

void Model::getArray(std::list<Vector> &triList, 
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
